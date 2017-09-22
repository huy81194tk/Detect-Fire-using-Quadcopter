#define SCL 30
#define SDA 29
#define NOP __asm__ __volatile__ ("nop\n\t")
class WireSoft
{
  private:
    void delay_Wire(void)
    {
      delayMicroseconds(1);
    }
    unsigned char Read_SCL(void)
    {
      digitalWrite(SCL, HIGH);
    }
    unsigned char Read_SDA(void)
    {
      pinMode(SDA, INPUT);
      return digitalRead(SDA);
    }
    void Clear_SCL(void)
    {
      digitalWrite(SCL, LOW);
    }
    void Clear_SDA(void)
    {
      pinMode(SDA, OUTPUT);
      digitalWrite(SDA, LOW);
    }
  public:
    void begin(void)
    {
      pinMode(SCL, OUTPUT);
      pinMode(SDA, OUTPUT);
      digitalWrite(SCL, HIGH);
      digitalWrite(SDA, HIGH);
    }

    void Start(void)
    {
      digitalWrite(SCL, HIGH);
      digitalWrite(SDA, HIGH);
      Read_SCL();
      Read_SDA();
      delay_Wire();
      Clear_SDA();
      delay_Wire();
      Clear_SCL();
    }
    void Stop(void)
    {
      Read_SCL();
      Clear_SDA();
      delay_Wire();
      Read_SCL();
      delay_Wire();
      pinMode(SDA, OUTPUT);
      digitalWrite(SCL, HIGH);
      digitalWrite(SDA, HIGH);
    }

    void Writebit(unsigned char bit)
    {
      if (bit)
        Read_SDA();
      else
        Clear_SDA();
      delay_Wire();
      Read_SCL();
      delay_Wire();
      Clear_SCL();
    }
    unsigned char Readbit(void)
    {
      unsigned char dbit;
      Read_SDA();
      delay_Wire();
      Read_SCL();
      dbit = Read_SDA();
      delay_Wire();
      Clear_SCL();
      return dbit;
    }

    void Writebyte(unsigned char Data)
    {
      unsigned char nBit;
      for (nBit = 0; nBit < 8; nBit++)
      {
        Writebit((Data & 0x80) != 0);
        Data = Data << 1;
      }
      Readbit();
    }
    unsigned char Readbyte()
    {
      unsigned char Buff = 0;
      unsigned char nBit;
      for (nBit = 0; nBit < 8; nBit++)
      {
        Buff = (Buff << 1) | Readbit();
      }
      return Buff;
    }

    void WriteData(unsigned char DevideAddr, unsigned char Register, unsigned char Data)
    {
      delay_Wire();
      Start();
      Writebyte(DevideAddr << 1);
      Writebyte(Register);
      Writebyte(Data);
      Stop();
    }

    unsigned char ReadData( unsigned char DevideAddr, unsigned char Register)
    {
      unsigned char Buff;
      delay_Wire();
      Start();
      Writebyte(DevideAddr << 1);
      Writebyte(Register);
      Stop();
      delay_Wire();
      Start();
      delay_Wire();
      Writebyte((DevideAddr << 1) | 1);
      Buff = Readbyte();
      Writebit(0x01);
      Stop();
      return Buff;
    }
} WireSoft;


