// Based on this example:
// http://www.arduino.cc/en/Tutorial/LiquidCrystal


#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

#include "RouterClient.h"

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);

IPAddress routerAddress(192,168,1,1);
RouterClient routerClient;

String line;
boolean capturingUpstream = false;
boolean capturingDownstream = false;
String upstream;
String downstream;



String extractInnerHtml(String line) {
  int i1 = line.indexOf('>') + 1;
  int i2 = line.indexOf('<', i1);
  upstream = line.substring(i1, i2);
}


void setup() {
  // Set up serial connection (baud rate) and LCD (# of rows and cols)
  Serial.begin(9600);
  lcd.begin(16, 2);

  RouterClient routerClient(routerAddress, 80);
}


void loop() {
  if (routerClient.available()) {
    lcd.setCursor(0, 0);
    lcd.print("Fetching...     ");
    routerClient.get("/info.html");

  } else if (routerClient.responseAvailable()) {
    line = routerClient.readLine();

    if (line.indexOf(">Line Rate - Upstream") >= 0) {
      capturingUpstream = true;

    } else if (line.indexOf(">Line Rate - Downstream") >= 0) {
      capturingDownstream = true;

    } else if (capturingUpstream) {
      lcd.setCursor(0, 0);
      lcd.print("Up:   " + extractInnerHtml(line) + "  Kbps");
      capturingUpstream = false;

    } else if (capturingDownstream) {
      lcd.setCursor(0, 1);
      lcd.print("Down: " + extractInnerHtml(line) + " Kbps");
      capturingDownstream = false;
    }

  } else if (routerClient.responseComplete()) {
    delay(10000);
  }
}
