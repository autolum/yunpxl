#include <Bridge.h>
#include <Console.h>
#include <YunServer.h>
#include <YunClient.h>

#include "SPI.h"
#include "WS2801.h"

#define NPXLS 3

uint8_t dataPin  = 3;
uint8_t clockPin = 2;

const int ledPin = 13; 
YunServer server(5555);

Adafruit_WS2801 strip = Adafruit_WS2801(NPXLS, dataPin, clockPin);

void setup() {
  // Bridge startup
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Bridge.begin();
  
  // Listen the entire network (socket communication)
  server.noListenOnLocalhost();
  server.begin();

  Serial.begin(9600);
  
  strip.begin();
  strip.show();

}

void loop() {
  
  // Get clients coming from server
  YunClient client = server.accept();
  
  // There is a new client
  if (client) {
    // Change the predifined timeout from 2000 to 5. Avoid impressive timeout.
    client.setTimeout(5);
    Serial.println("Client connected!");
    digitalWrite(ledPin,HIGH);

    // When we get a client, go in the loop 
    // and exit only when the client disconnect.
    while(client.connected()){	
      // Process request
      process(client);
    }
    
    // Close connection and free resources.
    client.stop();
    
    digitalWrite(ledPin,LOW);
  }
  
  else {
    Serial.println("no client connected, retrying");
  }
  
  
  strip.show();
  
  delay(100);

}

void process(YunClient client) {
  
  // Format: LED/n/R/G/B
  // Get the first element of the command.
  String command = client.readStringUntil('LED/');
  // Avoid interferences when there is no request.
  if(command.length() >0){
    int r = client.parseInt();
    int g = client.parseInt();
    int b = client.parseInt();
    strip.setPixelColor(0, cRGB(r,g,b));
    strip.setPixelColor(1, cRGB(g,b,r));
    strip.setPixelColor(2, cRGB(b,r,g));
    strip.show();
  }
  strip.show();
  //delay(100);
}

// Create a 24 bit color value from R,G,B
uint32_t cRGB(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}


