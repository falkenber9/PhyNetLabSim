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

#include "AppBox.h"

Define_Module(AppBox);


void AppBox::initialize(int stage)
{
    AppBase::initialize(stage);

    if (stage == 0){
        myProductID = intuniform(1,numProducts);
//        initColorMap();
//        cDisplayString& dispStr = getParentModule()->getDisplayString();
//        std::map<int,const char*>::iterator it = productColorMap.find(myProductID);
//        if(it == productColorMap.end())
//        {
//            error("[BOX-APP]: Not enough colors for products");
//        }
//        const char* value = it->second;
//        dispStr.setTagArg("i",1,value);
    }
}

void AppBox::handlePacket(cPacket * msg)
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
    }
    else
    {
        error("[BOX-APP]: Undefined Message arrived on inData gate!");
    }
}

void AppBox::generateResponse(long srcReqNr, MACAddressExt& srcAddr)
{
    char msgName[32];
    sprintf(msgName, "BoxResponse");
    AppPayload *newPayload = new AppPayload(msgName);
    newPayload->setByteLength(packetLengthPar->longValue());

    newPayload->setReqNr(srcReqNr);
    newPayload->setHostIndex(getParentModule()->getIndex());

    mcpsDataReq* data = new mcpsDataReq("MCPS-DATA.request");
    data->encapsulate(newPayload);

    data->setMsduHandle(msgHandle);
    if (msgHandle < 255)
    {
        msgHandle++;
    }
    else
    {
        msgHandle = 0;
    }
    data->setMsduLength(newPayload->getByteLength());
    data->setTxOptions(TXoption);
    MACAddressExt destination = srcAddr;
    data->setDstAddr(destination);

    appEV << "Sending MSDU (MAC Service Data Unit): " << endl;
    printPacket(data);
    emit(sentPkSignal, data);
    send(data, "outData");
    numSent++;
}

int AppBox::parseMsgName(const char* name)
{
    cStringTokenizer tokenizer(name);
    tokenizer.setDelimiter("-");
    const char *token;
    int id;
    if(tokenizer.hasMoreTokens()) {
        token = tokenizer.nextToken();
        // ignore first entry: "PAGE-xy"
    }
    if(tokenizer.hasMoreTokens()) {
        token = tokenizer.nextToken();
        id = (atoi(token)); // get Product ID
    }
    else {
        error("[BOX-APP]: Error in parse Product ID from name!");
    }
    return id;
}

void AppBox::initColorMap()
{
    productColorMap.insert(std::pair<int,const char*>(1,"#0000FF"));
    productColorMap.insert(std::pair<int,const char*>(2,"#800000"));
    productColorMap.insert(std::pair<int,const char*>(3,"#FFFF00"));
    productColorMap.insert(std::pair<int,const char*>(4,"#808000"));
    productColorMap.insert(std::pair<int,const char*>(5,"#00FF00"));
    productColorMap.insert(std::pair<int,const char*>(6,"#008000"));
    productColorMap.insert(std::pair<int,const char*>(7,"#00FFFF"));
    productColorMap.insert(std::pair<int,const char*>(8,"#008080"));
    productColorMap.insert(std::pair<int,const char*>(9,"#FF5733"));
    productColorMap.insert(std::pair<int,const char*>(10,"#000080"));
    productColorMap.insert(std::pair<int,const char*>(11,"#FF00FF"));
    productColorMap.insert(std::pair<int,const char*>(12,"#800080"));
    productColorMap.insert(std::pair<int,const char*>(13,"#808080"));
    productColorMap.insert(std::pair<int,const char*>(14,"#C0C0C0"));
    productColorMap.insert(std::pair<int,const char*>(15,"#FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(16,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(17,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(18,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(19,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(20,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(21,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(22,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(23,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(24,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(25,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(26,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(27,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(28,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(29,"FFFFFF"));
    productColorMap.insert(std::pair<int,const char*>(30,"FFFFFF"));

}

AppBox::AppBox()
{
    // default Constructor of AppBase is called
}

AppBox::~AppBox()
{
    // Destructor of AppBase is called
}
