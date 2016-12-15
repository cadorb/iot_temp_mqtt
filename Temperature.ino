/**
 * Created by Blaise Cador
 * setup and print Temperature
 */

void initTemp() {
    if (! tmp006.begin()) {
        Serial.println("No sensor found");
        while (1);
    }
}

float getTemp() {
    float objt = tmp006.readObjTempC();
    Serial.print("Distance: "); Serial.print(objt); Serial.println("*C");
    float diet = tmp006.readDieTempC();
    Serial.print("Ambiance: "); Serial.print(diet); Serial.println("*C");
    return diet;
}