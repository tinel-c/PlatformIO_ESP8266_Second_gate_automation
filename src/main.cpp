/*
 KeyPressed with interrupt and digital write all
 from P4 to P7
 by Mischianti Renzo <http://www.mischianti.org>

 https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
*/

#include "Arduino.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "password.h"



char relayStatusArray[6] = {0,0,0,0,0,0};

// mqtt setup


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void initRelayPins()
{
	// 220V Relays IO16, IO14, IO12, IO13
	pinMode(16,OUTPUT); 
	pinMode(14,OUTPUT);
	pinMode(12,OUTPUT);
	pinMode(13,OUTPUT);

	// 12V mosfets IO5, IO4
	pinMode(5,OUTPUT);
	pinMode(4,OUTPUT);
}

void processRelayStatus()
{
	if(relayStatusArray[0] == 0)  { digitalWrite(16,LOW);} else { digitalWrite(16,HIGH);}
	if(relayStatusArray[1] == 0)  { digitalWrite(14,LOW);} else { digitalWrite(14,HIGH);}
	if(relayStatusArray[2] == 0)  { digitalWrite(12,LOW);} else { digitalWrite(12,HIGH);}
	if(relayStatusArray[3] == 0)  { digitalWrite(13,LOW);} else { digitalWrite(13,HIGH);}
	
	if(relayStatusArray[4] == 0)  { digitalWrite(5,LOW);} else { digitalWrite(5,HIGH);}
	if(relayStatusArray[5] == 0)  { digitalWrite(4,LOW);} else { digitalWrite(4,HIGH);}



}

void printToLCD(char* message) {
	client.publish("SecondGate/STAT/message", message);
}

void printRuntimeToLCD(char* message) {
	client.publish("SecondGate/STAT/message", message);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  printToLCD("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  printToLCD("WiFi connected");
  Serial.println("IP address: ");
  printToLCD("IP address: ");
  Serial.println(WiFi.localIP());
  char* char_array = new char[20]; 
  strcpy(char_array,WiFi.localIP().toString().c_str());
  printToLCD(char_array);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // string to lcd
  String messageTemp;
  char statusReport[10];
  String relay1 = "SecondGate/CMD/Relay1";
  String relay2 = "SecondGate/CMD/Relay2";
  String relay3 = "SecondGate/CMD/Relay3";
  String relay4 = "SecondGate/CMD/Relay4";
  String relay5 = "SecondGate/CMD/Relay5";
  String relay6 = "SecondGate/CMD/Relay6";

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
	messageTemp += (char)payload[i];
  }
  Serial.println();

  if(relay1.compareTo(topic) == 0)
  {
    Serial.println("New number received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[0] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[0] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay1", statusReport);
  }

  if(relay2.compareTo(topic) == 0)
  {
    Serial.println("New text received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[1] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[1] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay2", statusReport);
  }

  if(relay3.compareTo(topic) == 0)
  {
    Serial.println("New text received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[2] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[2] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay3", statusReport);
  }

  if(relay4.compareTo(topic) == 0)
  {
    Serial.println("New text received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[3] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[3] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay4", statusReport);
  }
  
  if(relay5.compareTo(topic) == 0)
  {
    Serial.println("New text received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[4] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[4] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay5", statusReport);
  }

  if(relay6.compareTo(topic) == 0)
  {
    Serial.println("New text received on MQTT: " + messageTemp);
	if(messageTemp.compareTo("ON") == 0) { relayStatusArray[5] = 1; strcpy(statusReport, "ON");}
	if(messageTemp.compareTo("OFF") == 0) { relayStatusArray[5] = 0; strcpy(statusReport, "OFF");}
	client.publish("SecondGate/STAT/Relay6", statusReport);
  }

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

  //process the messages that are arriving
  processRelayStatus();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
	  client.subscribe("SecondGate/CMD/Relay1");
	  client.subscribe("SecondGate/CMD/Relay2");
	  client.subscribe("SecondGate/CMD/Relay3");
	  client.subscribe("SecondGate/CMD/Relay4");
	  client.subscribe("SecondGate/CMD/Relay5");
	  client.subscribe("SecondGate/CMD/Relay6");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup()
{
	// initialize the serial port for debug
	Serial.begin(115200);
	delay(1000);
	Serial.println("Intializing the I2C ports...");
	
	// initialize the port for the relays
	initRelayPins();

	Serial.println("Ready to start");

	//setup the mqtt connection
	//pinMode(BUILTIN_LED, OUTPUT);   
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	printToLCD("Boot-up ...");
}

void loop()
{
	if (!client.connected()) {
    reconnect();
	}
	client.loop();
}