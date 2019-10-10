#include <SoftwareSerial.h>
#include "RunningMedian.h"
 
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

char* ssid = "";         // 와이파이 이름
char* password = "";     // 와이파이 비밀번호
String api_key = "";     //APIKEY
#define PLAIVE_SERVER_ENABLE
//#define THINGSPEAK_SERVER_ENABLE

boolean wifi_ready;

SoftwareSerial dust(D1, D0, false, 256);//RX,TX Communication

void got_dust(int pm25, int pm10) {//formula for dust sensor just use!!
   pm25 /= 10;
   pm10 /= 10;
   pm25s.add(pm25);
   pm10s.add(pm10);
   do_oled(pm25, pm10);//print pm25,pm10 in oled
}

//서버에 보내기(send server)
void do_interval() {               
  if (wifi_ready){
#ifdef PLAIVE_SERVER_ENABLE
    do_server_plaive(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()),get_temperature());
#else
  #ifdef THINGSPEAK_SERVER_ENABLE
    do_server_thingspeak(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()),get_temperature());
  #else
    do_server_default(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()),get_temperature());
  #endif
#endif
  } 
                                        //wifi is ok 
}

unsigned long mark = 0;
boolean got_interval = false;

//초기 세팅  
void setup() {
  Serial.begin(115200);
  dust.begin(9600);
  setup_oled();//oled setting
  wifi_ready = connect_ap(ssid, password);             //wifi connection 유무
  
  if (!wifi_ready) nowifi_oled();//wifi no connection
  delay(5000);
  Serial.println("\nDust Sensor Box V1.0, 2019/1/4 MAGICECO");
}

//아두이노가 반복적으로 작동하는 부분
void loop() {
  while (dust.available() > 0) {
    do_dust(dust.read(), got_dust);
    yield();                                          //loop에서 while문 쓸때는 yield써주어야 합니다.
  }
  if (millis() > mark) {//one minute(60000) interval
    mark = millis() + 60000;
    got_interval = true;
  }
  
  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  yield();
}
