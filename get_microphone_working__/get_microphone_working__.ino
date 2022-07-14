/* Project: NeoPixel
 * Descpition: lightup NeoPixels
 * Author:  Micalah Miller
 * July-14-2022
*/


#include <Adafruit_NeoPixel.h>
int sensorValue;
int PIXELCOUNT = 1;
int PIXELPIN = 9; 
//int i; 
int loudnessToBrightness;

Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN,  NEO_GRB + NEO_KHZ800);

void setup() {
  pixel.begin();
  pixel.show();
  Serial.begin(9600);
  //pinMode(A9, INPUT);

}

void loop() {
  sensorValue = analogRead(A9);
  Serial.printf("Sensor Value: %i\n" , sensorValue);
  //delay(500);
  loudnessToBrightness = map(sensorValue, 0, 255, 0, 1025);
  pixel.setBrightness(loudnessToBrightness);
  pixel.show();
  
}
