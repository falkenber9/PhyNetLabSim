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

#ifndef __IEEE802154INET_STANDALONE_APPBOX_H_
#define __IEEE802154INET_STANDALONE_APPBOX_H_

#include <omnetpp.h>
#include "AppBase.h"
#include "mcpsData_m.h"
#include "MPDU_m.h"
#include "IEEE802154Fields.h"
#include "AppPayload_m.h"


class AppBox : public AppBase
{
  public:
    AppBox();
    virtual ~AppBox();

  protected:
    virtual void initialize(int stage);
    virtual void handlePacket(cPacket * msg);

    int myProductID;
    std::map<int, const char*> productColorMap;

    void initColorMap();
    int parseMsgName(const char* name);
    void generateResponse(long srcReqNr, MACAddressExt& srcAddr);

};

#endif
