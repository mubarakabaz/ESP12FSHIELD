/*\
 * Untuk menggunakan Lib ESP12FShield ini, terlebih dahulu tambahkan/install Lib WiFiEsp.h
 * Download lib WiFiEsp di https://github.com/bportaluri/WiFiEsp
 */

#include<ESP12FSHIELD.h>

char* ssid = "NAMA_SSID_WIFI";
char* pass = "PASSWORD_WIFI";

void setup(){
    Serial.begin(9600);

    setWifi(ssid, pass);
}

void loop(){
    //
}