
const char* host = "finedustapi.10make.com";  
const int httpPort = 80;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
String data;
String contentType;

void do_server(String api_key,int pm25, int pm10,float temperature) {
  data = "api_key="+ String(api_key) + "&pm25=" + String(pm25) + "&pm10=" + String(pm10) + "&temp=" + String(temperature);
  contentType= "application/x-www-form-urlencoded";

  //서버 통신 공식 client.println을 써야한다.
  if(client.connect(host,httpPort)){
    Serial.println("connected");
    client.print("GET /insert.php?");
    client.print(data); 
    client.println(" HTTP/1.1");
    client.println("Host: " + String(host)); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/xE-www-form-urlencoded"); 
    client.print("Content-Length: "); 
    client.println(data.length()); 
    client.println("Connection: close");
    client.println(); 
    //client.print(data); 
  }
  
  //서버 통신이 되지 않으면
  else{
    Serial.println("connection failed: ");
    return;
  }
}
