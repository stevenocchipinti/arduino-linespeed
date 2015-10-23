#include "routerClient.h"

RouterClient::RouterClient(IPAddress server, int port) {
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress defaultIp(192,168,1,177);
  EthernetClient client;

  Serial.print("Initializing... ");
  // If DHCP fails, try to manually set an IP
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, defaultIp);
  }
  Serial.println("done!");

  // Give the Ethernet shield a second to initialize
  delay(1000);
}

bool RouterClient::get(String url) {
  Serial.print("Fetching... ");
  if (client.connect(server, 80)) {
    Serial.println("done!");
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: 192.168.1.1");
    client.println("Authorization: Basic dXNlcjp1c2Vy");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("FAILED!");
  }
}





  /* // If the server's disconnected, stop the client */
  /* if (!routerClient.connected()) { */
  /*   routerClient.stop(); */
  /* } */


/*   // If there are incoming bytes available */
/*   // from the server, read them and print them */
/*   if (client.available()) { */
/*     char c = client.read(); */
/*     responseLine += c; */

/*     // At this point we have a whole line */
/*     if (c == '\n') { */

/*       if (responseLine.indexOf(">Line Rate - Upstream") >= 0) { */
/*         Serial.println("+ Found Upstream, capturing next line"); */
/*         capturingUpstream = true; */

/*       } else if (responseLine.indexOf(">Line Rate - Downstream") >= 0) { */
/*         Serial.println("+ Found Downstream, capturing next line"); */
/*         capturingDownstream = true; */

/*       } else if (capturingUpstream) { */
/*         upstream = responseLine; */
/*         int i1 = upstream.indexOf('>') + 1; */
/*         int i2 = upstream.indexOf('<', i1); */
/*         upstream = upstream.substring(i1, i2); */
/*         Serial.println("+ Upstream = " + upstream); */
/*         lcd.setCursor(0, 0); */
/*         lcd.print("Up:   " + upstream + "  Kbps"); */
/*         capturingUpstream = false; */

/*       } else if (capturingDownstream) { */
/*         downstream = responseLine; */
/*         Serial.println("+ Downstream = '" + downstream + "'"); */
/*         int i1 = downstream.indexOf('>') + 1; */
/*         int i2 = downstream.indexOf('<', i1); */
/*         downstream = downstream.substring(i1, i2); */
/*         Serial.println("+ Downstream = '" + downstream + "'"); */
/*         lcd.setCursor(0, 1); */
/*         lcd.print("Down: " + downstream + " Kbps"); */
/*         capturingDownstream = false; */
/*       } */

/*       responseLine = ""; */
/*     } */
/*   } */

/* } */

