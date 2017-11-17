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

package inet.battery.models;

import inet.battery.IBattery;

simple EnergyTracker like IBattery
{
    parameters:
        double nominal= default(3800);//mAh
        double capacity = default(3800);//mAh
        double voltage = default(12); // 12 volts
        double resolution @unit(s) = default(1s);
        double publishDelta = default(1); // between 0..1
        double publishTime @unit(s) = default(1s);
        bool ConsumedVector = default(false);
        
        @signal[totalEnergy](type = double);
        @signal[powerConsumtion](type = double);
        @statistic[totalEnergy](title="total energy consumed"; source="totalEnergy"; record=max,histogram; unit=mWh);
        @statistic[powerConsumtion](title="power"; source="powerConsumtion"; record=min,max,timeavg,vector; interpolationmode=sample-hold; unit=mW);
}
