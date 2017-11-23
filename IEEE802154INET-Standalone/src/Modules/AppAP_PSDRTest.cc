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

#include "AppAP_PSDRTest.h"

Define_Module(AppAP_PSDRTest);



void AppAP_PSDRTest::initialize(int stage)
{
    AppBase::initialize(stage);

    if (stage == 0){
        appStartTime = par("appStartTime");
        appStopTime = par("appStopTime");
        if (appStopTime >= SIMTIME_ZERO && appStopTime < appStartTime)
            error("Invalid appStartTime/appStopTime parameters");

        numPackets = par("numPackets");
        sendIntervalPar = &par("sendInterval");

        sendTimeMap.clear();
        sendTime = 0;
        requestID = 0;

        timer = new cMessage("sendTimer");
    }
}

void AppAP_PSDRTest::startApp()
{
    if(getParentModule()->getIndex()==0)
        int t = 3;
    if (isEnabled())
        scheduleNextPacket(-1);
}

bool AppAP_PSDRTest::isEnabled()
{
    return (numPackets == -1 || numSent < numPackets);
}


void AppAP_PSDRTest::scheduleNextPacket(simtime_t previous)
{
    simtime_t next;
    if (previous == -1)
    {
        next = simTime() <= appStartTime ? appStartTime : simTime();
        timer->setKind(START);
    }
    else
    {
        next = previous + sendIntervalPar->doubleValue();
        timer->setKind(NEXT);
    }
    if (appStopTime < SIMTIME_ZERO || next < appStopTime)
        scheduleAt(next, timer);
}

void AppAP_PSDRTest::handleSelfMessage(cMessage* msg)
{
    if (msg == timer){
        if(getParentModule()->getIndex()==0)
            int t = 4;
        if (msg->getKind() == START)
        {
            initDestAddrs();
        }
        if (!destAddresses.empty())
        {
            generatePacket();
            if (isEnabled())
                scheduleNextPacket(simTime());
        }
        else
            throw cRuntimeError("App Error: no destAdresses stored in AP with index %d", getParentModule()->getIndex());
    }
}

void AppAP_PSDRTest::handlePacket(cPacket* msg)
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

void AppAP_PSDRTest::generatePacket()
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

void AppAP_PSDRTest::initDestAddrs()
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

MACAddressExt* AppAP_PSDRTest::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    return &destAddresses[k];
}

AppAP_PSDRTest::AppAP_PSDRTest()
{
    // default Constructor of AppBase is called
    timer = NULL;
    sendIntervalPar = NULL;
}

AppAP_PSDRTest::~AppAP_PSDRTest()
{
    cancelAndDelete(timer);
    // Destructor of AppBase is called
}
