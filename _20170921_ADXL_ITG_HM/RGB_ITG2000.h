#define ITG2000_ADDRESS           (0x68)
#define ITG2000_WHO_AM_I          (0x00)
#define ITG2000_SMPLRT_DIV        (0x15)
#define ITG2000_DLPF_FS           (0x16)
#define ITG2000_INT_CFG           (0x17)
#define ITG2000_INT_STATUS        (0x1A)
#define ITG2000_TEMP_OUT_H        (0x1B)
#define ITG2000_TEMP_OUT_L        (0x1C)
#define ITG2000_GYRO_XOUT_H       (0x1D)
#define ITG2000_GYRO_XOUT_L       (0x1E)
#define ITG2000_GYRO_YOUT_H       (0x1F)
#define ITG2000_GYRO_YOUT_L       (0x20)
#define ITG2000_GYRO_ZOUT_H       (0x21)
#define ITG2000_GYRO_ZOUT_L       (0x22)
#define ITG2000_PWR_MGM           (0x3E)

class RGB_ITG2000 {
  private:
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    float dt = 0.015;

    unsigned long start, finish, elapsed;
    int16_t read16(uint8_t reg) {
      return ((int16_t)(WireSoft.ReadData(ITG2000_ADDRESS, reg)) << 8) | WireSoft.ReadData(ITG2000_ADDRESS, reg + 1);
    }
  public:
    float X = 0;
    float Y = 0;
    float Z = 0;
    float rX, rY, rZ;
    int rawX, rawY, rawZ;
    void begin() {
      while (WireSoft.ReadData(ITG2000_ADDRESS, ITG2000_WHO_AM_I) != ITG2000_ADDRESS) {
        Serial.println("Not connected to ITG2000...!");
      }
      WireSoft.WriteData(ITG2000_ADDRESS, ITG2000_PWR_MGM, 0x80);
      WireSoft.WriteData(ITG2000_ADDRESS, ITG2000_DLPF_FS, 0x1D);
      WireSoft.WriteData(ITG2000_ADDRESS, ITG2000_SMPLRT_DIV, 0x01);
      WireSoft.WriteData(ITG2000_ADDRESS, ITG2000_PWR_MGM, 0x01);
      Serial.println("Connected to ITG2000...!");
      autoOffset();
    }
    void getRaw() {
      rX = (float)read16(ITG2000_GYRO_XOUT_H) * 0.07;
      rY = (float)read16(ITG2000_GYRO_YOUT_H) * 0.07;
      rZ = (float)read16(ITG2000_GYRO_ZOUT_H) * 0.07;
    }
    void autoOffset() {
      float ox = 0;
      float oy = 0;
      float  oz = 0;
      for (int i = 0; i < 1000; i++) {
        getRaw();
        ox = ox + rX;
        oy = oy + rY;
        oz = oz + rZ;
        delay(1);
      }
      offsetX = ox / 1000.0;
      offsetY = oy / 1000.0;
      offsetZ = oz / 1000.0;
    }
    void updateRaw() {

    }
    void startGyro() {
      start = millis();
    }
    void readGyro() {
      getRaw();
      rawX = rX - offsetX;
      rawY = rY - offsetY;
      rawZ = rZ - offsetZ;
      X = X + (rawX * dt);
      Y = Y + (rawY * dt);
      Z = Z + (rawZ * dt);
    }
    void endGyro() {
      finish = millis();
      elapsed = finish - start;
      dt = elapsed / 1000.0;
      start = elapsed = 0;
    }
    
    void updateT(float minX = -0.01, float maxX = 0.01, float minY = -0.01, float maxY = 0.01, float minZ = -1.051, float maxZ = 1.051) {
      if (RGB_ADXL345.X >= minX && RGB_ADXL345.X <= maxX) X = 0;
      if (RGB_ADXL345.Y >= minY && RGB_ADXL345.Y <= maxY) Y = 0;
      if (RGB_ADXL345.Z <= minZ && RGB_ADXL345.Z >= maxZ) Z = 0;
    }
    void display() {
      Serial.print("XGyro: "); Serial.print(X); Serial.print("---");
      Serial.print("YGyro: "); Serial.print(Y); Serial.print("---");
      Serial.print("ZGyro: "); Serial.print(Z); Serial.println();
    }
} RGB_ITG2000;

