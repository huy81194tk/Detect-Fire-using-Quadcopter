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

SimpleKalmanFilter HM_fil_X(2, 2, 0.01);
SimpleKalmanFilter HM_fil_Y(2, 2, 0.01);
SimpleKalmanFilter HM_fil_Z(2, 2, 0.01);
SimpleKalmanFilter HM_fil_H(2, 2, 0.01);

class RGB_HM5883 {
  private:
    int fixedHeadingDegrees;
    float mgPerdigit;
    int16_t read16(uint8_t reg) {
      return ((int16_t)(WireSoft.ReadData(HM5883_ADDRESS, reg)) << 8) | WireSoft.ReadData(HM5883_ADDRESS, reg + 1);
    }
  public:
    uint32_t UpdateHm = 0;
    float X = 0;
    float Y = 0;
    float Z = 0;
    int previousDegree;
    float angle;
    float oangle = 0;
    float heading;
    float headingnow;
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
      X = (float)HM_fil_X.updateEstimate(read16(HM5883_DOX0)) * mgPerdigit;
      Y = (float)HM_fil_Y.updateEstimate(read16(HM5883_DOY0)) * mgPerdigit;
      Z = (float)HM_fil_Z.updateEstimate(read16(HM5883_DOZ0)) * mgPerdigit;
    }
    void getDegrees() {
      getRaw();
      //heading = atan2(X - 470, Z + 482);
      heading = atan2( Z + 482,X - 470);
      float declinationAngle = 0.22;
      heading += declinationAngle;
      if (heading < 0)
        heading += 2 * PI;

      // Check for wrap due to addition of declination.
      if (heading > 2 * PI)
        heading -= 2 * PI;

      // Convert radians to degrees for readability.
      heading = heading * 180 / M_PI;
      headingnow = heading-oangle;
      //heading = HM_fil_H.updateEstimate(heading);
    }
    void start()
    {
      UpdateHm = millis();
    }
    void Update(uint8_t second)
    {
      if (millis() - UpdateHm >= second * 1000) {
        oangle = heading;
        UpdateHm = millis();
      }
    }
    void autoOffset() {
      float ofangle = 0;
      for (int i = 0; i < 1000; i++) {
        getDegrees();
        ofangle = ofangle + heading;
      }
      oangle = ofangle / 1000;
    }
    void display() {
      Serial.print("XMag: "); Serial.print(X); Serial.print("---");
      Serial.print("YMag: "); Serial.print(Y); Serial.print("---");
      Serial.print("ZMag: "); Serial.print(Z); Serial.println();
    }
    void display_plot() {
      Serial.print(heading); Serial.print(",");
      Serial.print(headingnow);
      Serial.print(",");
    }
} RGB_HM5883;

