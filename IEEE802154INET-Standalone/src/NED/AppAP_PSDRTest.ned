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

package ieee802154.src.NED;

simple AppAP_PSDRTest extends AppBase
{
	@class(AppAP_PSDRTest);
	
	double appStartTime @unit("s") = default(1s); // time of sending the first packet
    double appStopTime @unit("s") = default(-1s);  // time of finishing sending, negative values mean forever
	volatile double sendInterval @unit("s") = default(10ms); // may be a random value, e.g. exponential(1)
	int numPackets = default(-1); // max number of packets to generate, -1 means forever
	string destAddresses = default(""); // list of destination addresses, separated by spaces
	
	@signal[delay](type=simtime_t);
	@statistic[delay](title="ApToApDelay"; source="delay"; unit=s; record=stats,histogram,vector);
}
