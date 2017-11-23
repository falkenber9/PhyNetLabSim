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

#ifndef __IEEE802154INET_STANDALONE_SUPERVISEDMOBILITY_H_
#define __IEEE802154INET_STANDALONE_SUPERVISEDMOBILITY_H_

#include <omnetpp.h>
#include "INETDefs.h"
#include "MobilityBase.h"


class INET_API SupervisedMobility : public MobilityBase
{
  private:
    cModule *hostModule;    // the host that contains this radio model
  protected:
    /** @brief Returns the required number of initialize stages. */
    virtual int numInitStages() const { return 4; }

    /** @brief Initializes mobility model parameters. */
    virtual void initialize(int stage);

    void initializePosition();

    void setInitialPosition();

    /** @brief Never happens. */
    virtual void handleSelfMessage(cMessage *message);

  public:
    /** @brief Returns the current position at the current simulation time. */
    virtual Coord getCurrentPosition() { return lastPosition; }

    /** @brief Returns the current speed at the current simulation time. */
    virtual Coord getCurrentSpeed() { return Coord::ZERO; }
};

#endif
