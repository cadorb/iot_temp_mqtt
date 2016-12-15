/**
 * Created by Blaise Cador
 * Buttons controller
 * SETUP and VOID
 */

void initButtons(){
    pinMode(buttonPin, INPUT_PULLUP);
}

void listenButtons() {
    if (buttonState == HIGH) {
        deleteConf();
    }
}