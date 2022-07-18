/* Project: get wemos to turn on at certain times
 * Descpition: lightup NeoPixels with sound
 * Author:  Micalah Miller
 * July-14-2022
*/

#include<SPI.h>
#include <Ethernet.h>
#include<mac.h>
#include<hue.h>
#include<Wire.h>
#include <Wemo.h>
#include <Timelib.h>

bool status;
byte thisbyte;  //for ip address

void setup() {
  inMode(10, OUTPUT);  // enables serial monitor pin
  digitalWrite(10, HIGH);

  Serial.begin(9600);

   status = Ethernet.begin(mac);    //begin ethernet connection
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
  if(DoTimesMatch())
  switchON(3);
  }
  else();
  switchOFF(3);
  }

int getCurrentTime(){ //try to write to get it on at anytime I pass into the function
  int currentTime;
  int wemoNumber;  //three is tea Kettle for your functionality
  int hours = hour();
  int minutes = minute();
  int seconds = second();  //might be extraneous?
  
  currentTime = hour + minute + second;
  Serial.printf("Time: %i : %i, %i", hours, minutes, seconds);  // have it display to screen eventually?
    return currentTime;
  }
int setSpecificiedTime(int hours, int minutes, int seconds){  //I am quite unsure what I am doing here
  specficTime = hours + minutes + seconds
  return specficiedTime
}
bool DoTimesMatch(currentTime, specifiedTime){
  bool timesMatch;
  if (currentTime == specifiedTime){  //this is most certainly wrong
    timesMatch = true;
  }
 else{
   timesMath = false;
  }
 return timesMatch;
}


  
