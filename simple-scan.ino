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
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), encoder_isr, RISING); // or RISING or FALLING or LOW
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), hall_isr, RISING);
}

volatile byte dstate = 0;
volatile byte encoder_dir = 0;
volatile int encoder_count = 0;
volatile int hall_encoder_count = 0;
volatile byte reset_count = 0;
/*
  1 = D0
  2 = D1
  4 = D2 / ENC-A
  8 = D3 / HES-C
 16 = D4 / ENC-B
 32 = D5 / A'
 64 = D6 / B'
 */

void encoder_isr() {
  dstate = PIND;
  encoder_dir = dstate & 16; // this should tell us the direction we're turning
  if (reset_count) {
    encoder_count = 0;
    reset_count = 0;
  } else {
    encoder_count++;
  }
}

void hall_isr() {
  hall_encoder_count = encoder_count;
  reset_count = 1;
}

void loop() {
  Serial.print(encoder_dir);
  Serial.print(" ");
  Serial.print(encoder_count);
  Serial.print(" ");
  Serial.print(hall_encoder_count);
  Serial.println(" ");
  delay(250);
}
