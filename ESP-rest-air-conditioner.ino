/* 

Air Conditioner Remote using ESP-8266 and some libraries:

Web server:

https://github.com/marcoschwartz/aREST

Brilliant Air Conditioner Arduino Library:

https://github.com/ToniA/arduino-heatpumpir



*/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <aREST.h>
#include "urlparser.h"
#include <FujitsuHeatpumpIR.h>

IRSenderBitBang irSender(5);
FujitsuHeatpumpIR *ac;

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "ssid";
const char* password = "password";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API

void setup(void)
{  
  // Start Serial
  Serial.begin(9600);
  
  // Init variables and expose them to REST API

  // Function to be exposed
  rest.function("ac",airConditioner);
  
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  ac = new FujitsuHeatpumpIR();
}


void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}

int airConditioner(String command) {
  char *params[10][2];
  char buf[command.length()+1];

  // AC parameters -  https://github.com/ToniA/arduino-heatpumpir/blob/master/HeatpumpIR.h
  
  unsigned int power = 1;       // power on
  unsigned int opMode = 5;      // mode: fan
  unsigned int fanSpeed =  3;
  unsigned int temp = 23;       // Temperature - celsius
  unsigned int vDir = 2;        // Vertical air directions: up
  unsigned int hDir = 0;        // Horizontal air directions: auto
  bool eco = true;
  
  command.toCharArray(buf, sizeof(buf));
  int results = parseUrlParams(buf, params, 5, true);

  for (int i = 0; i < results; i++) {

      if (strcmp(params[i][0], "power") == 0) {
        power = atoi(params[i][1]);
      }

      if (strcmp(params[i][0], "mode") == 0) {
        opMode = atoi(params[i][1]);
      }

      
      if (strcmp(params[i][0], "fan") == 0) {
        fanSpeed = atoi(params[i][1]);
      }

      if (strcmp(params[i][0], "temp") == 0) {
        temp = atoi(params[i][1]);
      }

      if (strcmp(params[i][0], "vert") == 0) {
        vDir = atoi(params[i][1]);
      }

      if (strcmp(params[i][0], "horiz") == 0) {
        hDir = atoi(params[i][1]);
      }       
    }
    

    ac->send(irSender, power,opMode,fanSpeed, temp, vDir, hDir); 
    return 1;
}

