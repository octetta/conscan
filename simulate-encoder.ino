#include <TimerOne.h>

void setup() {
  pinMode(7, OUTPUT);
  Timer1.initialize(9);
  Timer1.attachInterrupt(ping);
  Serial.begin(9600);
}

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

// PORTD = digital pins 0 to 7
// PORTB = digital pins 8 to 13
// PORTC = analog input pins

char state = 0;
volatile unsigned long tc = 0;
volatile unsigned long ttc = 0;

void ping() {
  if (state) {
    CLR(PORTD, 7);
  } else {
    SET(PORTD, 7);
  }
  state = !state;
  tc++;
}
#define PPR1 (1024L)
#define PPR0 (PPR1-1L)

void loop() {
  unsigned long nc = tc;
  unsigned long nnc = (nc/PPR1);
  Serial.print("@ ");
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(nc&PPR0);
  Serial.print(" ");
  Serial.print(nnc);
  Serial.print("/");
  Serial.println(PPR1);
  delay(1000);
}
