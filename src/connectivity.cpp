#include "connectivity.h"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

ESP8266 wifi(&Serial1);

char auth[] = "0a764907c78042b497a89d6f2afbecab";
char ssid[] = "PLUSNET-7WTF";
char pass[] = "6c2347ce5a";
char ip[] =  "192.168.1.83";

void init_blynk()
{
    Serial1.begin(115200);

    const uint8_t ch_pd = 3;

    pinMode(ch_pd, OUTPUT);
    digitalWrite(ch_pd, LOW);
    delay(50);
    digitalWrite(ch_pd, HIGH);

    Blynk.begin(auth, wifi, ssid, pass, ip, 8080);
}

void update_blynk(uint32_t currentTimeUs)
{
    Blynk.run();
}

void sync_params(uint32_t currentTimeUs)
{

}