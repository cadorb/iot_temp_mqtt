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


Adafruit_TMP006 tmp006(0x41);  // start with a diferent i2c address!


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

void setuptemp() {
  if (! tmp006.begin()) {
    Serial.println("No sensor found");
    while (1);
  }
}

void setupwebs() {
  delay(500);

  Serial.println("*** LaunchPad CC3200 WiFi Web-Server in AP Mode");

  // Start WiFi and create a network with wifi_name as the network name
  // with wifi_password as the password.
  Serial.print("Starting AP... ");
  WiFi.beginNetwork(wifi_name, wifi_password);
  while (WiFi.localIP() == INADDR_NONE)
  {
    // print dots while we wait for the AP config to complete
    Serial.print('.');
    delay(300);
  }
  Serial.println("DONE");

  Serial.print("LAN name = ");
  Serial.println(wifi_name);
  Serial.print("WPA password = ");
  Serial.println(wifi_password);

  IPAddress ip = WiFi.localIP();
  Serial.print("Webserver IP address = ");
  Serial.println(ip);

  Serial.print("Web-server port = ");
  myServer.begin();                           // start the web server on port 80
  Serial.println("80");
  Serial.println();
}

void temp() {
  listenButtons();
  float objt = tmp006.readObjTempC();
  Serial.print("Distance: "); Serial.print(objt); Serial.println("*C");
  float diet = tmp006.readDieTempC();
  Serial.print("Ambiance: "); Serial.print(diet); Serial.println("*C");
  if(objt >= 30 || diet >= 30){
    Serial.println("La temperature depasse les 30 degres");
    digitalWrite(RED_LED, HIGH);
  }
}

