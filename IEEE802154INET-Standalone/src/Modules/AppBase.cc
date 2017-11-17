//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "AppBase.h"
#include "InetSimpleBattery.h"
#include "omnetpp.h"

bool AppBase::firstDevice;

Define_Module(AppBase);

simsignal_t AppBase::rcvdPkSignal = registerSignal("rcvdPk");
simsignal_t AppBase::sentPkSignal = registerSignal("sentPk");
simsignal_t AppBase::sentPkConfSignal = registerSignal("sentPkConf");
simsignal_t AppBase::delaySignal = registerSignal("delay");



void AppBase::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == 0)
    {
        isAP = par("isAP"); // used by SupervisedMobility

        double hostsWithSameProduct = getParentModule()->getParentModule()->par("hostsWithSameProduct");
        numHosts = getParentModule()->getParentModule()->par("numHosts");
        numProducts = int(numHosts/(numHosts*hostsWithSameProduct));

        packetLengthPar = &par("packetLength");

        numSent = 0;
        numSentConf = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numSentConf);
        WATCH(numReceived);

        rcvdPkSignal = registerSignal("rcvdPk");
        sentPkSignal = registerSignal("sentPk");
        sentPkConfSignal = registerSignal("sentPkConf");


        if (hasPar("appDebug")){
            appDebug = par("appDebug").boolValue();
        }
        else{
            appDebug = false;
        }

        double seed = dblrand();

        TXoption = par("TXoption");
        logicalChannel = par("LogicalChannel");

        firstDevice = true;
        associateSuccess = false;

        WATCH(firstDevice);
        WATCH(associateSuccess);

        // Check if startWithoutStartRequest was enabled by user
        if (getModuleByPath("net.Box[0].NIC.MAC.IEEE802154Mac")->par("startWithoutStartReq").boolValue() == false){
            appEV << "Sending Start Request in " << seed << endl;
            selfMsg = new cMessage("LLC-Start");
            selfMsg->setKind(0);
            scheduleAt(seed, selfMsg);
        }
        else{
            appEV << "Starting without an explicit Start Request right now (startwithoutStartReq == true) \n";
        }

        scanChannels = par("ScanChannels");
        scanDuration = par("ScanDuration");
        scanPage = par("ScanPage");
        scanType = par("ScanType");
        pollFreq = par("PollFrequency");

        if (TXoption >= 4){
            pollTimer = new cMessage("App-POLL-Timer");
            appEV << "TXoption set to indirect - starting Poll timer \n";
            pollTimer->setKind(1);
            scheduleAt(pollFreq, pollTimer);
        }
    }

    else if (stage == 3){
        startApp();
    }
}

void AppBase::startApp()
{
    //
}

