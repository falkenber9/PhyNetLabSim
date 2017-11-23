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

#ifndef __IEEE802154INET_STANDALONE_APPAP_PSDRTEST_H_
#define __IEEE802154INET_STANDALONE_APPAP_PSDRTEST_H_

#include <omnetpp.h>
#include "AppBase.h"
#include "mcpsData_m.h"
#include "MPDU_m.h"
#include "IEEE802154Fields.h"
#include "AppPayload_m.h"

/**
 * TODO - Generated class
 */
class AppAP_PSDRTest : public AppBase
{
  public:
    AppAP_PSDRTest();
    virtual ~AppAP_PSDRTest();

  protected:
    virtual void initialize(int stage);
    virtual void startApp();
    virtual void handleSelfMessage(cMessage* msg);
    virtual void handlePacket(cPacket *msg);
    virtual void generatePacket();

  private:
    bool isEnabled();
    void scheduleNextPacket(simtime_t previous);
    void initDestAddrs();
    virtual MACAddressExt* chooseDestAddr();

    simtime_t appStartTime;
    simtime_t appStopTime;
    int numPackets;
    cPar *sendIntervalPar;
    std::map<long, simtime_t> sendTimeMap;
    simtime_t sendTime;
    long requestID;
    enum Kinds {START=111, NEXT = 222};
    cMessage *timer;
    std::vector<MACAddressExt> destAddresses;

};

#endif
