#include "WireSoft.h"
#include "RGB_ADXL345.h"
#include "RGB_ITG2000.h"
#include "RGB_HM5883.h"
void Init_GY_85();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Init_GY_85();
}

void loop() {
  RGB_ITG2000.startGyro();
  RGB_ITG2000.readGyro();
  RGB_HM5883.getRaw();
  RGB_ADXL345.lowpass();
  float heading = atan2(RGB_HM5883.Y, RGB_HM5883.X);
  float declinationAngle = 0.22;
  heading += declinationAngle;
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  float headingDegrees = heading * 180 / M_PI;
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);

  //delay(500);
  //RGB_HM5883.display();
  RGB_ITG2000.update();
  RGB_ITG2000.endGyro();
}

void Init_GY_85() {
  Serial.println("Initializing...!");
  WireSoft.begin();
  RGB_ADXL345.begin(0.5);
  RGB_ITG2000.begin();
  RGB_HM5883.begin();
  RGB_ADXL345.autoOffset();
  Serial.println("Initialized!");
  delay(1000);
  Serial.println("5");
  delay(1000);
  Serial.println("4");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
}