void AppBase::handleMessage(cMessage *msg)
{
    appEV << "Got Message in Application - : " << msg->getName() << endl;

    if (msg->isSelfMessage()) {
        if (msg->getKind() == 0) {
            appEV << "Got Startup Msg - Sending out Scan Request \n";
            genScanReq();
            delete (msg);
        }
        else if (msg->getKind() == 1){
            appEV << "Got POLL Timer - Sending POLL Request \n";
            genPollReq();
            scheduleAt(simTime() + pollFreq, msg);
        }
        handleSelfMessage(msg);
        return;
    }

    else if (msg->arrivedOn("inMngt")){
        if (dynamic_cast<beaconNotify*>(msg)){
            if (associateSuccess == false){
                beaconNotify* bN = check_and_cast<beaconNotify*>(msg);
                coordAddrMode = bN->getPANDescriptor().CoordAddrMode;
                coordPANId = bN->getPANDescriptor().CoordPANId;
                coordAddress = bN->getPANDescriptor().CoordAddress;  // shared by both 16 bit short address or 64 bit extended address
                logicalChannel = bN->getPANDescriptor().LogicalChannel;
                appEV << "Beacon Notify received an not yet associated -> generate Association Request for the existing PAN Coordinator \n";
                genAssoReq();
            }
            else{
                appEV << "Beacon Notify received and associated - no further processing of the Beacon Notify \n";
            }
        }

        if (dynamic_cast<StartConfirm*>(msg)){
            // TODO divide between "started your own PAN" and "found a PAN and starting association process" ??
            appEV << "Got MLME-START.confirm from lower layer \n";
        }

        if (dynamic_cast<ScanConfirm*>(msg)){
            ScanConfirm* scanConf = check_and_cast<ScanConfirm*>(msg);

            if (scanConf->getResultListSize() == 0){
                appEV << "Got MLME-SCAN.confirm with ResultListSize == 0 \n";

                if (firstDevice){
                    appEV << "First global device without results sends out MLME-START for Coordinator \n";

                    startMsg = new StartRequest("MLME-START.request");
                    startMsg->setPANId(par("PANId"));
                    startMsg->setLogicalChannel(logicalChannel);
                    startMsg->setChannelPage(par("ChannelPage"));
                    startMsg->setStartTime(par("StartTime"));
                    startMsg->setBeaconOrder(par("BeaconOrder"));
                    startMsg->setSuperframeOrder(par("SuperframeOrder"));
                    startMsg->setBatteryLifeExtension(par("BatteryLifeExtension"));
                    startMsg->setPANCoordinator(true);
                    startMsg->setCoordRealignment(false);   // override user parameter here since 1st device starts PAN and doesn't realign it

                    firstDevice = false;
                    send(startMsg, "outMngt");
                }
                else{
                    appEV << "No results - scan again \n";
                    genScanReq();
                }
            }
            else{
                appEV << "Got MLME-SCAN.confirm with ResultListSize > 0 \n";

                unsigned short panId = par("PANId");
                logicalChannel = par("LogicalChannel");
                unsigned short channelPage = par("ChannelPage");
                unsigned int startTime = par("StartTime");
                unsigned short beaconOrder = par("BeaconOrder");
                unsigned short superframeOrder = par("SuperframeOrder");
                bool batteryLifeExtension = par("BatteryLifeExtension");
                bool coordRealignment = par("CoordRealignment");

                startMsg = new StartRequest("MLME-START.request");
                startMsg->setPANId(panId);
                startMsg->setLogicalChannel(logicalChannel);
                startMsg->setChannelPage(channelPage);
                startMsg->setStartTime(startTime);
                startMsg->setBeaconOrder(beaconOrder);
                startMsg->setSuperframeOrder(superframeOrder);
                startMsg->setBatteryLifeExtension(batteryLifeExtension);
                startMsg->setCoordRealignment(coordRealignment);

                startMsg->setPANCoordinator(false);
                send(startMsg, "outMngt");
            }
        }

        if (dynamic_cast<AssociationConfirm*>(msg)){
            appEV << "Association Confirm received - association process was successful \n";
            associateSuccess = true;
        }

        if (dynamic_cast<OrphanIndication*>(msg)){
            // just for convenience of functional tests
            OrphanIndication* oi = check_and_cast<OrphanIndication*>(msg);
            genOrphanResp(oi);
        }

        delete (msg);
        return;
    } // if (msg->arrivedOn("inMngt"))

    if (msg->arrivedOn("inData")){
        handlePacket(check_and_cast<cPacket *>(msg));
    }// if (msg->arrivedOn("inData"))

    if (ev.isGUI())
    {
//        cObject* target = this->getParentModule()->getSubmodule("battery");
//        InetSimpleBattery* battery = check_and_cast<InetSimpleBattery*>(target);
//        double energy = battery->getConsumedEnergy(); // mWs
//        energy = energy/60/60; //mWh
//        double amp = energy/battery->getVoltage();
        char buf[40];
        sprintf(buf, "r: %d\ns: %d", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
        cModule* host = this->getParentModule();
        host->getDisplayString().setTagArg("t", 0, buf);
        host->getDisplayString().setTagArg("t", 2, "black");
    }
}

void AppBase::handleSelfMessage(cMessage* msg)
{
    //
}

void AppBase::generatePacket()
{
    //
}

void AppBase::handlePacket(cPacket *msg)
{
    //
}


void AppBase::genAssoReq()
{
    AssociationRequest* assoReq = new AssociationRequest("MLME-ASSOCIATE.request");
    MACAddressExt* coordId = new MACAddressExt(coordPANId);
    assoReq->setCoordAddrMode(coordAddrMode);
    assoReq->setCoordPANId(*coordId);
    assoReq->setCoordAddress(coordAddress);
    DevCapability capInfo;
    capInfo.alloShortAddr = true;
    capInfo.FFD = true;
    capInfo.recvOnWhenIdle = true;
    capInfo.secuCapable = false;
    assoReq->setCapabilityInformation(capInfo);
    assoReq->setChannelPage(0);
    assoReq->setLogicalChannel(logicalChannel);

    send(assoReq, "outMngt");
}

void AppBase::genPollReq()
{
    PollRequest* pollReq = new PollRequest("MLME-POLL.request");
    pollReq->setCoordAddrMode(addrShort);
    pollReq->setCoordPANId(coordPANId);
    pollReq->setCoordAddress(coordAddress);

    send(pollReq, "outMngt");
}

void AppBase::genScanReq()
{
    ScanRequest* scanReq = new ScanRequest("MLME-SCAN.request");
    scanReq->setScanType(scanType);          // see enum
    scanReq->setScanChannels(scanChannels);  // 27 bit indicating the channels to be scanned
    scanReq->setScanDuration(scanDuration);  // time spent on scanning each channel
    scanReq->setChannelPage(scanPage);

    send(scanReq, "outMngt");
}

void AppBase::genOrphanResp(OrphanIndication* oi)
{
    OrphanResponse* oR = new OrphanResponse("MLME-ORPHAN.response");
    oR->setOrphanAddress(oi->getOrphanAddress());
    oR->setShortAddress(0xffff);
    oR->setAssociatedMember(false);

    send(oR, "outMngt");
}

void AppBase::printPacket(cPacket *msg)
{
    if (dynamic_cast<mcpsDataInd*>(msg) != NULL) {
        mcpsDataInd* ind = check_and_cast<mcpsDataInd*>(msg);
        appEV << ind->getName() << endl;
        appEV << "Seq-Nr:       " << (int)ind->getDSN() << endl;
        appEV << "Src:          " << ind->getSrcAddr() << endl;
        appEV << "Dest:         " << ind->getDstAddr() << endl;
        appEV << "Length:       " << (int)ind->getMsduLength() << "Byte" << endl;
        const cPacket* packet = ind->getEncapsulatedPacket();
        appEV << "Content:      " << packet->getName() << endl;
        appEV << "Link Quality: " << (int)ind->getMpduLinkQuality() << endl;
    }
    else if (dynamic_cast<mcpsDataReq*>(msg) != NULL) {
        mcpsDataReq* req = check_and_cast<mcpsDataReq*>(msg);
        appEV << req->getName() << endl;
        appEV << "Seq-Nr:  " << (int)req->getMsduHandle() << endl;
        appEV << "Dest:    " << req->getDstAddr() << endl;
        appEV << "Length:  " << (int)req->getMsduLength() << "Byte" << endl;
        const cPacket* packet = req->getEncapsulatedPacket();
        appEV << "Content: " << packet->getName() << endl;
    }
//    else if (dynamic_cast<mcpsDataConf*>(msg) != NULL) {
//        mcpsDataConf* conf = check_and_cast<mcpsDataConf*>(msg);
//        appEV << conf->getName() << endl;
//        appEV << "Seq-Nr:  " << (int)conf->getMsduHandle() << endl;
//        appEV << "Status:  " << (int)conf->getStatus() << endl;
//        appEV << "Content: " << conf->decapsulate()->getName() << endl;
//    }
    else
        appEV<< "unexpected Packet: "<< msg << endl;
}

AppBase::AppBase()
{
    packetLengthPar = NULL;
    logicalChannel = 0;
    TXoption = 0;
    msgHandle = 0;
    associateSuccess = false;
    coordAddrMode = 0;
    coordPANId = 0;
    scanChannels = 0;
    scanDuration = 0.0;
    scanPage = 0;
    scanType = 0;
    startMsg = NULL;
    selfMsg = NULL;
    pollTimer = NULL;
}

AppBase::~AppBase()
{
    if (TXoption >= 4){
        if (pollTimer)
            cancelAndDelete(pollTimer);
    }
}
