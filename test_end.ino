#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include <PubSubClient.h>

#include <DFRobotDFPlayerMini.h>
#include "SoftwareSerial.h"

SoftwareSerial mySoftwareSerial(D3, D4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int LED_PIN1 = D5;//blue
int LED_PIN2 = D6;//green
int LED_PIN3 = D7;//red 

StaticJsonDocument<300> doc;

int dem = 1000;
unsigned long time_now1 =0;
unsigned long time_now2 =0;

const char* ssid = "A405";     // your network SSID (name):QUOCDAT:Pham Quoc Tuan MTA:Phenikaa 167 HN
const char* pass = "tuan211095"; // your network password::66668888:tuan151299:phenikaa2018

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
        myDFPlayer.play(1);
        Serial.println(" led = 1 , speaker = 1 " );
        time_now1 =millis();
        digitalWrite(LED_PIN1, HIGH);
        while(millis() < time_now1 + dem)
        {}
        digitalWrite(LED_PIN1, LOW);
        
     }
          if(led==2 && speaker==2)
     {
        myDFPlayer.play(3);
        Serial.println(" led = 2 , speaker = 2 " );
        time_now2 =millis();
        digitalWrite(LED_PIN3, HIGH);
        while(millis() < time_now2 + dem)
        {
          delay(100);
        }
        digitalWrite(LED_PIN3, LOW);
        
     }
     else{}
      //Serial.println("received topic '"+control1+"' with data '"+(String)data_str+"'");
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
  WiFi.mode(WIFI_STA);Serial.println("131");
  WiFi.begin(ssid, pass);Serial.println("132");
  delay(8000);Serial.println("133");

   if(WiFi.status() == WL_CONNECTED)
   {Serial.println("135");
    digitalWrite(LED_PIN1, HIGH);
    delay(500);
    digitalWrite(LED_PIN1, LOW);
    Serial.println("WiFi client connected");
    Serial.println("IP address: " + WiFi.localIP().toString());
    client.setServer(WiFi.localIP(), 1883);Serial.println("142");
    //
    
   }
   if(WiFi.status() != WL_CONNECTED)
   {
    Serial.println("145");
    digitalWrite(LED_PIN2, HIGH);
    delay(500);
    digitalWrite(LED_PIN2, LOW);
    startWiFiAP();Serial.println("152");
   }

}


void startWiFiAP()
{
  WiFi.mode(WIFI_AP);Serial.println("160");
  WiFi.softAP("Hello World","88888888");Serial.println("161");
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

void(* resetFunc) (void) = 0;
void setup()
{
    mySoftwareSerial.begin(9600);Serial.println("180");
    if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {  while(true){delay(0); }   }Serial.println("181");
    myDFPlayer.volume(30);Serial.println("182");
//  myDFPlayer.play(1);
//  delay(2000);
//  myDFPlayer.play(2);
//  delay(2000);
//  myDFPlayer.play(3);
//  delay(2000);
//  myDFPlayer.play(4);
//  delay(2000);
//  digitalWrite(LED_PIN1,HIGH);
//  delay(500);
//  digitalWrite(LED_PIN2,HIGH);
//  delay(500);
//  digitalWrite(LED_PIN3,HIGH);
  
  Serial.begin(115200);Serial.println("197");
  
  Serial.println();
  Serial.println();
  
  pinMode(LED_PIN1, OUTPUT);Serial.println("202");
  pinMode(LED_PIN2, OUTPUT);Serial.println("203");
  pinMode(LED_PIN3, OUTPUT);Serial.println("204");

  startWiFiClient();Serial.println("206");
  
  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();Serial.println("211");
  
  myBroker.subscribe("control1");Serial.println("213");
  myBroker.subscribe("control2");Serial.println("214");
  //myBroker.publish("control1", WiFi.localIP().toString());
  
}

//int counter = 1;


void loop()
{
  client.loop();
  //Serial.println("221");
  client.publish("control1", "hello");
//  check();
  if(WiFi.status()==1)
  {
    Serial.println("225");
    resetFunc();
  }
// tao timer dem va so sanh voi 1 bien--> bang nhau --> chay ham  startClient
}
