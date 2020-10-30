#include <M5StickC.h>
#include <WiFi.h>
#include <ThingSpeak.h>

//#define __OFF //WiFi OFF
//#define __IDLE //WiFi idle

#define MEAS_DELAY     100
#define REQUEST_DELAY  20000  

const char* ssid = "Fyzika-je-lahka";
const char* pass = "12serenada12";

const unsigned long ch_id = 1210286;
const char* api_key = "LLIJ1EDFAQDT5OSF";

const float Temp = 9.81;

WiFiClient client;

void WiFiLoop(void * parameter){
    
    vTaskDelay(5000);

#ifdef __OFF
    WiFi.mode(WIFI_OFF);
    while(true){
        vTaskDelay(100);
    }
#endif
    
    WiFiClient client;
    ThingSpeak.begin(client);
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED){
        vTaskDelay(100);
    }
    vTaskDelay(5000);

#ifdef __IDLE
    while(true){
        vTaskDelay(100);
    }
#endif

    while(true){
        ThingSpeak.setField(1, Temp);    
        int ret = ThingSpeak.writeFields(ch_id, api_key);
        vTaskDelay(REQUEST_DELAY);
    }

    vTaskDelete(NULL);
}

void AXPloop(void * parameter){
    M5.begin();
    //M5.Axp.ScreenBreath(0);

    for(uint16_t sizmaj = 0; sizmaj < 600; sizmaj++){
        Serial.printf("%ld, %.2f\n", millis(), M5.Axp.GetVBusCurrent());
        vTaskDelay(100);
    }

    vTaskDelete(NULL);
}

void setup(){
    Serial.begin(115200);
    btStop();
    //setCpuFrequencyMhz(80); //config esp core freq = 80MHz
    
    xTaskCreate(AXPloop, "AXPloop", 10000, NULL, 1, NULL);
    xTaskCreate(WiFiLoop, "WiFiLoop", 20000, NULL, 1, NULL);
}

void loop(){

}
