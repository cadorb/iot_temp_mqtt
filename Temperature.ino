void setuptemp() {
    if (! tmp006.begin()) {
        Serial.println("No sensor found");
        while (1);
    }
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