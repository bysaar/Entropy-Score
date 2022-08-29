/*
 * --------FINAL_PROJECT----------
 * Saar Ben-Yochana - 313234155
 * Efi Korenfeld - 208838359
 * -------------------------------
 */
#include "Server.h"
using namespace std;
Define_Module(Server);

Server::Server()
{
}

Server::~Server()
{
}

void Server::initialize()
{
    // Retrieve Threshold parameters from ini file:
    thresh_packet_score = par("thresh_packet_score");
    thresh_entropy = par("thresh_entropy");
    // Initialize parameters:
    num_groups = 0;
    total_messages = 0;
    system_entropy = 0;
}

void Server::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()){ // For debugging
    }
    else{
        // New request received from client:
        Request* req_rcvd = check_and_cast<Request *>(msg);
        string sender_ip = req_rcvd->getIp_addr();
        total_messages++;
        bool found = searchGroup(sender_ip);
        if (!found){    // If not found matching group - create a new one
            EV << "Creating New group: "<< num_groups <<endl;
            group_info new_group;
            new_group.center_addr = sender_ip;
            new_group.message_count = 1;
            new_group.pxi = 0.01;
            new_group.pyi = 0.01;
            new_group.group_addresses.push_back(sender_ip);
            new_group.group_id = num_groups;
            new_group.group_type = req_rcvd->getType();
            new_group.score = 0;
            new_group.classification = UNCLASSIFIED;
            groups.push_back(new_group);
            addresses[sender_ip] = pair<int,int>(1,num_groups);
            num_groups++;
        }
        calculate_entropy();
        if(total_messages == 400){ // End of Phase 1
            // Set the entropy threshold according to number of normal groups created:
            thresh_entropy = log2(num_groups) + 0.0001; // 0.0001 is safety distance (delta)
            EV <<"Entropy Threshold set as: "<< thresh_entropy << endl;
            classify_groups(" phase NORMAL");
        }
        if(total_messages == 1210){ // End of Phase 2
            classify_groups(" phase DDOS");
        }
        if(total_messages == 2010){ // End of Phase 3
            classify_groups(" phase FC");
        }
        delete(msg);
    }

}
bool Server::searchGroup(string sender_ip){
    // Check if address is already registered in a group
    int group_id = check_already_registered(sender_ip);

    if (group_id == -1){   // Address is not in any existing group
        int octet_val = last_octet(sender_ip);
        // Iterate through all existing groups and try to find a matching one (by distance from center):
        for (auto it = begin (groups); it != end (groups); ++it) {
            if(!check_pattern(it->center_addr,sender_ip)) continue;               // If first 3 octets are different - group is not matching
            if(abs(last_octet(it->center_addr)-octet_val) < DISTANCE){            // Check distance from center of group
                EV << "Found matching group by distance, adding to group: "<<it->group_id <<endl;
                it->message_count++;                                              // Update number of messages in group
                if(it->group_type == NORMAL){
                    // Update pyi and pxi, and calculate score for normal groups:
                    it->pyi=it->pxi;
                    it->pxi = it->message_count/total_messages;
                    it->score = it->pxi/it->pyi;
                    EV<< "New score for group "<<it->group_id<< " is: "<<it->score<<endl;
                }
                else{
                    // Update pxi
                    it->pxi = it->message_count/total_messages;
                }

                it->group_addresses.push_back(sender_ip);                   // Add address to group addresses
                addresses[sender_ip] = pair<int,int>(1,it->group_id);       // Add address to global addresses table
                update_group_center(*it);                                   // Update group center
                return true;
            }
        }
        return false;  // No matching group
    }
    // Address is already in group:
    else{
        for (auto iter = begin (groups); iter != end (groups); ++iter) {
            if(iter->group_id == group_id){     // Find the corresponding group and update the parameters:
                iter->message_count++;
                if(iter->group_type == NORMAL){
                    // Update pyi and pxi, and calculate score for normal groups:
                    iter->pyi=iter->pxi;
                    iter->pxi = iter->message_count/total_messages;
                    iter->score = iter->pxi/iter->pyi;
                    EV<< "New score for group :"<<iter->group_id<< " is: "<<iter->score<<endl;
                }
                else{
                    // Update pxi
                    iter->pxi = iter->message_count/total_messages;
                }
                return true;
            }
        }
    }
    EV << "DEBUG ERROR"<<endl;
    return false; // should not get here
}


void Server::update_group_center(group_info &group){
    // Sort address vector:
    std::sort(group.group_addresses.begin(), group.group_addresses.end(), [](string a, string b) {
            // Lambda function to sort addresses in increasing order:
            int temp = a.find_last_of('.') + 1;
            string octet1  = a.substr(temp);
            int octet1_val = stoi(octet1);
            temp = b.find_last_of('.') + 1;
            string octet2  = b.substr(temp);
            int octet2_val = stoi(octet2);
            return octet1_val < octet2_val;
    });
    int size = group.group_addresses.size();
    int median = size / 2;                                               // Find index of median of sorted address group
    group.center_addr = group.group_addresses[median];                   // Set median as new group center
    EV << "Group "<< group.group_id << " new center address is: "<< group.center_addr <<endl;

}


