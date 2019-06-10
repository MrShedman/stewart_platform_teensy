#include "connectivity.h"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "pattern_manager.h"
#include "controller.h"

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

void update_blynk(const Time& currentTime)
{
    Blynk.run();
}

BLYNK_WRITE(V0)
{
    int32_t x = param[0].asInt();
    int32_t y = param[1].asInt();

    set_target_pos(Vec3(x, y, 0.0));
}

BLYNK_WRITE(V1)
{
    set_pattern((PatternList)param.asInt());
}

BLYNK_WRITE(V2)
{
    set_speed(param.asInt());
}

BLYNK_WRITE(V3)
{
    reset_errors();
}

void sync_params(const Time& currentTime)
{
    const char* pattern_name = get_pattern_name(get_pattern());

    Blynk.virtualWrite(V1, (int)get_pattern()); 
    Blynk.virtualWrite(V2, (int)get_speed());
    Blynk.virtualWrite(V3, has_errors());
    Blynk.virtualWrite(V4, pattern_name);  
}