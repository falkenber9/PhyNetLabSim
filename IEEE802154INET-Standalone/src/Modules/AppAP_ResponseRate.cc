//
// Copyright (C) 2017 Jens Drenhaus     (PhyNetLabSim adaptations)
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

#include "AppAP_ResponseRate.h"
#include "AppBox_ResponseRate.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "EnergyTracker.h"
#include "SupervisedMobility.h"

using namespace std;

Define_Module(AppAP_ResponseRate);

void AppAP_ResponseRate::initialize(int stage)
{
    AppBase::initialize(stage);

    if (stage == 0){
        appStartTime = par("appStartTime");
        appStopTime = par("appStopTime");
        if (appStopTime >= SIMTIME_ZERO && appStopTime < appStartTime)
            error("Invalid appStartTime/appStopTime parameters");

        responseRate = par("responseRate");
        int numHosts = getParentModule()->getParentModule()->par("numHosts");
        double tmp = numHosts*responseRate;
        responseThr = (int)ceil(tmp); // round up allways: 2.3 -> 3.0
        responseCnt = 0;
        responseSet.clear();

        qrt = SIMTIME_ZERO;

        sentNotifyCnt = 0;

        sendTimeMap.clear();
        sendTime = 0;
        requestID = 0;

        timer = new cMessage("sendTimer");
    }
}

void AppAP_ResponseRate::startApp()
{
    if (isEnabled())
        scheduleNextPacket(-1);
}

bool AppAP_ResponseRate::isEnabled()
{
    return (responseCnt < responseThr);
}

void AppAP_ResponseRate::scheduleNextPacket(int cnt)
{
    simtime_t next;
    if (cnt == -1)
    {
        next = simTime() <= appStartTime ? appStartTime : simTime();
        timer->setKind(START);
    }
    else
    {
        //next = simTime();
        next = (simTime() + SimTime(1, SIMTIME_S));
        timer->setKind(NEXT);
    }
    if (appStopTime < SIMTIME_ZERO || next < appStopTime)
        scheduleAt(next, timer);
}


void AppAP_ResponseRate::handleSelfMessage(cMessage* msg)
{
    if (msg == timer){
        if (msg->getKind() == START){
            initDestAddrs();
        }
        if (!destAddresses.empty()){
            if(isEnabled()){
                generatePacket();
            }
        }
        else
            throw cRuntimeError("App Error: no destAdresses stored in AP with index %d", getParentModule()->getIndex());
    }
}

void AppAP_ResponseRate::handlePacket(cPacket* msg)
{
    // this can only be a confirm or indication
    if (dynamic_cast<mcpsDataInd*>(msg))
    {
        mcpsDataInd* ind = check_and_cast<mcpsDataInd*>(msg);
        AppPayload* payload = check_and_cast<AppPayload*>(ind->getEncapsulatedPacket());

        long reqNr = payload->getReqNr();
        std::map<long,simtime_t>::iterator it = sendTimeMap.find(reqNr);
        if(it == sendTimeMap.end()){
            error("[BOX-APP]: Can't find reqNr!");
        }
        simtime_t start = it->second;
        simtime_t delay = simTime()-start;

        emit(delaySignal, delay);
        emit(rcvdPkSignal, msg);

        updateResponseList(payload);
        if((responseCnt == responseThr) && (qrt == SIMTIME_ZERO)){
            qrt = simTime();
        }

        appEV << "Received MSDU (MAC Service Data Unit): " << endl;
        printPacket(msg);
        delete msg;
        numReceived++;

    }
    else if (dynamic_cast<mcpsDataConf*>(msg))
    {
        mcpsDataConf* conf = check_and_cast<mcpsDataConf*>(msg);
        appEV << "Got a Confirmation from MAC entity with Status: " << MCPSStatusToString(MCPSStatus(conf->getStatus())) << " for Message #" << (int) conf->getMsduHandle() << endl;

        emit(sentPkConfSignal, msg);
        //printPacket(msg);
        delete msg;
        numSentConf++;
    }
    else
    {
        error("[BOX-APP]: Undefined Message arrived on inData gate!");
    }
}

