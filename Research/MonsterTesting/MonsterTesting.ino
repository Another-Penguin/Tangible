#include "Adafruit_Thermal.h"
#include "adalogo.h"

#include "SoftwareSerial.h"
#define TX_PIN 6
#define RX_PIN 5

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);


void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  printer.begin();

  printer.wake();

  printer.underlineOn();
  printer.println(F("Holy Schmoly"));
  printer.underlineOff();

  delay(2000);
  printer.printBitmap(adalogo_width, adalogo_height, adalogo_data);
  printer.feed(2);

  printer.setDefault();
}

void loop() {
  // put your main code here, to run repeatedly:

}
