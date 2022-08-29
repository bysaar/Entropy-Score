/*
 * --------FINAL_PROJECT----------
 * Saar Ben-Yochana - *********
 * Efi Korenfeld - *********
 * -------------------------------
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Request_m.h"
#include <vector>
#include <algorithm>

// Client types:
#define NORMAL 0
#define DDOS 1
#define FC 2

using namespace omnetpp;
using namespace std;

class Client : public cSimpleModule
{
  private:
    // Client fields:
    cMessage *generate;
    string self_ip;
    int index;
    int type;
    int msg_count;
    int num_requests;

  public:
    static vector<int> addresses;   // Static class member - for generating distinct random addresses
    Client();
    virtual ~Client();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    /**
    *  @brief Initializes Normal client parameters- address, number of requests and schedules first message
    */
    virtual void initialize_Normal();
    /**
    *  @brief Initializes DDOS client parameters- address, number of requests and schedules first message
    */
    virtual void initialize_DDOS();
    /**
    *  @brief Initializes FC client parameters- address, number of requests and schedules first message
    */
    virtual void initialize_FC();
};


#endif /* CLIENT_H_ */
