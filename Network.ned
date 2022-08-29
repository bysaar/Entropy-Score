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
network Network
{
    parameters:
        int num_normal_clients = default(50);					// (int) Number of normal clients in the network
        int num_ddos_clients = default(15);						// (int) Number of DDOS clients in the network
        int num_flash_crowd_clients = default(40);				// (int) Number of FC clients in the network

        @display("bgb=1860,1102");
    submodules:
        norm[num_normal_clients]: Client {						// Create Normal clients
            @display("p=134,76,m,10,120,80;i=device/pc");
        }
        ddos[num_ddos_clients]: Client {						// Create DDoS Attackers
            @display("p=1364,496,ri,170,170;i=old/telnet");
        }
        fc[num_flash_crowd_clients]: Client {					// Create Flash Crowd clients
            @display("p=134,837,m,10,120,70;i=device/lan");
        }
        server: Server {										// Create Server
            @display("p=942,507;i=device/server");
        }
    connections:
        // Connect all clients to server:
        for i=0..num_normal_clients-1 {
            norm[i].client_server --> server.server_client++;
        }
        for i=0..num_ddos_clients-1 {
            ddos[i].client_server --> server.server_client++;
        }
        for i=0..num_flash_crowd_clients-1 {
            fc[i].client_server --> server.server_client++;
        }
}
