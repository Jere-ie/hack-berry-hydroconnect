// Library includes
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// defines of pin numbers
const int trigPin = 9;
const int echoPin = 10;
const int PhPin = A4;
const int TemperaturePin = 8;

//Instances
OneWire oneWireObjecto(TemperaturePin);
DallasTemperature Temperature_Sensor(&oneWireObjecto);

// defines variables
int BH1750address = 0x23; //setting i2c address for lux meter
byte buff[2];
int Temperature = 0;
uint8_t timer = 0;

void setup() {
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  OCR4A = 35535;            // compare match register 16MHz/256/2Hz
  TCCR4B |= (1 << WGM12);   // CTC mode
  TCCR4B |= (1 << CS12) | (1 << CS10);    // 1024 prescaler 
  TIMSK4 |= (1 << OCIE1A);  // enable timer compare interrupt
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  OCR3A = 65535;            // compare match register 16MHz/256/2Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12) | (1 << CS10);    // 1024 prescaler 
  TIMSK3 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

  Wire.begin();
  Temperature_Sensor.begin();
  Serial.begin(9600); // Starts the serial communication
  Serial1.begin(9600);
}

void loop() {
  BH1750_Init(BH1750address);
  delay(200);
  BH1750_Read(BH1750address);
}

/** Timer0 interrupt*/
ISR(TIMER4_COMPA_vect){
  // prints the measured data in the monitor
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.print("    ");
  Serial.print("Lux: ");
  Serial.println(buff[1]+(buff[0]<<8), DEC);
  Serial1.print("{'temperature':'");
  Serial1.print(Temperature);
  Serial1.print("','type':'");
  Serial1.print("sensedData");
  Serial1.print("','lux':'");
  Serial1.print(buff[1]+(buff[0]<<8));
  Serial1.print("'}");
}

ISR(TIMER3_COMPA_vect){
  if(1 <= timer)
  {
    // Measure Temperature
    Temperature = Temperature_Sensor.getTempCByIndex(0);
    timer = 0;
    Temperature_Sensor.requestTemperatures();
  }
  timer ++;
}

int BH1750_Read(int address) //
{
  int i=0;
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  return i;
}
void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx resolution 120ms
  Wire.endTransmission();
}
