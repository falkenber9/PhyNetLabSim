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

#ifndef __IEEE802154INET_STANDALONE_SUPERVISOR_H_
#define __IEEE802154INET_STANDALONE_SUPERVISOR_H_

#include <omnetpp.h>
#include <fstream>
#include <map>
#include <vector>
#include "BoxFeature.h"


/**
 * TODO - Generated class
 */
class Supervisor : public cSimpleModule
{
  private:
    std::fstream file;
    std::map<int,BoxFeature> featureMap;
    std::map<int,BoxFeature>::iterator it;
    std::vector<BoxFeature> featureList;


    int boxFeatureCnt;
    int attachCnt;
    Coord* maxCoord;
    Coord* minCoord;
    void parseCoordinates();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  public:
    virtual Coord getCoord(int index);
    virtual Coord getCenterCoord();
    virtual Coord getMaxCoord();


};

#endif
