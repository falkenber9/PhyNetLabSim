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

#ifndef __IEEE802154INET_STANDALONE_APPBOX_RESPONSERATE_H_
#define __IEEE802154INET_STANDALONE_APPBOX_RESPONSERATE_H_

#include <omnetpp.h>
#include "AppBox.h"
#include "mcpsData_m.h"
#include "MPDU_m.h"
#include "IEEE802154Fields.h"
#include "AppPayload_m.h"
#include "AppAP_ResponseRate.h"


class AppBox_ResponseRate : public AppBox
{
  public:
    AppBox_ResponseRate();
    virtual ~AppBox_ResponseRate();

  protected:
    virtual void initialize(int stage);
    virtual void handlePacket(cPacket * msg);

  private:
    AppAP_ResponseRate* ap;
};

#endif
