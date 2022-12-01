#include <TimerOne.h>

#define THEPIN 9

void setup() {
  Serial.begin(9600);
  pinMode(THEPIN, OUTPUT);
  pinMode(4, OUTPUT);
  //Timer1.initialize(1); // 1000000 Hz
  //Timer1.initialize(2); // 500000 Hz
  //Timer1.initialize(4); // 250000 Hz
  
  //Timer1.initialize(195); // ~5128.2051 Hz ~= 300 RPM / 5 Hz
  //Timer1.initialize(97); // ~10309.2783 Hz ~= 600 RPM / 10 Hz
  Timer1.initialize(48); // ~20833.3333 Hz ~= 1200 RPM / 20 Hz
  //Timer1.initialize(39); // ~25641.0256 Hz ~= 1500 RPM / 25 Hz
  //Timer1.initialize(32); // 31250.0000 Hz ~= 1800 RPM / 30 Hz
  //Timer1.initialize(24); // ~41666.6667 Hz ~= 2400 RPM / 40 Hz
  //Timer1.initialize(19); // ~52631.5789 Hz ~= 3000 RPM / 50 Hz
  
  Timer1.pwm(THEPIN, 512);
  Timer1.attachInterrupt(callback);
  Timer1.start();
  Timer1.setPwmDuty(THEPIN, 512);
}

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

// PORTD = digital pins 0 to 7
// PORTB = digital pins 8 to 13
// PORTC = analog input pins

unsigned long cb0 = 0;
void callback() {
  cb0++;
}
long gcb0 = 0;
long lcb0 = 0;
long dcb0 = 0;
long ldcb0 = 0;
void loop() {
  gcb0 = cb0;
  dcb0 = gcb0 - lcb0;
  Serial.print(gcb0);
  Serial.print(" ");
  Serial.print(dcb0);
  Serial.print(" ");
  Serial.println(dcb0-ldcb0);
  lcb0 = gcb0;
  ldcb0 = dcb0;
  delay(1000);
}
