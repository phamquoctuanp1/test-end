#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include <PubSubClient.h>
#define LED_PIN1 D6
#define LED_PIN2 D7
int speakerPin = D8;
StaticJsonDocument<300> doc;

const char* ssid = "Pham Quoc Tuan MTA";     // your network SSID (name):QUOCDAT::Phenikaa 167 HN
const char* pass = "tuan151299"; // your network password:66668888:phenikaa2018

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
    
    virtual void onData(String topic, const char *data, uint32_t length) {
     char data_str[length+1];
     os_memcpy(data_str, data, length);
     data_str[length] = '\0';  
     DeserializationError error  = deserializeJson(doc, data);//data = json = {/"led/":1,/"speaker/":1}
     if (error)
     {
         Serial.println(" deserrialzeJon()failed ");

     }
     int led = doc["led"];
     int speaker = doc["speaker"]; 
     
     if(led == 0 && speaker==0) 
     {
        digitalWrite(LED_PIN2,HIGH); 
        delay(1000);
        digitalWrite(LED_PIN2,LOW); 
        Serial.println(" NO " );
        tone(speakerPin,100,1000);
     } 

     if(led==1 && speaker==1)
     {
        digitalWrite(LED_PIN1, HIGH);
        delay(1000);
        digitalWrite(LED_PIN1, LOW); 
        Serial.println(" YES " );
        tone(speakerPin,1000,500);
      
     }
     

      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
     }
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
    digitalWrite(LED_PIN2, HIGH);
    delay(2000);
    digitalWrite(LED_PIN2, LOW);
    Serial.println("WiFi client connected");
   Serial.println("IP address: " + WiFi.localIP().toString());
   }
   if(WiFi.status() != WL_CONNECTED)
   {
    digitalWrite(LED_PIN1, HIGH);
    delay(2000);
    digitalWrite(LED_PIN1, LOW);
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

  startWiFiClient();

// Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();
  myBroker.subscribe("led");
//
  client.setServer("192.168.4.1", 1883);
//


}

//int counter = 1;

//  time
void loop()
{
//  gettime
//  currenttime ss time
  check();
  if(WiFi.status()==1)
  {
    resetFunc();//tiến hành reset
  }
// tao timer dem va so sanh voi 1 bien--> bang nhau --> chay ham  startClient
}
