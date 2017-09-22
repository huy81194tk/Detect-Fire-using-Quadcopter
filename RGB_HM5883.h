#define HM5883_ADDRESS         (0x1E)
#define HM5883_Mode_Register   (0x02)
#define HM5883_DOX0            (0X03)
#define HM5883_DOX1            (0X04)
#define HM5883_DOY0            (0X05)
#define HM5883_DOY1            (0X06)
#define HM5883_DOZ0            (0X07)
#define HM5883_DOZ1            (0X08)

#define SENSORS_GAUSSE_TO_MICROTESLA  (100)

class RGB_HM5883 {
  private:
    static float HM5883_Gauss_LSB_XY = 1100.0F;
    static float HM5883_Gauss_LSB_Z  = 980.0F;
    int angle;
    int16_t read16(uint8_t reg) {
      return ((int16_t)(WireSoft.ReadData(HM5883_ADDRESS, reg)) << 8) | WireSoft.ReadData(HM5883_ADDRESS, reg + 1);
    }
  public:
    float X = 0;
    float Y = 0;
    float Z = 0;
    void begin() {
      WireSoft.WriteData(HM5883_ADDRESS, HM5883_Mode_Register, 0x00);
    }
    void getRaw() {
      X = (float)read16(HM5883_DOX0) / HM5883_Gauss_LSB_XY * SENSORS_GAUSSE_TO_MICROTESLA;
      Y = (float)read16(HM5883_DOY0) / HM5883_Gauss_LSB_XY * SENSORS_GAUSSE_TO_MICROTESLA;
      Z = (float)read16(HM5883_DOZ0) / HM5883_Gauss_LSB_Z * SENSORS_GAUSSE_TO_MICROTESLA;
    }
    void getDegrees(){
      getRaw();
      float heading = atan2(Y,X);
      float decilinationAngle = 0.22;
      heading += decilinationAngle;
      if (heading < 0) heading += 2*PI;
      if (heading > 2*PI) heading -= 2*PI;
      float headingDegrees = heading * 180/M_PI;
      angle = (int)headingDegrees;
    }
    void display() {
      Serial.print("XMag: "); Serial.print(X); Serial.print("---");
      Serial.print("YMag: "); Serial.print(Y); Serial.print("---");
      Serial.print("ZMag: "); Serial.print(Z); Serial.println();
    }
} RGB_HM5883;

