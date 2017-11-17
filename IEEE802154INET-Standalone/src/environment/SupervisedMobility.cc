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

#include "SupervisedMobility.h"
#include "Supervisor.h"
#include "AppBase.h"

Define_Module(SupervisedMobility);

void SupervisedMobility::initialize(int stage)
{
    //TODO evtl überflüssig?? falls noch ein paar stellen von MobBase hier hin übernommen werden
    //MobilityBase::initialize(stage);

    cModule* target = this->getParentModule()->getSubmodule("application");
    AppBase* app = check_and_cast<AppBase*>(target);

    if (stage == 0)
    {
            constraintAreaMin.x = par("constraintAreaMinX");
            constraintAreaMin.y = par("constraintAreaMinY");
            constraintAreaMin.z = par("constraintAreaMinZ");
            constraintAreaMax.x = par("constraintAreaMaxX");
            constraintAreaMax.y = par("constraintAreaMaxY");
            constraintAreaMax.z = par("constraintAreaMaxZ");
            visualRepresentation = findVisualRepresentation();
            if (visualRepresentation) {
                const char *s = visualRepresentation->getDisplayString().getTagArg("p", 2);
                if (s && *s)
                    error("The coordinates of '%s' are invalid. Please remove automatic arrangement"
                          " (3rd argument of 'p' tag) from '@display' attribute.", visualRepresentation->getFullPath().c_str());
            }
//        hostModule = getParentModule();
//        int index = hostModule->getIndex();
//        EV << "SupervisedMobility is Part of a Module" << hostModule->getFullName() << "with index: " << index << endl;

    }
    else if (stage == 2)
    {
        if(!app->isAccessPoint()){
            initializePosition();
        }
        else if(app->isAccessPoint()){
            cModule* targetModule = getParentModule(); // host
            targetModule = targetModule->getParentModule();  // network
            targetModule = targetModule->getSubmodule("supervisor");
            Supervisor* target = check_and_cast<Supervisor*>(targetModule);
            Coord coord = target->getCenterCoord();
            lastPosition.x = coord.x;
            lastPosition.y = coord.y;
            lastPosition.z = coord.z;
            checkPosition();
            emitMobilityStateChangedSignal();
            updateVisualRepresentation();
            double x = target->getMaxCoord().x;
            double y = target->getMaxCoord().y;
            x = x+200;
            y = y+450;
            char buf[20];
            sprintf(buf, "%f", x);
            this->getParentModule()->getParentModule()->getDisplayString().setTagArg("bgb", 0, buf);
            sprintf(buf, "%f", y);
            this->getParentModule()->getParentModule()->getDisplayString().setTagArg("bgb", 1, buf);
        }
    }
//    else if (stage == 3)
//    {
//        if(app->isAccessPoint()){
//            cModule* targetModule = getParentModule(); // host
//            targetModule = targetModule->getParentModule();  // network
//            targetModule = targetModule->getSubmodule("supervisor");
//            Supervisor* target = check_and_cast<Supervisor*>(targetModule);
//            lastPosition.x = target->getCenterCoord().x;
//            lastPosition.y = target->getCenterCoord().y;
//            lastPosition.z = target->getCenterCoord().z;
//            checkPosition();
//            emitMobilityStateChangedSignal();
//            updateVisualRepresentation();
//        }
//    }
}

void SupervisedMobility::initializePosition()
{
    setInitialPosition();
    checkPosition();
    emitMobilityStateChangedSignal();
    updateVisualRepresentation();
}

void SupervisedMobility::setInitialPosition()
{
    cModule* targetModule = getParentModule(); // host
    targetModule = targetModule->getParentModule();  // network
    targetModule = targetModule->getSubmodule("supervisor");
    Supervisor* target = check_and_cast<Supervisor*>(targetModule);
    int index = getParentModule()->getIndex();
    Coord coord = target->getCoord(index);

    lastPosition.x = coord.x;
    lastPosition.y = coord.y;
    lastPosition.z = coord.z;
}

void SupervisedMobility::handleSelfMessage(cMessage *msg)
{
    ASSERT(false);
}
