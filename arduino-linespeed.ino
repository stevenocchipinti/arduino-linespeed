#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
EthernetClient client;
IPAddress routerAddress(192,168,1,1);

String responseLine;
boolean capturingUpstream = false;
boolean capturingDownstream = false;
String upstream;
String downstream;
int buttonValue;




void setup() {
  // Set up serial connection (baud rate) and LCD (# of rows and cols)
  Serial.begin(9600);
  lcd.begin(16, 2);

  screenPrint(0, "Initializing...");
  // If DHCP fails, try to set the IP manually
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, ip);
  }

  // Give the Ethernet shield a second to initialize
  delay(1000);
  refreshData();
}


void loop() {
  // If there are incoming bytes available
  if (client.available()) {
    char c = client.read();
    responseLine += c;

    // At this point we have a whole line
    if (c == '\n') {
      if (responseLine.indexOf(">Line Rate - Upstream") >= 0) {
        capturingUpstream = true;

      } else if (responseLine.indexOf(">Line Rate - Downstream") >= 0) {
        capturingDownstream = true;

      } else if (capturingUpstream) {
        screenPrint(0, "Up:   " + extractInnerHtml(responseLine) + " Kbps");
        capturingUpstream = false;

      } else if (capturingDownstream) {
        screenPrint(1, "Down: " + extractInnerHtml(responseLine) + " Kbps");
        capturingDownstream = false;
      }

      responseLine = "";
    }
  }

  // If the server's disconnected, stop the client
  if (!client.connected()) {
    client.stop();
    waitForSelectButton();
    refreshData();
  }
}



// Helper functions

void screenClear() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void screenPrint(int line, String text) {
  lcd.setCursor(0, line);
  lcd.print("                ");
  lcd.setCursor(0, line);
  lcd.print(text);
}

void refreshData() {
  screenClear();
  screenPrint(0, "Refreshing...");
  if (client.connect(routerAddress, 80)) {
    client.println("GET /info.html HTTP/1.1");
    client.println("Host: 192.168.1.1");
    client.println("Authorization: Basic dXNlcjp1c2Vy");
    client.println("Connection: close");
    client.println();
  }
}

String extractInnerHtml(String line) {
  int i1 = line.indexOf('>') + 1;
  int i2 = line.indexOf('<', i1);
  return line.substring(i1, i2);
}

void waitForSelectButton() {
  while (true) {
    buttonValue = analogRead(0);
    if (buttonValue > 555 && buttonValue < 790) {
      break;
    }
  }
}

