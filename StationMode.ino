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