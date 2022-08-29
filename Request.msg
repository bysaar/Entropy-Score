message Request extends cPacket
{
    string payload = "This is a Request";
    string ip_addr;				// Sender's IP address
    int type;					// Type of message - for statistic collection to distinguish between normal/ddos/flash-crowd
}
