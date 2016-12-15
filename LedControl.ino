/**
 * Created by Blaise Cador
 * Led Controller
 * SETUP
 */

void shutdownLeds(){
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
}

void initLeds(){
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
}

void blinkRedLed(){
    digitalWrite(RED_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(300);               // wait for a second
    digitalWrite(RED_LED, LOW);    // turn the LED off by making the voltage LOW
    delay(300);               // wait for a second
}