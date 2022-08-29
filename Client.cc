/*
 * --------FINAL_PROJECT----------
 * Saar Ben-Yochana - 313234155
 * Efi Korenfeld - 208838359
 * -------------------------------
 */
#include "Client.h"
using namespace std;

Define_Module(Client);
vector <int> Client::addresses;     // Initialize static class member

Client::Client()
{
    generate = nullptr;
}

Client::~Client() {}

void Client::initialize()
{
    // Retrieve parameters from ini file:
    type = par("client_type");
    num_requests = par("num_requests");
    self_ip = par("ip_addr").stdstringValue();

    // Initialize parameters:
    index = getIndex();
    msg_count = 0;
    generate = new cMessage("generate");

    switch(type) {
    // Calling the appropriate initialization client function based on type:
        case NORMAL:
            initialize_Normal();
        break;

        case DDOS:
            initialize_DDOS();
        break;

        case FC:
            initialize_FC();
        break;
    }

}

void Client::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()){
        // Create and send new request:
        string request_name = "Request type: " + to_string(type);
        Request *req = new Request(request_name.c_str());
        req->setIp_addr(self_ip.c_str());
        req->setType(type);
        send(req,"client_server");
        msg_count++;
        if (msg_count < num_requests){
            scheduleAt(simTime() + exponential(5), generate);
        }
        else{
            delete(generate);           // Sent all messages
        }

    }

}

void Client::initialize_Normal(){
    int n;
    // Generate random address for last octet:
    do {
        n=intuniform(0,200);
    } while(find(addresses.begin(), addresses.end(), n) != addresses.end());
    addresses.push_back(n);
    self_ip += to_string(n);
    // Schedule first message:
    scheduleAt(0.0 + exponential(5) , generate);

    // Clear the static parameter (for further random generating of addresses)
    if (index == 49)addresses.clear();

}

void Client::initialize_DDOS(){
    int n;
    // Generate 3'rd octet for DDOS clients:
    int octet = intuniform(5,7);
    self_ip += (to_string(octet) + ".");

    // Generate random address for last octet:
    do {
       n=intuniform(0,25);
    } while(find(addresses.begin(), addresses.end(), n) != addresses.end());
    addresses.push_back(n);
    self_ip += to_string(n);

    // Schedule first message:
    scheduleAt(100 + exponential(1) , generate);
}

void Client::initialize_FC(){

    // Generate random address for 3 last octets:
    self_ip = self_ip + to_string(intuniform(2,255)) + "." + to_string(50*intuniform(0,4)+index) + "." + to_string(intuniform(2,255));

    // Schedule first message:
    scheduleAt(1000 + exponential(1) , generate);
}

