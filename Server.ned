simple Server
{
    parameters:
        double thresh_entropy = default(2.42);			// Threshold for entropy calculation (2.32- max entropy for 5 groups, + 0.1 safety)
        double thresh_packet_score = default(15);		// Threshold for packet scoring
    gates:
        input server_client[];							// (Server Module) Gate <- (Client Modules) 
}



