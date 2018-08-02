/*

     OOOOOOOOO     NNNNNNNN        NNNNNNNNEEEEEEEEEEEEEEEEEEEEEE     LLLLLLLLLLL                  OOOOOOOOO     VVVVVVVV           VVVVVVVVEEEEEEEEEEEEEEEEEEEEEE
   OO:::::::::OO   N:::::::N       N::::::NE::::::::::::::::::::E     L:::::::::L                OO:::::::::OO   V::::::V           V::::::VE::::::::::::::::::::E
 OO:::::::::::::OO N::::::::N      N::::::NE::::::::::::::::::::E     L:::::::::L              OO:::::::::::::OO V::::::V           V::::::VE::::::::::::::::::::E
O:::::::OOO:::::::ON:::::::::N     N::::::NEE::::::EEEEEEEEE::::E     LL:::::::LL             O:::::::OOO:::::::OV::::::V           V::::::VEE::::::EEEEEEEEE::::E
O::::::O   O::::::ON::::::::::N    N::::::N  E:::::E       EEEEEE       L:::::L               O::::::O   O::::::O V:::::V           V:::::V   E:::::E       EEEEEE
O:::::O     O:::::ON:::::::::::N   N::::::N  E:::::E                    L:::::L               O:::::O     O:::::O  V:::::V         V:::::V    E:::::E             
O:::::O     O:::::ON:::::::N::::N  N::::::N  E::::::EEEEEEEEEE          L:::::L               O:::::O     O:::::O   V:::::V       V:::::V     E::::::EEEEEEEEEE   
O:::::O     O:::::ON::::::N N::::N N::::::N  E:::::::::::::::E          L:::::L               O:::::O     O:::::O    V:::::V     V:::::V      E:::::::::::::::E   
O:::::O     O:::::ON::::::N  N::::N:::::::N  E:::::::::::::::E          L:::::L               O:::::O     O:::::O     V:::::V   V:::::V       E:::::::::::::::E   
O:::::O     O:::::ON::::::N   N:::::::::::N  E::::::EEEEEEEEEE          L:::::L               O:::::O     O:::::O      V:::::V V:::::V        E::::::EEEEEEEEEE   
O:::::O     O:::::ON::::::N    N::::::::::N  E:::::E                    L:::::L               O:::::O     O:::::O       V:::::V:::::V         E:::::E             
O::::::O   O::::::ON::::::N     N:::::::::N  E:::::E       EEEEEE       L:::::L         LLLLLLO::::::O   O::::::O        V:::::::::V          E:::::E       EEEEEE
O:::::::OOO:::::::ON::::::N      N::::::::NEE::::::EEEEEEEE:::::E     LL:::::::LLLLLLLLL:::::LO:::::::OOO:::::::O         V:::::::V         EE::::::EEEEEEEE:::::E
 OO:::::::::::::OO N::::::N       N:::::::NE::::::::::::::::::::E     L::::::::::::::::::::::L OO:::::::::::::OO           V:::::V          E::::::::::::::::::::E
   OO:::::::::OO   N::::::N        N::::::NE::::::::::::::::::::E     L::::::::::::::::::::::L   OO:::::::::OO              V:::V           E::::::::::::::::::::E
     OOOOOOOOO     NNNNNNNN         NNNNNNNEEEEEEEEEEEEEEEEEEEEEE     LLLLLLLLLLLLLLLLLLLLLLLL     OOOOOOOOO                 VVV            EEEEEEEEEEEEEEEEEEEEEE


*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


/************ WIFI and MQTT INFORMATION (CHANGE THESE FOR YOUR SETUP) ******************/
#define wifi_ssid "YOUR WIFI NAME"
#define wifi_password "YOUR WIFI PASSWORD"
#define mqtt_server "YOUR MQTT IP"
#define mqtt_port 1883
#define mqtt_user "YOUR MQTT USERNAME"
#define mqtt_password "YOUR MQTT PASSWORD"

