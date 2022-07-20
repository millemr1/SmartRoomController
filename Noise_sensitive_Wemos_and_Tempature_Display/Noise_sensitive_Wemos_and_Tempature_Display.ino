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
#include <TimeLib.h>
#include <wemo.h>

byte thisByte;   //for ethernet IP address
int averagedReadings;
int lastSound;

const int screenWidth = 128;
const int screenHeight = 64;
const int screenAddress= 0x3C;  //Screen Address (I think)
const int sensorAddress = 0x76; //temp, press, and sensor adress on breadboard
const int  OLEDReset = -1;

int currentTimeforWemos;
int lastTimeforWemos;
int TeaTime;
int BedTime;
int currentTiming;
bool timesMatched;
 
Adafruit_BME280 bme;   //declare objects
Adafruit_SSD1306 display(screenWidth , screenHeight, &Wire, OLEDReset);

void setup() {
     // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time); 

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
  turnLightsOff();
}
void loop(){
 averagedReadings = averageMicrophoneReadings();
 if (averagedReadings > 650) {    //turn the lights in the room on if sound has been heard
    Serial.printf("sound has been detected lights will be on soon");
    turnLightsOn();
    Serial.printf("the lights on function worked");
    displayText();
    Serial.printf("the display worked");
    takeReadings();
    Serial.printf("readings were taken");
    lastSound =  millis();
 }                          
 if (millis()-lastSound > 2000){  //turn of sound if light hasn't been heard
    display.clearDisplay();
    Serial.printf("sound has not been detected for 5 seconds turning lights off");//turn off the display when sound is not heard
    turnLightsOff();
    lastSound = millis();
 }   
currentTiming = getCurrentTime();  //somehow this feels redundant
TeaTime = setSpecifiedTime(11,19);  //somehow this also feels redundant
BedTime = setSpecifiedTime(11,16);

   if(DoTimesMatch(TeaTime,currentTiming)){   //Keep wemo on for a few minutes would be something like 
    Serial.printf("times match turning wemo 4 on \n");
    switchON(4); 
     }
    if(DoTimesMatch(BedTime, currentTiming)){
      Serial.printf("turning Wemo 0 on");
      switchON(0);
      }
currentTimeforWemos = millis();
    if(currentTimeforWemos - lastTimeforWemos > 30000) {
      switchOFF(4);
      switchOFF(0);
    lastTimeforWemos = millis();
    Serial.printf("turning off wemo \n");
    } 
   digitalClockDisplay();  
   makeLightsDim();
}
int getCurrentTime(){ //try to write to get it on at anytime I pass into the function
  
  int currentTime;
  int hours = hour();  //tells you what minute of the day it is
  int minutes = minute();
  //might be extraneous?
  currentTime = (hours*60)+minutes;
  Serial.printf("Time: %i : %i, : %i \n", hours, minutes);  // have it display to screen eventually?
    return currentTime;
}
int setSpecifiedTime(int hours, int minutes){
  int certainTime =(hours*60)+minutes;
  return certainTime;
}
bool DoTimesMatch(int time1, int time2){
  bool timesMatch;
  if (time1 == time2){  //this is most certainly wrong
    timesMatch = true;
   }
 else{
   timesMatch = false;
  }
 return timesMatch;
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
void makeLightsDim(){    //dim lights at specific time
 int currentTime = getCurrentTime();
 int dimLightsTime =  setSpecifiedTime(17,48);
 static int brightness=250;
 static bool dimState = false;
 int lightNumber;
 static int startMinute;
 static int endMinute;
 int lastTime;

 Serial.printf("dimSum %i, current %i, difference %i\n",dimLightsTime, currentTime, currentTime - dimLightsTime);
  if(DoTimesMatch(dimLightsTime, currentTime) && !dimState){
       dimState = true;
       Serial.printf("dim state enabled \n");
       turnLightsOn();
       startMinute = getCurrentTime();   // I could consolidate these but I will confuse myself if I do.
       endMinute = startMinute + 5;
   }
  if(dimState){
    Serial.printf("dim state true \n");
    Serial.printf("Prepare to be dim --- dimSum %i, current %i, difference %i\n",dimLightsTime, currentTime, currentTime - dimLightsTime);
   if(currentTime > startMinute && currentTime < endMinute){
     brightness = 250 - ((currentTime-startMinute)*50);//incrementally decreasing my lights
     Serial.printf("Brightness: i% \n" , brightness);
     if(currentTime != lastTime){
      for (lightNumber = 1; lightNumber < 7; lightNumber++){
        setHue(lightNumber, true, HueRainbow[(lightNumber%7)-1], brightness, 255);  //
        Serial.printf("Lights are dimming");
        lastTime = getCurrentTime();
       }
      }                                   
   }
  }
  if((brightness <= 0) && dimState){
    Serial.printf("Setting Dimsum to false\n");
    dimState = false;
    delay(60000);
   }
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
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}
/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message
unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
