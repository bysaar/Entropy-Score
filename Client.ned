simple Client
{
    parameters:
        //  Default parameters are for Normal client:
        int client_type = default(0);			
        int num_requests = default(8);			
        string ip_addr = default("192.168.1.");
    gates:
        output client_server;					// (Client Module) -> (Server Module) Gate
}