int Server::check_pattern(string address1,string address2){

    int temp = address1.find_last_of('.');
    string check1  = address1.substr(0,temp);   // First 3 octets of address1

    temp = address2.find_last_of('.');
    string check2  = address2.substr(0,temp);   // First 3 octets of address2

    return !check1.compare(check2);             // If equal - return 1 else 0
}


int Server::last_octet(string ip){
    int temp = ip.find_last_of('.')+1;
    string octet  = ip.substr(temp);
    int octet_val = stoi(octet);
    return octet_val;                           // Return int value of last octet
}

int Server::check_already_registered(string ip){
   EV<< "---------------------------------------------------------------"<<endl;
   EV << "Received message from: "<< ip << endl;
   EV<<"Searching if address is already registered..."<<endl;
   map<string, pair<int,int>>::iterator itr;
   for (itr = addresses.begin(); itr != addresses.end(); ++itr) {
       if(strcmp(ip.c_str(),itr->first.c_str())==0){
           EV<< "Found - address is in group: " << itr->second.second <<endl;
           itr->second.first++;
           EV<< "---------------------------------------------------------------"<<endl;
           return itr->second.second;
       }
   }
   // Address not found in table
   EV<< "Address not registered"<<endl;
   return -1;

   EV<< "---------------------------------------------------------------"<<endl;

}

void Server::print_addresses(){
    EV<<"---------------------------------------------------------------"<<endl;
    EV<< " Addresses table: "<<endl;
    map<string, pair<int,int>>::iterator itr;
    for (itr = addresses.begin(); itr != addresses.end(); ++itr) {
        EV<<"Address: "<< itr->first <<" Number of messages: " <<itr->second.first  << " Group id: "<<itr->second.second<<endl;
    }
    EV<< "---------------------------------------------------------------"<<endl;
}

void Server::record_probabilities(int phase){
    string name;
    for (auto it = begin (groups); it != end (groups); ++it) {
        name = "Phase " + to_string(phase) + ", Probability group " + to_string(it->group_id) + " type: " + to_string(it->group_type);
        recordScalar(name.c_str(), it->pxi);
    }
}

void Server::calculate_entropy(){
    int normal_groups = 0;
    int fc_groups = 0;
    int ddos_groups = 0;
    system_entropy = 0;

    // Iterate through all groups and calculate entropy:
    for (auto it = begin (groups); it != end (groups); ++it) {

        it->pxi = it->message_count/total_messages;
        system_entropy -= it->pxi * log2(it->pxi);

        // for Debugging:
        switch (it->group_type){
            case NORMAL:
                normal_groups++;
                break;

            case DDOS:
                ddos_groups++;
                break;

            case FC:
                fc_groups++;
                break;
        }

    }
    EV<< "---------------------------------------------------------------"<<endl;
    EV << "Total entropy of system: "<< system_entropy << endl;
    EV<< "Normal groups: " << normal_groups << endl;
    EV<< "DDOS groups: " << ddos_groups << endl;
    EV<< "FC groups: " << fc_groups << endl;
    EV <<"Total messages: " << total_messages << endl;
    EV<< "---------------------------------------------------------------"<<endl;
}

void Server::print_stats(){
    string classification;
    for (auto iter = begin (groups); iter != end (groups); ++iter) {

        EV<< "---------------------------------------------------------------"<<endl;
        EV << "Group " << iter->group_id <<": "<<endl;
        switch (iter->classification){
            case NORMAL:
                classification= "Normal traffic";
                break;

            case DDOS:
                classification= "DDoS Attack";
                break;

            case FC:
                classification= "Flash Crowd traffic";
                break;
        }
        EV << "Group classification: "<< classification << endl;
        EV << "pxi: "<< iter->pxi <<endl;
        EV << "Group Score: "<< iter->score <<endl;
        EV << "Center Address: "<< iter->center_addr <<endl;
        EV << "Group messages: "<< iter->message_count <<endl;
        EV << "Group addresses: " <<endl;
        for (auto i: iter->group_addresses){
            EV << i << ", ";
        }
        EV<<endl<<"---------------------------------------------------------------"<<endl;
    }

}

void Server::classify_groups(string phase){
    string probability;
    string entropy = "System entropy" + phase;
    string score;
    recordScalar(entropy.c_str(),system_entropy);
    EV << "---------------------------------------------------------------" << endl;
    for (auto iter = begin (groups); iter != end (groups); ++iter) {
        probability = "probability group #" + to_string(iter->group_id) + phase;
        recordScalar(probability.c_str(), iter->pxi);

        if (iter->classification == UNCLASSIFIED){
            // Calculate score and classify group:
            iter->score = iter->pxi/iter->pyi;
            if(system_entropy < thresh_entropy){
                iter->classification = NORMAL;
            }
            else{ // entropy is higher than threshold, DDoS or FC:
                if(iter->score < thresh_packet_score){
                    iter->classification = FC;
                }
                else{
                    iter->classification = DDOS;
                }
            }

            EV << "Group " << iter->group_id <<": "<<endl;
            EV << "Group Score " << iter->score << endl;
            EV << "Group classification: "<< iter->classification << endl << endl;

            score = "Score group #" + to_string(iter->group_id) + phase;
            recordScalar(score.c_str(), iter->score);

        }
    }
    EV << "---------------------------------------------------------------" << endl;
}


void Server::finish(){
    calculate_entropy();
    print_stats();
}
