/*
 * --------FINAL_PROJECT----------
 * Saar Ben-Yochana - 313234155
 * Efi Korenfeld - 208838359
 * -------------------------------
 */
#ifndef SERVER_H_
#define SERVER_H_
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Request_m.h"
#include <vector>
#include <algorithm>

// Classification
#define UNCLASSIFIED -1
#define NORMAL 0
#define DDOS 1
#define FC 2

// Group management
#define DISTANCE 29

using namespace omnetpp;
using namespace std;

class Server : public cSimpleModule
{
  private:
    // Struct to represent different groups and their statistics:
    struct group_info {
      int group_id;
      int group_type;                       // Pre-defined group type
      int classification;                   // Algorithm's classification for group type
      double message_count;
      double pxi;                           // Occurrence probability of the group
      double pyi;                           // Last recorded occurrence probability of the group (score book)
      double score;                         // Score value of the group
      string center_addr;                   // Current center of group address
      vector <string> group_addresses;      // All existing group addresses
    };

    vector <group_info> groups;             // Vector to store all groups
    map<string, pair<int,int>> addresses;   // Stores all addresses with count of messages and group id they belong to
    double total_messages;                  // Total number of messages received
    double thresh_packet_score;             // Threshold for packet score
    double thresh_entropy;                  // Threshold for entropy
    int num_groups;                         // Number of groups created
    double system_entropy;                  // Current calculated system entropy


  public:
    Server();
    virtual ~Server();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;


    /**
    *  @brief Searches in Groups table, if found matching groups (if address already in group or found new group by distance)
    *  then updates parameters - else creates new one, uses helper function: 'check_already_registered'
    */
    virtual bool searchGroup(string sender_ip);

    /**
    *  @brief Searches in address table, if address is already registered- returns group id, else- returns -1
    */
    virtual int check_already_registered(string ip);

    /**
    *  @brief Sorts addresses in group and picks the median as the new center of group
    */
    virtual void update_group_center(group_info &group);


    /**
    *  @brief Returns integer value of IP address last octet
    */
    virtual int last_octet(string ip);

    /**
    *  @brief Checks if first 3 octets of IP address are equal, if so - returns 1, else - returns 0
    */
    virtual int check_pattern(string address1,string address2);

    /**
    *  @brief Prints all addresses registered in system - with group id and number of messages received from each
    */
    virtual void print_addresses();

    /**
    *  @brief Prints groups information and classifications
    */
    virtual void print_stats();

    /**
    *  @brief Record group probabilities
    */
    virtual void record_probabilities(int phase);

    /**
    *  @brief Calculate current System entropy
    */
    virtual void calculate_entropy();

    /**
    *  @brief Classify system groups
    */
    virtual void classify_groups(string phase);

};



#endif /* SERVER_H_ */
