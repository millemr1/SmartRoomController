/* Project: playing with time to make the neopixel light
 * Descpition: make things happen with time to the neopixel
 * Author:  Micalah Miller
 * July-14-2022
*/
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#include <colors.h>

int PIXELCOUNT = 1;
int PIXELPIN = 9;  
int i;


Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);

void setup() {
//  setSyncProvider(getTeensy3Time);
  //setSyncProvider();
  Serial.begin(9600);
  pixel.begin();
  pixel.show();
  
}

void loop() {
 int seconds = second(); 
 pixel.setBrightness(0, 0+1);
 pixel.show();
 int minutes = minute();
 pixel.setPixelColor(0, rainbow[i%7]);
 Serial.printf("minute: %i \n second: %i \n" , minutes, seconds);

}
