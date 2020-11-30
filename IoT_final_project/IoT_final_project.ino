/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
const int trigPin = 4;  //D4 ultra sonic
const int echoPin = 0;  //D3 ultra sonic
const int smokeA0 = A0;// gas sensor 

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "$$$$$$$"
#define WLAN_PASS       "$$$$$$$"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "###############"
#define AIO_KEY         "######################"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish gasvalue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas_value");
Adafruit_MQTT_Publish ultravalue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ultra_value");


// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe ultravalue = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ultra_value");
//Adafruit_MQTT_Subscribe ultravalue = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ultra_value");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(smokeA0, INPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input ek second ayyi aawwwwww dungarpur aa rahe ho . aajau masti karenge XD
pinMode(2, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
 // mqtt.subscribe(&gasvalue);
 // mqtt.subscribe(&ultravalue);
  
}

uint32_t x=0;
uint32_t u=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  /*Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &ultravalue) {
      Serial.print(F("Got: "));
      Serial.println((char *)ultravalue.lastread);
    }
  }*/

int analogSensor = analogRead(smokeA0);
  // Now we can publish stuff!
  Serial.print(F("Gas value"));
 
  Serial.println(analogSensor);
  if(analogSensor>300)
 {
  digitalWrite(2, LOW);  // Turn the LED off by making the voltage HIGH
  delay(2000);  
 }
 else
 {
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);   
 }
 // Serial.print("...");
  if (!gasvalue.publish(analogSensor)) {
    Serial.println(F("Failed"));
  } 
delay(2000);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, LOW);
delayMicroseconds(10);
digitalWrite(trigPin, HIGH);

// Reads the echoPin, returns the sound wave travel time in microseconds
int duration = pulseIn(echoPin, HIGH);

// Calculating the distance
int distance= duration*0.034/2;
Serial.println("Distance:");
Serial.println(distance);
if(distance<50)
{
 digitalWrite(LED_BUILTIN,LOW);
 delay(1000);
}
else
{
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
}

if (!ultravalue.publish(distance)) {
    Serial.println(F("Failed"));
  } 
delay(2000);



  

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