void apmode() {
  countClients = WiFi.getTotalDevices();

  // Did a client connect/disconnect since the last time we checked?
  if (countClients != oldCountClients)
  {
    if (countClients > oldCountClients)
    { // Client connect
      //            digitalWrite(RED_LED, !digitalRead(RED_LED));
      Serial.println("Client connected to AP");
      for (uint8_t k = 0; k < countClients; k++)
      {
        Serial.print("Client #");
        Serial.print(k);
        Serial.print(" at IP address = ");
        Serial.print(WiFi.deviceIpAddress(k));
        Serial.print(", MAC = ");
        Serial.println(WiFi.deviceMacAddress(k));
        Serial.println("CC3200 in AP mode only accepts one client.");
      }
    }
    else
    { // Client disconnect
      //            digitalWrite(RED_LED, !digitalRead(RED_LED));
      Serial.println("Client disconnected from AP.");
      Serial.println();
    }
    oldCountClients = countClients;
  }

  WiFiClient myClient = myServer.available();

  if (myClient)
  { // if you get a client,
    Serial.println(". Client connected to server");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    int8_t i = 0;
    while (myClient.connected())
    { // loop while the client's connected
      if (myClient.available())
      { // if there's bytes to read from the client,
        char c = myClient.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            myClient.println("HTTP/1.1 200 OK");
            myClient.println("Content-type:text/html");
            myClient.println();

            // the content of the HTTP response follows the header:
            myClient.println("<html><head><title>Energia CC3200 WiFi Web-Server in AP Mode</title></head><body align=center>");
            myClient.println("<h1 align=center><font color=\"red\">LaunchPad CC3200 WiFi Web-Server in AP Mode</font></h1>");
            myClient.println("<FORM METHOD=\"GET\" ACTION=\"\">");
            myClient.println("<INPUT type=\"text\" value=\"Houat\" name=\"ssid\">");
            myClient.println("<INPUT type=\"password\" value=\"Houat_WPA2!\" name=\"password\">");
            myClient.println("<INPUT type=\"submit\">");
            myClient.println("</FORM>");

            // The HTTP response ends with another blank line:
            myClient.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r')
        { // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        Serial.println();

        String text = buffer;

        // Check to see if the client request was "GET /H" or "GET /L":
        if (text.endsWith(" HTTP/1.1") && text.startsWith("GET /?ssid="))
        {
          text.replace(" HTTP/1.1", "");
          text.replace("GET /?ssid=", "");
          text.replace("password=", "");

          writeconf(text);
          restart();


        }
      }
    }

    // close the connection:
    myClient.stop();
    Serial.println(". Client disconnected from server");
    Serial.println();
  }
}

void stationmode(String wifiConf) {


  int index = wifiConf.indexOf("&");

  String station_ssid_str = wifiConf.substring(0, index);
  String station_password_str = wifiConf.substring(index + 1);

  station_password_str.replace("%21", "!");

  int size_station_ssid_str = sizeof(station_ssid_str) + 1;
  int size_station_password_str = sizeof(station_password_str) + 1 ;

  char station_ssid[size_station_ssid_str];
  char station_password[size_station_password_str];

  strcpy(station_ssid, station_ssid_str.c_str());
  strcpy(station_password, station_password_str.c_str());

  Serial.println(station_ssid);
  Serial.println(station_password);

  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(station_ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(station_ssid, station_password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");



  delay(4000);
}

void writeconf(String getString) {
  SerFlash.begin();  // This actually calls WiFi.init() btw
  Serial.println("Creating a conf file");


  int32_t retval = SerFlash.open(wifi_confFile, FS_MODE_OPEN_CREATE(512, _FS_FILE_OPEN_FLAG_COMMIT));

  Serial.println("############");
  Serial.print("getString");
  Serial.println(getString);
  Serial.print("retval");
  Serial.println(retval);

  if (retval == SL_FS_OK) {
    SerFlash.write(getString);  // Write text to the file-
    SerFlash.close();
  } else {
    // retval did not return SL_FS_OK, there must be an error!
    Serial.print("Error opening /storage/mine.txt, error code: ");
    Serial.println(SerFlash.lastErrorString());
    Serial.flush();  // flush pending serial output before entering suspend()
    while (1) ; // halting (suspend() isn't support by Energia MT at the moment FYI)
  }




}

int confExists() {
  int retval = SerFlash.open(wifi_confFile, FS_MODE_OPEN_READ);
  SerFlash.close();
  return retval;
}

String readConf() {
  String my_ssid;
  String my_password;

  // Open "/storage/mine.txt" and read its contents to the serial monitor.
  if (SerFlash.open(wifi_confFile, FS_MODE_OPEN_READ) != 0) {
    // Some sort of error occurred!
    Serial.print("ERROR opening confFile!  Error code: ");
    Serial.println(SerFlash.lastErrorString());
    Serial.println("Halting.");
    while (1) delay(1000);  // Infinite loop to stop the program
  }

  char buf[1024];  // our RAM buffer to house the file's contents

  size_t read_size = SerFlash.readBytes(buf, 1023);
  if (read_size >= 0) {
    buf[read_size] = '\0';  // NUL-terminate the buffer before treating it as a string!
    Serial.print("Read ");
    Serial.print(read_size);
    Serial.println(" bytes from /storage/mine.txt - displaying contents:");

    Serial.print(buf);

    SerFlash.close();
  } else {
    Serial.print("There was an error reading from the /storage/mine.txt file! Error code: ");
    Serial.println(SerFlash.lastErrorString());
  }

  String text = buf;

  return text;
}

void listenButtons() {
  if (buttonState == HIGH) {
    deleteConf();
  }
}

void deleteConf() {
  SerFlash.begin();
  int32_t retval = SerFlash.del(wifi_confFile);
  Serial.print("Deleting config.txt return code: ");
  Serial.println(SerFlash.lastErrorString());
}

void restart() {
  Serial.println("Restarting CC3200");

  UDMAInit();
  sl_Start(NULL, NULL, NULL);
  sl_WlanDisconnect();
  sl_NetAppMDNSUnRegisterService(0, 0);
  sl_WlanRxStatStart();
  sl_WlanSetMode(ROLE_STA);
  sl_Stop(0xFF);
  sl_Start(0, 0, 0);


  String confString = readConf();
  stationmode(confString);

  while (1) {
    listenButtons();
    temp();
  }

}





