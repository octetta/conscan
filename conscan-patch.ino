#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

// PORTD = digital pins 0 to 7
// PORTB = digital pins 8 to 13
// PORTC = analog input pins

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define HALL_C_PIN 4

#define ENCODER_AP_PIN 5
#define ENCODER_BP_PIN 6

void setup() {
  Serial.begin(9600);
  
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  
  pinMode(HALL_C_PIN, INPUT);
  pinMode(HALL_C_PIN, INPUT_PULLUP);
  
  pinMode(ENCODER_AP_PIN, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), encoder_isr, CHANGE); // or RISING or FALLING or LOW
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), hall_isr, RISING);
}

#define ENCODER_A_PER_CIRCLE (1024)
#define ENCODER_A_MAX (ENCODER_A_PER_CIRCLE * 2)
#define ENCODER_A_MASK (ENCODER_A_MAX-1)
#define ENCODER_A_MARK (0)
#define ENCODER_A_HALF (ENCODER_A_MAX/2)
#define ENCODER_A_1OF4 (ENCODER_A_HALF/2)

#define ENCODER_B_MASK 16 // (1<<(ENCODER_B_PIN-1)) // input encoder B bitmask

long encoder_interrupt_count = 0;
long now_encoder_count = 0;
long mark = ENCODER_A_MARK;
long half = ENCODER_A_HALF;

bool hall_seen = false;
bool encoder_dir = false;

byte state;

/*
  1 = D0
  2 = D1
  4 = D2
  8 = D3
 16 = D4
 32 = D5
 64 = D6 
 */

void encoder_isr() {
  state = PIND;
  
  if (state & 4) {
    encoder_dir = state & ENCODER_B_MASK; // this should tell us the direction we're turning
  }
  
  if (no_hall_seen) return;

  encoder_interrupt_count++;
  
  encoder_interrupt_count &= ENCODER_A_MASK;
  
  if (encoder_interrupt_count == half) {
    SET(PORTD, ENCODER_AP_PIN);
  } else if (encoder_interrupt_count == mark) {
    CLR(PORTD, ENCODER_AP_PIN);
  }
}

void hall_isr() {
  mark = encoder_interrupt_count & ENCODER_A_MASK;
  half = (mark + ENCODER_A_HALF) & ENCODER_A_MASK;
  no_hall_seen = true;
}

#define CW "CW"
#define CCW "CCW"

void loop() {
  char running = '!';
  char *direction = CW;
  unsigned long new_mark, new_half;
  now_encoder_count = encoder_interrupt_count & ENCODER_A_MASK;
  new_mark = mark;
  new_half = half;
  if (no_hall_seen) running = '?';
  if (encoder_dir) direction = CCW;
  Serial.print(direction);
  Serial.print(running);
  Serial.print(" encA:");
  Serial.print(now_encoder_count);
  Serial.print(" hes:");
  Serial.print(new_mark);
  Serial.print(" half:");
  Serial.print(new_half);
  Serial.print(" count:");
  Serial.println((long)(new_half-new_mark));
  delay(250);
}
