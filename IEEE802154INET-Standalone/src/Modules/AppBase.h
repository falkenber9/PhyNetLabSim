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

#ifndef __IEEE802154INET_STANDALONE_APPBASE_H_
#define __IEEE802154INET_STANDALONE_APPBASE_H_

#include <omnetpp.h>
#include <vector>
#include "mcpsData_m.h"
#include "MPDU_m.h"
#include "IEEE802154Fields.h"
#include "AppPayload_m.h"



#define appEV (ev.isDisabled()||!appDebug) ? EV : EV << "[App]: " // switchable debug output


class AppBase : public cSimpleModule
{
  public:
    AppBase();
    virtual ~AppBase();
    bool isAccessPoint() {return isAP;}


  protected:
    virtual int  numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    virtual void startApp();
    virtual void handlePacket(cPacket *msg);
    virtual void handleSelfMessage(cMessage* msg);

    virtual void generatePacket();
    virtual void printPacket(cPacket *msg);

    bool appDebug = false;

    bool isAP;
    cPar* packetLengthPar;
    int numProducts;
    int numHosts;
    /** @brief The 3 bits (b0, b1, b2) indicate the transmission options for this MSDU.
    *
    * For b0: 1 = acknowledged transmission, 0 = unacknowledged transmission.
    * For b1: 1 = GTS transmission, 0 = CAP transmission for a beacon-enabled PAN.
    * For b2: 1 = indirect transmission, 0 = direct transmission.
    * For a non-beacon-enabled PANs, bit b1 should always be set to 0.
    */
    unsigned char TXoption;
    unsigned char msgHandle;    // simple 8-bit counter to set msduHandle (and subsequently MAC DSN) for created packets

    int numReceived;
    int numSent;
    int numSentConf;
    static simsignal_t sentPkSignal;
    static simsignal_t sentPkConfSignal;
    static simsignal_t rcvdPkSignal;
    static simsignal_t delaySignal;


  private:
    void genAssoReq();
    void genPollReq();
    void genScanReq();
    void genOrphanResp(OrphanIndication* oi);

    static bool firstDevice; // first device that receives a "Scan confirm" without results shall announce that it is PAN-Coordinator
    unsigned short logicalChannel;
    simtime_t pollFreq;
    bool associateSuccess;
    unsigned char coordAddrMode;
    unsigned short coordPANId;
    MACAddressExt coordAddress;
    int scanChannels;
    double scanDuration;
    int scanPage;
    int scanType;
    StartRequest* startMsg; // MLME-START.request message
    cMessage* selfMsg;      // Message for start up on rng
    cMessage* pollTimer;    // Poll Timer
};

#endif
