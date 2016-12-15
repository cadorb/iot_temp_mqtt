/**
 * Created by Blaise Cador
 * System functions
 * VOID
 */

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

}