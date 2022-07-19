#include<SPI.h>
#include <wemo.h>
#include<mac.h>
#include<hue.h>
#include <TimeLib.h>
void setup() {
 setSyncProvider(getTeensy3Time); 
  turnLightsOff();
  Serial.begin(9600);
  while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }
  pinMode(10, OUTPUT);  // enables serial monitor pin
  digitalWrite(10, HIGH);
  Ethernet.begin(mac); //begin ethernet connection
  delay(200);
//  printIP();
}
void loop() {
  makeLightsDim();
}

void makeLightsDim(){    //dim lights at specific time
 int currentTime = getCurrentTime();
 int dimLightsTime =  setSpecifiedTime(16,00);
 int brightness;
 static bool dimState = false;
 int lightNumber;
 int startMinute;
 int endMinute;
  if(DoTimesMatch(dimLightsTime, currentTime)){
       dimState = true;
       Serial.printf("dim state enabled \n");
       turnLightsOn();
       startMinute = minute();   // I could consolidate these but I will confuse myself if I do.
       endMinute = startMinute + 5;
   }
  if(dimState){
    minute();
    Serial.printf("dim state true \n");
   if(minute() > startMinute && minute() < endMinute){
     brightness = 250 - ((minute()-startMinute)*50);//incrementally decreasing my lights
     Serial.printf("Brightness: i% \n" , brightness);
      for (lightNumber = 1; lightNumber < 7; lightNumber++){
        setHue(1, true, HueRainbow[(lightNumber%7)-1], brightness, 255);  //
        Serial.printf("Lights are dimming");
      }                                   
   }
  }
    //else{
  if(brightness <= 0){
    dimState = false;
   }
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
  minutes = minute();
  hours = hour();
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
