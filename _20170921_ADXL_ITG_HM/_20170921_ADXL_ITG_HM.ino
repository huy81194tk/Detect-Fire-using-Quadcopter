#include <SimpleKalmanFilter.h>
#include "WireSoft.h"
#include "RGB_ADXL345.h"
#include "RGB_ITG2000.h"
#include "RGB_HM5883.h"

void Init_GY_85();
void update(float minX = -0.02, float maxX = 0.02, float minY = -0.02, float maxY = 0.02, float minZ = -1.052, float maxZ = 1.052);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Init_GY_85();
}

void loop() {
  int start = millis();
  RGB_ITG2000.startGyro();
  RGB_ITG2000.readGyro();
  RGB_ADXL345.lowpass();
  RGB_HM5883.getDegrees();
  //Bỏ 2 dòng tiếp theo và thêm PID vào đây
  RGB_HM5883.display_plot();
  RGB_ITG2000.display_plot();
  //
  update();
  RGB_HM5883.Update(5);
  //Serial.println((millis() - RGB_HM5883.UpdateHm) / 1000);
  RGB_ITG2000.endGyro();
  //delay(33 - (millis()-start));
  //Serial.println(millis()-start);
}

void Init_GY_85() {
  Serial.println("Initializing...!");
  WireSoft.begin();
  RGB_ADXL345.begin(0.5);
  RGB_ITG2000.begin();
  RGB_HM5883.begin();
  RGB_ADXL345.autoOffset();
  RGB_HM5883.start();
  //RGB_HM5883.autoOffset();
  Serial.println("Initialized!");
  delay(10);
  Serial.println("5");
  delay(10);
  Serial.println("4");
  delay(10);
  Serial.println("3");
  delay(10);
  Serial.println("2");
  delay(10);
  Serial.println("1");
  delay(10);
}
void update(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
  if (RGB_ADXL345.X >= minX && RGB_ADXL345.X <= maxX && RGB_ADXL345.Y >= minY && RGB_ADXL345.Y <= maxY && RGB_ADXL345.Z >= minZ && RGB_ADXL345.Z <= maxZ) {
    RGB_ITG2000.X = 0;
    RGB_ITG2000.Y = 0;
  }
  if ((RGB_HM5883.headingnow <= 0.01) && (RGB_HM5883.headingnow >= -0.01)) {
    RGB_ITG2000.Z = 0;
  }
}
