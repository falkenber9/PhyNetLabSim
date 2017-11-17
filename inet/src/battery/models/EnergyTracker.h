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

#ifndef __INET_ENERGYTRACKER_H_
#define __INET_ENERGYTRACKER_H_

#include <omnetpp.h>
#include <vector>
#include <map>

#include "INETDefs.h"

#include "BasicBattery.h"

class INET_API EnergyTracker : public BasicBattery
{
  protected:
    class DeviceEntry
    {
      public:
        int currentState;
        cObject * owner;
        double radioUsageCurrent[4];
        double  draw;
        int     currentActivity;
        int     numAccts;
        double  *accts;
        simtime_t   *times;
        DeviceEntry()
        {
            currentState = 0;
            numAccts = 0;
            currentActivity = -1;
            accts = NULL;
            times = NULL;
            owner = NULL;
            for (int i=0; i<4; i++)
                radioUsageCurrent[i] = 0.0;
        }
        ~DeviceEntry()
        {
            delete [] accts;
            delete [] times;
        }
    };
    typedef std::map<int,DeviceEntry*>  DeviceEntryMap;
    typedef std::vector<DeviceEntry*>  DeviceEntryVector;
    DeviceEntryMap deviceEntryMap;
    DeviceEntryVector deviceEntryVector;

  public:
    virtual void    initialize(int);
    virtual int numInitStages() const {return 2;}
    virtual void    finish();
    virtual void handleMessage(cMessage *msg);
    /**
     * @brief Registers a power draining device with this battery.
     *
     * Takes the name of the device as well as a number of accounts
     * the devices draws power for (like rx, tx, idle for a radio device).
     *
     * Returns an ID by which the device can identify itself to the
     * battery.
     *
     * Has to be implemented by actual battery implementations.
     */
    virtual int registerDevice(cObject *id, int numAccts);
    virtual void registerWirelessDevice(int id, double mUsageRadioIdle, double mUsageRadioRecv, double mUsageRadioSend, double mUsageRadioSleep);

    /**
     * @brief Draws power from the battery.
     *
     * The actual amount and type of power drawn is defined by the passed
     * DrawAmount parameter. Can be an fixed single amount or an amount
     * drawn over time.
     * The drainID identifies the device which drains the power.
     * "Account" identifies the account the power is drawn from.
     */
    virtual void draw(int drainID, DrawAmount& amount, int account);
    ~EnergyTracker();
    EnergyTracker() {mustSubscribe = true; publish = NULL; timeout = NULL;}
    double getVoltage();
    /** @brief current state of charge of the battery, relative to its
     * rated nominal capacity [0..1]
     */
    double estimateResidualRelative();
    /** @brief current state of charge of the battery (mW-s) */
    double estimateResidualAbs();
    /** @brief amount og consumed energy of the battery (mW-s) */
    double getConsumedEnergy();

  protected:

    cOutVector residualVec;
    cOutVector* mCurrEnergy;

    enum msgType
    {
        AUTO_UPDATE, PUBLISH,
    };

    cMessage *publish;
    cMessage *timeout;
    simtime_t lastUpdateTime;


    void receiveChangeNotification(int aCategory, const cObject* aDetails);

  private:

    static simsignal_t totalEnergySignal;
    static simsignal_t powerConsumtionSignal;

  private:
    double totalConsumedEnergy;
  public:
    double getTotalConsumedEnergy(){return totalConsumedEnergy;}
    virtual void deductAndCheck();
};

#endif
