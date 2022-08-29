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
simple Server
{
    parameters:
        double thresh_entropy = default(2.42);			// Threshold for entropy calculation (2.32- max entropy for 5 groups, + 0.1 safety)
        double thresh_packet_score = default(15);		// Threshold for packet scoring
    gates:
        input server_client[];							// (Server Module) Gate <- (Client Modules) 
}



