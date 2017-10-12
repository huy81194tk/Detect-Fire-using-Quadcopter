#include <PID_v1.h>
#include <SimpleKalmanFilter.h>
#include "WireSoft.h"
#include "RGB_ADXL345.h"
#include "RGB_ITG2000.h"
#include "RGB_HM5883.h"
double SetpointX,InputX,OutputX;
double SetpointY,InputY,OutputY;
double SetpointZ,InputZ,OutputZ;
double KpX=2, KiX=5, KdX=1;
double KpY=2, KiY=5, KdY=1;
double KpZ=2, KiZ=5, KdZ=1;

PID myX(&InputX,&OutputX,&SetpointX,KpX,KiX,KdX,DIRECT);
PID myY(&InputY,&OutputY,&SetpointY,KpY,KiY,KdY,DIRECT);
PID myZ(&InputZ,&OutputZ,&SetpointZ,KpZ,KiZ,KdZ,DIRECT);
void Init_GY_85();
void update(float minX = -0.02, float maxX = 0.02, float minY = -0.02, float maxY = 0.02, float minZ = -0.01, float maxZ = 0.01);
void update_new(float minX = -0.2, float maxX = 0.2, float minY = -0.2, float maxY = 0.2, float minZ = -0.01, float maxZ = 0.01);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Init_GY_85();
  SetpointX = -5;
  SetpointY = 5;
  SetpointZ = 5;
  myX.SetMode(AUTOMATIC);
  myY.SetMode(AUTOMATIC);
  myZ.SetMode(AUTOMATIC);
}

void loop() {
  int start = millis();
  RGB_ITG2000.startGyro();
  RGB_ITG2000.readGyro();
  RGB_ADXL345.lowpass();
  RGB_HM5883.getDegrees();
  //Bỏ 2 dòng tiếp theo và thêm PID vào đây
  //RGB_HM5883.display_plot();
  //RGB_ADXL345.display_plot();
  InputX = (double)RGB_ITG2000.X;
  InputY = (double)RGB_ITG2000.Y;
  InputZ = (double)RGB_ITG2000.Z;
  RGB_ITG2000.display_plot();
  myX.Compute();
  myY.Compute();
  myZ.Compute();
  Serial.print(OutputX);Serial.print(",");
  Serial.print(OutputY);Serial.print(",");  
  Serial.println(OutputZ);
  //RGB_HM5883.display();
  //
  update_new();
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
void update_new(float minX, float maxX, float minY, float maxY, float minZ, float maxZ){
  if (RGB_ADXL345.X >= minX && RGB_ADXL345.X <= maxX) RGB_ITG2000.X = 0;
  if (RGB_ADXL345.Y >= minY && RGB_ADXL345.Y <= maxY) RGB_ITG2000.Y = 0;
  if (RGB_HM5883.headingnow >= minZ && RGB_HM5883.headingnow <= maxZ) RGB_ITG2000.Z = 0;
}

