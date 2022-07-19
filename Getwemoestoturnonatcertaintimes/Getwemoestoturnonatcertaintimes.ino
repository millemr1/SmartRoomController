/* Project: get wemos to turn on at certain times
 * Descpition: lightup NeoPixels with sound
 * Author:  Micalah Miller
 * July-14-2022
*/
// I want to use time to turn on wemos (2 smart outlets on twice a day);


#include<SPI.h>
#include <wemo.h>
#include<mac.h>
#include <TimeLib.h>


//int currentTimeforWemos;
int specTime;
int currentTiming;
bool timesMatched;
bool status;
byte thisbyte;  //for ip address
void setup() {
  
   // set the Time library to use Teensy 3.0's RTC to keep time
 setSyncProvider(getTeensy3Time);  
  
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
  status = Ethernet.begin(mac); //begin ethernet connection
  delay(200);
//  printIP();
  
}
void loop() {
  
  currentTiming = getCurrentTime();  //somehow this feels redundant
  specTime= setSpecifiedTime(10,21,00);  //somehow this also feels redundant
  //timesMatched = DoTimesMatch(currentTiming, specTime);
  if(DoTimesMatch(currentTiming, specTime)){   //Keep wemo on for a few minutes would be something like 
     Serial.printf("times match");
     switchON(4);
     turnWemoOffAfter3Minutes(4); 
  }
  //else();
 // switchOFF(3);
  digitalClockDisplay();  
}
int getCurrentTime(){ //try to write to get it on at anytime I pass into the function
  int currentTime;
  int hours = hour();
  int minutes = minute();
  int seconds = second();  //might be extraneous?
  currentTime = hours + minutes + seconds;
  Serial.printf("Time: %i : %i, : %i \n", hours, minutes, seconds);  // have it display to screen eventually?
    return currentTime;
  }
int setSpecifiedTime(int hours, int minutes, int seconds){  //I am quite unsure what I am doing here
  int certainTime = hours + minutes + seconds;
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

void turnWemoOffAfter3Minutes(int wemoNumber){ //turn Wemo off for 3 minutes
  int currentTimeforWemos = millis();
  int lastTimeforWemos;
  if(currentTimeforWemos - lastTimeforWemos > 180000) {
    switchOFF(wemoNumber);
    lastTimeforWemos = millis();
    Serial.printf("turning off wemo");
    } 
}


  
