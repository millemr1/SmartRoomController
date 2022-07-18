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
#include <Adafruit_BME280.h>
#include<Wire.h>


 byte thisByte;   //for ethernet IP address
 int averagedReadings;
 int lastSound;


const int screenWidth = 128;
const int screenHeight = 64;
const int screenAddress= 0x3C;  //Screen Address (I think)
const int sensorAddress = 0x76; //temp, press, and sensor adress on breadboard
const int  OLEDReset = -1;
 
Adafruit_BME280 bme;   //declare objects
Adafruit_SSD1306 display(screenWidth , screenHeight, &Wire, OLEDReset);

void setup() {

  Serial.begin(9600);
  pinMode(A9, INPUT);  // pin for reading
  pinMode(10, OUTPUT);  // chip select for ehternet
  digitalWrite(10,HIGH); //
  Ethernet.begin(mac);
  delay(200);
  printIP();   
  Serial.printf("LinkStatus: %i \n", Ethernet.linkStatus());  // making sure we have a connection and IP address for the network in the room

  bme.begin(sensorAddress); // initialize bme address
  display.begin(SSD1306_SWITCHCAPVCC,screenAddress);  // initialize display 
  display.display();

 // pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
}

void loop() {
  averagedReadings = averageMicrophoneReadings();
  if (averagedReadings > 650) {    //turn the lights in the room on if sound has been heard
    Serial.printf("sound has been detected lights will be on soon");
    turnLightsOn();
    Serial.printf("the lights on function worked");
    displayText();
    Serial.printf("the display worked");
    takeReadings();
    Serial.printf("readings were taked");
    lastSound =  millis();
    }                          
  if (millis()-lastSound > 2000){
    display.clearDisplay();
    Serial.printf("sound has not been detected for 5 seconds turning lights off");//turn off the display when sound is not heard
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
    setHue(lightNumber, true, HueRainbow[(lightNumber%7)-1], 150, 255);  //get to start 
    Serial.printf("lights are on");
    }      
}
void turnLightsOff(){
  int lightNumber;
      for(lightNumber = 1; lightNumber < 7; lightNumber++){
        setHue(lightNumber, false, HueRainbow[(lightNumber%7)-1], 0, 0);
        }
        Serial.printf("lights are off");
      }
void takeReadings(){     // take and convert temperature and pressure readings and display them on the screen
  float tempC;
  float pressPA;
  float humidRH;
  float roomTempF;
  float pressureHG;

Serial.printf("before readings");
 tempC= bme.readTemperature();
 pressPA = bme.readPressure();
 humidRH = bme.readHumidity();
 Serial.printf("humidity: %0.2f \n", humidRH);
 roomTempF = (tempC*1.8)+32;  // convert to Celcius to Farenheit degrees
 pressureHG =  (pressPA)*(1/3386.39); //convert from Pascals to units of mercury
 //display.clearDisplay();
 Serial.printf("cleared");
 display.printf(" Welcome, Micalah! \n Temp: %0.2f \n Pressure: %0.2f \n Humidity: %0.2f \n" ,roomTempF, pressureHG,humidRH);
 display.display();
}

void displayText(){ //display things on screen
  Serial.printf("texts should be displaying");
  display.clearDisplay();  //display.clearDisplay();
  display.setTextSize(1); // Normal 1:1 pixel size will help with font
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);//display.setTextSize(1); 
}
void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
  Serial.printf("%i.",Ethernet.localIP()[thisByte]);
 }
 Serial.printf("%i\n",Ethernet.localIP()[3]);
}
