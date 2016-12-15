#include <Wire.h>
#include "Adafruit_TMP006.h"
#include "Energia.h"
#include "string.h"
#include "stdio.h"
#ifndef __CC3200R1M1RGC__
#include <SPI.h>                // Do not include SPI for CC3200 LaunchPad
#endif
#include <WiFi.h>
#include <SLFS.h>
#include "utility/udma_if.h"
#include "utility/simplelink.h"

// Temp Sensor
Adafruit_TMP006 tmp006(0x41);

// Define variables and constants
char wifi_name[] = "blaiseAPMode";
char wifi_password[] = "launchpad";
char wifi_confFile[] = "wifiConf.txt";

WiFiServer myServer(80);
uint8_t oldCountClients = 0;
uint8_t countClients = 0;

const int buttonPin = PUSH2;
int buttonState = 0;


void setup() {

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  delay(2000);


  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("#### Mesure de temperature ####");
  delay(500);
  SerFlash.begin();  // This actually calls WiFi.init() btw
  int insetup_retval = SerFlash.open(wifi_confFile, FS_MODE_OPEN_READ);
  SerFlash.close();


  Serial.println("#######");
  Serial.println(insetup_retval);
  Serial.println("#######");

  if (insetup_retval != 0) {
    setupwebs();
  } else {
    String confString = readConf();
    setuptemp();
    digitalWrite(GREEN_LED, HIGH);
    stationmode(confString);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);

  int conf = confExists();

  Serial.println("#######");
  Serial.println(conf);
  Serial.println("#######");

  if (conf != 0) {
    while (1) {
      digitalWrite(RED_LED, HIGH);
      apmode();
    }
  } else {
    while (1) {
      listenButtons();
      digitalWrite(RED_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(300);               // wait for a second
      digitalWrite(RED_LED, LOW);    // turn the LED off by making the voltage LOW
      delay(300);               // wait for a second
      temp();
    }
  }



}





