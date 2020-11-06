#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include <PubSubClient.h>
int LED_PIN1 = D5;//blue
int LED_PIN2 = D6;//green
int LED_PIN3 = D7;//red 
int speakerPin = D8;//Chân được nối với loa hoặc buzzer

StaticJsonDocument<300> doc;

int dem = 1000;
unsigned long time_now1 =0;
unsigned long time_now2 =0;

const char* ssid = "Phenikaa 167 HN";     // your network SSID (name):QUOCDAT:Pham Quoc Tuan MTA:Phenikaa 167 HN
const char* pass = "phenikaa2018"; // your network password::66668888:tuan151299:phenikaa2018

class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }
    
    virtual bool onAuth(String username, String password) {
      Serial.println("Username/Password: "+username+"/"+password);
      return true;
    }
    
    virtual void onData(String control1, const char *data, uint32_t length) {
     char data_str[length+1];
     os_memcpy(data_str, data, length);
     data_str[length] = '\0';  
     DeserializationError error  = deserializeJson(doc, data);//data = json = {led:1,speaker:1}
     if (error)
     {
         Serial.println(" deserrialzeJon()failed1 ");

     }
     int led = doc["led"]; 
     int speaker = doc["speaker"]; 

     if(led==1 && speaker==1)
     {
      tone(speakerPin,2000,200);
        Serial.println(" led = 1 , speaker = 1 " );
        time_now1 =millis();
        digitalWrite(LED_PIN1, HIGH);
        while(millis() < time_now1 + dem)
        {}
        digitalWrite(LED_PIN1, LOW);
        
     }
          if(led==2 && speaker==2)
     {
      tone(speakerPin,100,1000);
        Serial.println(" led = 2 , speaker = 2 " );
        time_now2 =millis();
        digitalWrite(LED_PIN3, HIGH);
        while(millis() < time_now1 + dem)
        {}
        digitalWrite(LED_PIN3, LOW);
        
     }
     else{}
      Serial.println("received topic '"+control1+"' with data '"+(String)data_str+"'");
     }

//    virtual void onData(String control2, const char *data, uint32_t length) {
//     char data_str[length+1];
//     os_memcpy(data_str, data, length);
//     data_str[length] = '\0';  
//     DeserializationError error2  = deserializeJson(doc, data);//data = json = {led:1,speaker:1}
//     if (error2)
//     {
//         Serial.println(" deserrialzeJon()failed2 ");
//
//     }
//     int led = doc["led"]; 
//     int speaker = doc["speaker"]; 
//
//     if(led==3)
//     {
//        Serial.println(" led_blue HIGH (led=3)");
//        digitalWrite(LED_PIN1, HIGH);
//     }
//     
//     if(led==4 )
//     {
//        Serial.println(" led_blue LOW (led=4)");
//        digitalWrite(LED_PIN1, LOW);    
//     }
//     if(speaker==3)
//     {
//      Serial.println("speaker true (3)");
//      tone(speakerPin,2000,200);
//     }
//     if(speaker==4)
//     {
//      Serial.println("speaker true (4)");
//      tone(speakerPin,100,1000);
//     }
//     else
//     {
//     }
//     
//
//      Serial.println("received topic '"+control2+"' with data '"+(String)data_str+"'");
//     }
    
};

myMQTTBroker myBroker;
WiFiClient espClient;
PubSubClient client(espClient);

void startWiFiClient()
{
  Serial.println("Connecting to "+(String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  delay(5000);

   if(WiFi.status() == WL_CONNECTED)
   {
    digitalWrite(LED_PIN1, HIGH);
    delay(500);
    digitalWrite(LED_PIN1, LOW);
    Serial.println("WiFi client connected");
   Serial.println("IP address: " + WiFi.localIP().toString());
   }
   if(WiFi.status() != WL_CONNECTED)
   {
    digitalWrite(LED_PIN2, HIGH);
    delay(500);
    digitalWrite(LED_PIN2, LOW);
    startWiFiAP();
   }

}


void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Hello World","88888888");
  Serial.println("AP started... Hello World");
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  
}
void check()
{
    Serial.printf("Status = %d\n",WiFi.status());
//   myBroker.publish("led", (String)counter++);
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  // wait a second
delay(1000);
}

void(* resetFunc) (void) = 0;//cài đặt hàm reset--quay tro ve dong code dau tien

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);

  startWiFiClient();

// Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();
  myBroker.subscribe("control1");
  myBroker.subscribe("control2");
//
  client.setServer(WiFi.localIP(), 1883);//192.168.4.1,WiFi.localIP().toString()
//


}

//int counter = 1;


void loop()
{

//  check();
  if(WiFi.status()==1)
  {
    resetFunc();//tiến hành reset
  }
// tao timer dem va so sanh voi 1 bien--> bang nhau --> chay ham  startClient
}
