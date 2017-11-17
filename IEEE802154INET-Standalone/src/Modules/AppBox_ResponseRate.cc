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

#include "AppBox_ResponseRate.h"

Define_Module(AppBox_ResponseRate);

void AppBox_ResponseRate::initialize(int stage)
{
    AppBox::initialize(stage);

    if (stage == 0){
        cModule* module = getParentModule()->getParentModule()->getSubmodule("AccessPoint")->getSubmodule("application");
        ap = check_and_cast<AppAP_ResponseRate*>(module);
    }
}

void AppBox_ResponseRate::handlePacket(cPacket * msg)
{
    // this can only be a confirm or indication
    if (dynamic_cast<mcpsDataInd*>(msg))
    {
        mcpsDataInd* ind = check_and_cast<mcpsDataInd*>(msg);

        emit(rcvdPkSignal, msg);
        appEV << "Received MSDU (MAC Service Data Unit): " << endl;
        printPacket(msg);

        AppPayload* payload = check_and_cast<AppPayload*>(ind->getEncapsulatedPacket());
        int productID = parseMsgName(payload->getName());
        if(productID == myProductID){
            generateResponse(payload->getReqNr(), ind->getSrcAddr());
        }

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
        ap->responseSentNotify();
    }
    else
    {
        error("[BOX-APP]: Undefined Message arrived on inData gate!");
    }
}

AppBox_ResponseRate::AppBox_ResponseRate()
{
    // default Constructor of AppBase is called
}

AppBox_ResponseRate::~AppBox_ResponseRate()
{
    // Destructor of AppBase is called
}
