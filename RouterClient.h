#ifndef RouterClient_h
#define RouterClient_h

#include <SPI.h>
#include <Ethernet.h>

class RouterClient {
  public:
    RouterClient(IPAddress server, int port);
    bool get(String);
};

#endif