void AppAP_ResponseRate::generatePacket()
{
    char msgName[32];
    int id = intuniform(1,numProducts);
    sprintf(msgName, "PAGE-%d",id);
    AppPayload *payload = new AppPayload(msgName);
    payload->setByteLength(packetLengthPar->longValue());

    payload->setReqNr(requestID);
    sendTimeMap.insert(std::pair<long,simtime_t>(requestID,simTime()));
    requestID++;

    mcpsDataReq* data = new mcpsDataReq("MCPS-DATA.request");
    data->encapsulate(payload);

    data->setMsduHandle(msgHandle);
    if (msgHandle < 255){
        msgHandle++;
    }
    else {
        msgHandle = 0;
    }
    data->setMsduLength(payload->getByteLength());
    data->setTxOptions(TXoption);
    MACAddressExt* destination = chooseDestAddr();
    data->setDstAddr(*destination);

    appEV << "Sending MSDU (MAC Service Data Unit): " << endl;
    printPacket(data);
    emit(sentPkSignal, data);
    send(data, "outData");
    numSent++;
}

void AppAP_ResponseRate::responseSentNotify()
{
    Enter_Method("SendNotify");
    sentNotifyCnt++;
    if(sentNotifyCnt == numHosts){
        sentNotifyCnt = 0;
        scheduleNextPacket(1);
    }
}

void AppAP_ResponseRate::updateResponseList(AppPayload* msg)
{
    int index = msg->getHostIndex();
    std::set<int>::iterator it;
    std::pair<std::set<int>::iterator,bool> ret;

    ret = responseSet.insert(index);
    if(ret.second == true){
        responseCnt++;
    }
}

void AppAP_ResponseRate::initDestAddrs()
{
    destAddresses.clear();
    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *addr;
    while ((addr = tokenizer.nextToken()) != NULL)
    {
        MACAddressExt* result = new MACAddressExt(addr);
        destAddresses.push_back(*result);
    }
}

MACAddressExt* AppAP_ResponseRate::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    return &destAddresses[k];
}

void AppAP_ResponseRate::finish()
{
    cModule* module;

    module = getParentModule()->getSubmodule("battery");
    EnergyTracker* energyTrackerAP = check_and_cast<EnergyTracker*>(module);
    energyTrackerAP->deductAndCheck();

    module = getParentModule()->getSubmodule("mobility");
    SupervisedMobility* mobilityAP = check_and_cast<SupervisedMobility*>(module);
    double xAP = mobilityAP->getCurrentPosition().x;
    double yAP = mobilityAP->getCurrentPosition().y;
    double zAP = mobilityAP->getCurrentPosition().z;

    ofstream myfile;
    myfile.open ("results.txt", ios::app);


    EnergyTracker* energyTracker;
    double energy;
    int qrr = responseSet.size();
    int sendAttempts = numSent;
    SupervisedMobility* mobility;
    double x, y, z;

    myfile <<numHosts<<";"<<-1<<";"<<0<<";"<<xAP<<";"<<yAP<<";"<<zAP<<";"<<qrr<<";"<<sendAttempts<<";"<<qrt << endl;

    int i;
    for(i=0; i<numHosts; i++){
        module = getParentModule()-> getParentModule()->getSubmodule("Box", i)->getSubmodule("battery");
        energyTracker = check_and_cast<EnergyTracker*>(module);
        energy = energyTracker->getTotalConsumedEnergy();

        module = getParentModule()-> getParentModule()->getSubmodule("Box", i)->getSubmodule("mobility");
        mobility = check_and_cast<SupervisedMobility*>(module);
        x = mobility->getCurrentPosition().x;
        y = mobility->getCurrentPosition().y;
        z = mobility->getCurrentPosition().z;

        myfile << numHosts<<";"<<i<<";"<<energy<<";"<<x<<";"<<y<<";"<<z<<";"<<0<<";"<<0<<";"<<0 << endl;
    }

    myfile.close();
}

AppAP_ResponseRate::AppAP_ResponseRate()
{
    // default Constructor of AppBase is called
    timer = NULL;
}

AppAP_ResponseRate::~AppAP_ResponseRate()
{
    cancelAndDelete(timer);
    // Destructor of AppBase is called
}
