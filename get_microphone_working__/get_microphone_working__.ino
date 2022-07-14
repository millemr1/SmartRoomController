/* Project: NeoPixel
 * Descpition: lightup NeoPixels
 * Author:  Micalah Miller
 * July-14-2022
*/


#include <Adafruit_NeoPixel.h>
#include <colors.h>

///int sensorValue;
int PIXELCOUNT = 1;
int PIXELPIN = 9; 
//int i; 
int loudnessToBrightness;
int average;


Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN,  NEO_GRB + NEO_KHZ800);

void setup() {
  pixel.begin();
  pixel.show();
  Serial.begin(9600);
  //pinMode(A9, INPUT);

}

void loop() {
 
  ///Serial.printf("Sensor Value: %i\n" , );
  averageMicrophoneReadings();
  loudnessToBrightness = map(average, 0, 1024,0,64);
  pixel.setBrightness(loudnessToBrightness);
  pixel.setPixelColor(0, blue);
  pixel.show();
}

int averageMicrophoneReadings(){
  int summation = 0;
  int min = 0;
  int max = 0;
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
    average = (summation/1000);
    return average;
}
  
  
