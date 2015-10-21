// Based on this example:
// http://www.arduino.cc/en/Tutorial/LiquidCrystal


#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

IPAddress server(192,168,1,1);


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

String responseLine;
boolean capturingUpstream = false;
boolean capturingDownstream = false;
String upstream;
String downstream;


void setup() {
  Serial.begin(9600);
  Serial.println("Hello");

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /info.html HTTP/1.1");
    client.println("Host: 192.168.1.1");
    client.println("Authorization: Basic dXNlcjp1c2Vy");
    client.println("Connection: close");
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Hello");
  //delay(1000);
}


void loop() {

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    responseLine += c;

    // At this point we have a whole line
    if (c == '\n') {
      if (responseLine.indexOf(">Line Rate - Upstream") >= 0) {
        Serial.println("+ Found Upstream, capture next line");
        capturingUpstream = true;
      } else if (responseLine.indexOf(">Line Rate - Downstream") >= 0) {
        Serial.println("+ Found Downstream, capture next line");
        capturingDownstream = true;
      } else if (capturingUpstream) {
        upstream = responseLine;
        int i1 = upstream.indexOf('>') + 1;
        int i2 = upstream.indexOf('<', i1);
        upstream = upstream.substring(i1, i2);
        Serial.println("+ Upstream = " + upstream);
        lcd.setCursor(0, 0);
        lcd.print("Up:   " + upstream + "  Kbps");
        capturingUpstream = false;
      } else if (capturingDownstream) {
        downstream = responseLine;
        Serial.println("+ Downstream = '" + downstream + "'");
        int i1 = downstream.indexOf('>') + 1;
        int i2 = downstream.indexOf('<', i1);
        downstream = downstream.substring(i1, i2);
        Serial.println("+ Downstream = '" + downstream + "'");
        lcd.setCursor(0, 1);
        lcd.print("Down: " + downstream + " Kbps");
        capturingDownstream = false;
      }
      responseLine = "";
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }

  //lcd.setCursor(0, 0);
  //lcd.print("Up:   964  Kbps");
  //lcd.setCursor(0, 1);
  //lcd.print("Down: 5035 Kbps");
  
  //delay(10*1000);

  //lcd.setCursor(0, 0);
  //lcd.print("Refreshing...   ");
  //lcd.setCursor(0, 1);
  //lcd.print("                ");
  
  //delay(1000);
}

