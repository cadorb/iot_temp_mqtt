/**
 * Created by Blaise Cador
 * Configuration station mode
 * SETUP
 */

void initStationMode(String wifiConf) {

    // get indexof & symbol in wifi config string from wifi config file
    int index = wifiConf.indexOf("&");

    // Split string and attribute values to ssid and password strings
    String station_ssid_str = wifiConf.substring(0, index);
    String station_password_str = wifiConf.substring(index + 1);
    station_password_str.replace("%21", "!");

    // get size of ssid and password strings
    int size_station_ssid_str = sizeof(station_ssid_str) + 1;
    int size_station_password_str = sizeof(station_password_str) + 1 ;

    // initialize char arrays for ssid and password
    char station_ssid[size_station_ssid_str];
    char station_password[size_station_password_str];

    // put string values of ssid and password into char arrays
    strcpy(station_ssid, station_ssid_str.c_str());
    strcpy(station_password, station_password_str.c_str());

    // print ssid and password
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
}

void setStationMode(){
    getTemp();
}