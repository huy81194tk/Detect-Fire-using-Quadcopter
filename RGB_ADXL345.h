#define ADXL345_ADDRESS         (0x53)
#define ADXL345_DEVID           (0x00)
#define ADXL345_THRESH_TAP      (0x1D)
#define ADXL345_OFSX            (0x1E)
#define ADXL345_OFSY            (0x1F)
#define ADXL345_OFSZ            (0x20)
#define ADXL345_DUR             (0x21)
#define ADXL345_Latent          (0x22)
#define ADXL345_Window          (0x23)
#define ADXL345_THRESH_ACT      (0x24)
#define ADXL345_THRESH_INACT    (0x25)
#define ADXL345_TIME_INACT      (0x26)
#define ADXL345_ACT_INACT_CTL   (0x27)
#define ADXL345_THRESH_FF       (0x28)
#define ADXL345_TIME_FF         (0x29)
#define ADXL345_TAP_AXES        (0x2A)
#define ADXL345_ACT_TAP_STATUS  (0x2B)
#define ADXL345_BW_RATE         (0x2C)
#define ADXL345_POWER_CTL       (0x2D)
#define ADXL345_INT_ENABLE      (0x2E)
#define ADXL345_INT_MAP         (0x2F)
#define ADXL345_INT_SOURCE      (0x30)
#define ADXL345_DATA_FORMAT     (0x31)
#define ADXL345_DATAX0          (0x32)
#define ADXL345_DATAX1          (0x33)
#define ADXL345_DATAY0          (0x34)
#define ADXL345_DATAY1          (0x35)
#define ADXL345_DATAZ0          (0x36)
#define ADXL345_DATAZ1          (0x37)
#define ADXL345_FIFO_CTL        (0x38)
#define ADXL345_FIFO_STATUS     (0x39)

class RGB_ADXL345 {
  private:
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    float alpha = 0.5;
    int16_t read16(uint8_t reg) {
      return ((int8_t)(WireSoft.ReadData(ADXL345_ADDRESS, reg + 1)) << 8) | WireSoft.ReadData(ADXL345_ADDRESS, reg);
    }
  public:
    float X = 0;
    float Y = 0;
    float Z = 0;
    float roll = 0;
    float pitch = 0;
    void begin(float al = 0.5) {
      while (WireSoft.ReadData(ADXL345_ADDRESS, ADXL345_DEVID) != 0xE5) {
        Serial.println("Not connected to ADXL345...!");
      }
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_DATA_FORMAT, 0x08);
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x08);
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_BW_RATE, 0x08);
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_OFSX, 0x00);
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_OFSY, 0x00);
      WireSoft.WriteData(ADXL345_ADDRESS, ADXL345_OFSZ, 0x00);
      alpha = al;
      Serial.println("Connected to ADXL345...!");
    }
    void Offset(float oX, float oY, float oZ) {
      offsetX = oX;
      offsetY = oY;
      offsetZ = oZ;
    }
    float readX() {
      return ((float)read16(ADXL345_DATAX0) * 0.004) + offsetX;
    }
    float readY() {
      return ((float)read16(ADXL345_DATAY0) * 0.004) + offsetY;
    }
    float readZ() {
      return ((float)read16(ADXL345_DATAZ0) * 0.004) + offsetZ;
    }
    void lowpass() {
      float Xg = readX();
      float Yg = readY();
      float Zg = readZ();
      X = Xg * alpha + (X * (1.0 - alpha));
      Y = Yg * alpha + (Y * (1.0 - alpha));
      Z = Zg * alpha + (Z * (1.0 - alpha));
      roll = (atan2(-Y, Z) * 180.0) / M_PI;
      pitch = (atan2(X, sqrt(Y * Y + Z * Z)) * 180.0) / M_PI;
    }
    void autoOffset() {
      float ox = 0;
      float oy = 0;
      float oz = 0;
      for (int i = 0; i < 1000; i++) {
        lowpass();
        ox = ox + readX();
        oy = oy + readY();
        oz = oz + readZ();
      }
      ox = 0 - ox / 1000;
      oy = 0 - oy / 1000;
      oz = -1.05 - oz / 1000;
      Offset(ox, oy, oz);
    }
    void display() {
      Serial.print("XGyro: "); Serial.print(X); Serial.print("---");
      Serial.print("YGyro: "); Serial.print(Y); Serial.print("---");
      Serial.print("ZGyro: "); Serial.print(Z); Serial.println();
    }
} RGB_ADXL345;

