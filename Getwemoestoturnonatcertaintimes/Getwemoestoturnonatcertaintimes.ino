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
  setSyncProvider(getTeensy3Time);  //what goes through here?
  
  Serial.begin(9600);
  while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }
}
  
  pinMode(10, OUTPUT);  // enables serial monitor pin
  digitalWrite(10, HIGH);
  status = Ethernet.begin(mac); //begin ethernet connection
  delay(200);
//  printIP();
  if (!status) {
    Serial.printf("failed to configure Ethernet using DHCP \n");
    //no point in continueing 
    while(1);
  }
  Serial.print("My IP address: ");    //get local IP address for the room, somehow. not quite sure why we use this incrementing.
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {
    //print value of each byte of the IP address
    Serial.printf("%i.",Ethernet.localIP()[thisbyte]);
    }
 Serial.printf("%i\n",Ethernet.localIP()[thisbyte]);
}
void loop() {
  //currentTimeforWemos = millis();
  currentTiming = getCurrentTime();  //somehow this feels redundant
  specTime= setSpecifiedTime(17,49,01);  //somehow this also feels redundant
  //timesMatched = DoTimesMatch(currentTiming, specTime);
  if(DoTimesMatch(currentTiming, specTime)){   //Keep wemo on for a few minutes would be something like 
     switchON(3);
  }
  //else();
 // switchOFF(3);
  
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

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
  Serial.printf("%i.",Ethernet.localIP()[thisByte]);
 }
 Serial.printf("%i\n",Ethernet.localIP()[3]);
}
  


  
