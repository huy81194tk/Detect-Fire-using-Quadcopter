#define HM5883_ADDRESS           (0x1E)
#define HM5883_Config_Register_A (0x00)
#define HM5883_Config_Register_B (0x01)
#define HM5883_Mode_Register     (0x02)
#define HM5883_DOX0              (0x03)
#define HM5883_DOX1              (0x04)
#define HM5883_DOY0              (0x05)
#define HM5883_DOY1              (0x06)
#define HM5883_DOZ0              (0x07)
#define HM5883_DOZ1              (0x08)

class RGB_HM5883 {
  private:
    int fixedHeadingDegrees;
    float mgPerdigit;
    
    int16_t read16(uint8_t reg) {
      return ((int16_t)(WireSoft.ReadData(HM5883_ADDRESS, reg)) << 8) | WireSoft.ReadData(HM5883_ADDRESS, reg + 1);
    }
  public:
    float X = 0;
    float Y = 0;
    float Z = 0;
    int previousDegree;
    float angle;
    float oangle = 0;
    float heading;
    void begin() {
      WireSoft.WriteData(HM5883_ADDRESS, HM5883_Config_Register_B, 0b001 << 5);
      mgPerdigit = 0.92f;
      uint8_t value;
      value = WireSoft.ReadData(HM5883_ADDRESS, HM5883_Mode_Register);
      value &= 0b11111100;
      value |= 0b00;
      WireSoft.WriteData(HM5883_ADDRESS, HM5883_Mode_Register, value);

      value = WireSoft.ReadData(HM5883_ADDRESS, HM5883_Config_Register_A);
      value &= 0b11100011;
      value |= (0b101 << 2);
      WireSoft.WriteData(HM5883_ADDRESS, HM5883_Config_Register_A, value);

      value = WireSoft.ReadData(HM5883_ADDRESS, HM5883_Config_Register_A);
      value &= 0b10011111;
      value |= (0b11 << 5);
      WireSoft.WriteData(HM5883_ADDRESS, HM5883_Config_Register_A, value);
    }
    void getRaw() {
      X = (float)read16(HM5883_DOX0) * mgPerdigit;
      Y = (float)read16(HM5883_DOY0) * mgPerdigit;
      Z = (float)read16(HM5883_DOZ0) * mgPerdigit;
    }
    void getDegrees() {
      getRaw();
      heading = atan2(Y, X)-oangle;
    }
    void autoOffset() {
      float ofangle = 0;
      for (int i = 0; i < 1000; i++) {
        getDegrees();
        ofangle = ofangle + heading;
      }
      oangle = ofangle/1000;
    }
    void display() {
      Serial.print("XMag: "); Serial.print(X); Serial.print("---");
      Serial.print("YMag: "); Serial.print(Y); Serial.print("---");
      Serial.print("ZMag: "); Serial.print(Z); Serial.println();
    }
} RGB_HM5883;

