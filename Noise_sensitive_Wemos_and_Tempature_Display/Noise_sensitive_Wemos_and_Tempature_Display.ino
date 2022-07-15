/* Project: when noise is a certain level display temperature reading and toggle the lights on and off
 * Descpition: lightup NeoPixels with sound
 * Author:  Micalah Miller
 * July-14-2022
*/
#include <SPI.h>
#include<Ethernet.h>
#include<hue.h>
#include <mac.h>
#include <Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>
#include<Wire.h>


 byte thisByte;   //for ethernet IP address
 int averagedReadings;
 int lastSound;

void setup() {

  Serial.begin(9600);
  pinMode(A9, INPUT);  // pin for reading
  pinMode(10, OUTPUT);  // chip select for ehternet
  digitalWrite(10,HIGH); //
  Ethernet.begin(mac);
  delay(200);
  printIP();   
  Serial.printf("LinkStatus: %i \n", Ethernet.linkStatus());  // making sure we have a connection and IP address for the network in the room
  
 // pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
}

void loop() {
  averagedReadings = averageMicrophoneReadings();
  if (averagedReadings > 800) {    //turn the lights in the room on if sound has been heard
    turnLightsOn();
    lastSound =  millis();
    }                          
  if (millis()-lastSound > 3000){
    turnLightsOff();
    lastSound = millis();
    }
    
}

int averageMicrophoneReadings(){
  int average;
  int summation = 0;
  int min = 1023;  //start min high and bring it down
  int max = 0;  //start max low and bring it up
  int i;
  int  microValue;

  for(i = 0; i < 100; i++){  // averages the reading from the microphone every 100 times to help level off backround noise 
    microValue = analogRead(A9);
    summation = microValue + summation; // adds the updated values at the top of every loop to summation
    if(min > microValue) {  //updates minimum and maximum values 
       min = microValue;
    }
     if (max < microValue){
       max = microValue;    
   }
    }
    
  Serial.printf("Min: %i \n, Max: %i \n", min, max);
    average = (summation/100);
    return  average;  
}
void turnLightsOn(){  //get all smart lights in room to turn on I think
 int lightNumber;
    for (lightNumber = 1; lightNumber < 7; lightNumber++){
    setHue(lightNumber, true, HueRainbow[lightNumber%7], 80, 255);
    }      
}
void turnLightsOff(){
  int lightNumber;
      for(lightNumber = 1; lightNumber < 7; lightNumber++){
        setHue(lightNumber, false, HueRainbow[lightNumber%7], 0, 0);
        }
      }

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
  Serial.printf("%i.",Ethernet.localIP()[thisByte]);
 }
 Serial.printf("%i\n",Ethernet.localIP()[3]);
}
