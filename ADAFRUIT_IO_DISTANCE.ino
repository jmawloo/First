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

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "IoT2018"
#define WLAN_PASS       "00000000"
const int trigPinL = 12;
const int echoPinL = 14;

const int trigPinR = 15;
const int echoPinR = 13;

const int trigPinF = 5;
const int echoPinF = 2;

const int trigPinB = 16;
const int echoPinB = 0;

long durationR;
int distanceR;

long durationL;
int distanceL;

long durationB;
int distanceB;

long durationF;
int distanceF;

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "sarf28"
#define AIO_KEY         "142f4c7ed6d64595a8bad07d78c3210b"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Front = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Font");

Adafruit_MQTT_Publish Back = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Back");

Adafruit_MQTT_Publish Left = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Left");

Adafruit_MQTT_Publish Right = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Right");

Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  pinMode(trigPinL, OUTPUT); 
  pinMode(echoPinL, INPUT); 

  pinMode(trigPinR, OUTPUT); 
  pinMode(echoPinR, INPUT); 

  pinMode(trigPinF, OUTPUT); 
  pinMode(echoPinF, INPUT); 

  pinMode(trigPinB, OUTPUT); 
  pinMode(echoPinB, INPUT); 
  
  Serial.begin(115200);
  delay(10);

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
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  front();
  back();
  left();
  right();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
    if((char *)onoffbutton.lastread=="OFF")
    {
      Serial.println("It's off");
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending FRONT val "));
  Serial.print(distanceF);
  Serial.print("...");
  if (! Front.publish(distanceF)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending Back val "));
  Serial.print(distanceB);
  Serial.print("...");
  if (! Back.publish(distanceB)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending Left val "));
  Serial.print(distanceL);
  Serial.print("...");
  if (! Left.publish(distanceL)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending Right val "));
  Serial.print(distanceR);
  Serial.print("...");
  if (! Right.publish(distanceR)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
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



void front()
{
  digitalWrite(trigPinF, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPinF, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinF, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationF = pulseIn(echoPinF, HIGH);
// Calculating the distance
distanceF= durationF*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distanceF);
}

void back()
{
  digitalWrite(trigPinB, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPinB, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinB, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationB = pulseIn(echoPinB, HIGH);
// Calculating the distance
distanceB= durationB*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distanceB);
}

void right()
{
  digitalWrite(trigPinR, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPinR, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinR, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationR = pulseIn(echoPinR, HIGH);
// Calculating the distance
distanceR= durationR*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distanceR);
}


void left()
{
  digitalWrite(trigPinL, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPinL, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinL, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
durationL = pulseIn(echoPinL, HIGH);
// Calculating the distance
distanceL= durationL*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distanceL);
}



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
