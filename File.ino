/**
 * Created by Blaise Cador
 * Read Write Delete Configuration File
 */

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

void deleteConf() {
    SerFlash.begin();
    int32_t retval = SerFlash.del(wifi_confFile);
    Serial.print("Deleting config.txt return code: ");
    Serial.println(SerFlash.lastErrorString());
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