/************* MQTT TOPICS (change these topics as you wish)  **************************/
#define button_1_topic "irrigation/zone1_button" //change to whatever you like to use as topic
#define button_2_topic "irrigation/zone2_button" //change to whatever you like to use as topic
#define button_3_topic "irrigation/zone3_button" //change to whatever you like to use as topic
#define button_4_topic "irrigation/zone4_button" //change to whatever you like to use as topic
#define button_5_topic "irrigation/zone5_button" //change to whatever you like to use as topic
#define button_6_topic "irrigation/zone6_button" //change to whatever you like to use as topic
#define button_7_topic "irrigation/zone7_button" //change to whatever you like to use as topic
#define button_8_topic "irrigation/zone8_button" //change to whatever you like to use as topic

/**************************** FOR OTA **************************************************/
#define HOSTNAME "Irrigation_Buttons" // change this to whatever you want to use
#define OTApassword "PW" // change this to whatever password you want to use when you upload OTA
int OTAport = 8266;

/**************************** PIN DEFINITIONS ********************************************/
const int button_1_Pin = D1;
const int button_2_Pin = D2;
const int button_3_Pin = D3;
const int button_4_Pin = D4;
const int button_5_Pin = D5;
const int button_6_Pin = D6;
const int button_7_Pin = D7;
const int button_8_Pin = D8;

int button_1_State = 0;
int button_2_State = 0;
int button_3_State = 0;
int button_4_State = 0;
int button_5_State = 0;
int button_6_State = 0;
int button_7_State = 0;
int button_8_State = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  pinMode(button_1_Pin, INPUT_PULLUP);
  pinMode(button_2_Pin, INPUT_PULLUP);
  pinMode(button_3_Pin, INPUT_PULLUP);
  pinMode(button_4_Pin, INPUT_PULLUP);
  pinMode(button_5_Pin, INPUT_PULLUP);
  pinMode(button_6_Pin, INPUT_PULLUP);
  pinMode(button_7_Pin, INPUT_PULLUP);
  pinMode(button_8_Pin, INPUT_PULLUP);

  Serial.begin(115200);
  delay(10);

  ArduinoOTA.setPort(OTAport);

  ArduinoOTA.setHostname(HOSTNAME);

  ArduinoOTA.setPassword((const char *)OTApassword);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IPess: ");
  Serial.println(WiFi.localIP());
  reconnect();
}

/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/********************************** START RECONNECT*****************************************/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(HOSTNAME, mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/********************************** START MAIN LOOP***************************************/
void loop() {

  ArduinoOTA.handle();
  
  if (!client.connected()) {
   // reconnect();
    software_Reset();
  }
  client.loop();
 
button_1_State = digitalRead(button_1_Pin);
button_2_State = digitalRead(button_2_Pin);
button_3_State = digitalRead(button_3_Pin);
button_4_State = digitalRead(button_4_Pin);
button_5_State = digitalRead(button_5_Pin);
button_6_State = digitalRead(button_6_Pin);
button_7_State = digitalRead(button_7_Pin);
button_8_State = digitalRead(button_8_Pin);

 if ( button_1_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_1_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_1_topic, "on" , true);
    Serial.println("Button 1 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_1_topic, "off" , true);
  }
  }

 if ( button_2_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_2_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_2_topic, "on" , true);
    Serial.println("Button 2 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_2_topic, "off" , true);
  }
  }

 if ( button_3_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_3_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_3_topic, "on" , true);
    Serial.println("Button 3 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_3_topic, "off" , true);
  }
  }

 if ( button_4_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_4_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_4_topic, "on" , true);
    Serial.println("Button 4 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_4_topic, "off" , true);
  }
  }

 if ( button_5_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_5_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_5_topic, "on" , true);
    Serial.println("Button 5 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_5_topic, "off" , true);
  }
  }

 if ( button_6_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_6_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_6_topic, "on" , true);
    Serial.println("Button 6 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_6_topic, "off" , true);
  }
  }

 if ( button_7_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_7_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_7_topic, "on" , true);
    Serial.println("Button 7 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_7_topic, "off" , true);
  }
  }

 if ( button_8_State == LOW ) { // Checks if pin is low
    delay( 500 ); // Delay for 500ms to check for button debounce
 if ( digitalRead(button_8_Pin) == LOW ) { // Read pin to see if still low after debounce delay
    client.publish(button_8_topic, "on" , true);
    Serial.println("Button 8 is pressed!");
    delay( 5000 ); //wait 5 seconds, then publish the off message
    client.publish(button_8_topic, "off" , true);
  }
  }
}

/****reset***/
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
Serial.print("resetting");
ESP.reset(); 
}
