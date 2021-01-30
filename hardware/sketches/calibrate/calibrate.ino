#define CV1 PIN_PB0
#define CV2 PIN_PB1
#define CV3 PIN_PB2
#define CV4 PIN_PB3

int ledPin = PIN_PD7; 
unsigned long next = 0;
byte pwmstate = LOW;
byte stepspersemi;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(CV1, OUTPUT);
  pinMode(CV2, OUTPUT);
  pinMode(CV3, OUTPUT);
  pinMode(CV4, OUTPUT);
  TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz (16MHz / 510 with 8 bit mode)
  TCCR2B = TCCR2B & B11111000 | B00000001;
  Serial.begin(115200);
  Serial.println("Starting Calibrate");
  Serial.println("0V on PWM1");
  digitalWrite(CV1,LOW);

/*
  analogWrite(CV2,51); // appx 1V when calibrated to 5v
  Serial.println("3 oct on PWM3");
  analogWrite(CV3,154); // appx 3V when calibrated to 5v.
*/

/* We can't get our supply low enough for 0-3V full range (we are designed for 9V=9octaves (when I have figured out 10bit timer1 config.)
  stepspersemi = 7;
  Serial.println("1V=oct on PWM2");
  analogWrite(CV2,12*7); // 8 bits, 3 octaves = 0..252(=3*12*7) for even spacing
  Serial.println("3V=3oct on PWM3");
  analogWrite(CV3,3*12*7);
*/
  // 0-5V range = 256/(5*12) = 4 steps per semitone.
  stepspersemi = 4;
  Serial.println("1V=oct on PWM2");
  analogWrite(CV2,12*stepspersemi); // 8 bits, 5 octaves = 0..240(=5*12*4) for even spacing
  Serial.println("5V=5oct on PWM3");
  analogWrite(CV3,5*12*stepspersemi);
  
  Serial.println("Scale on PWM4");
  // Connect CV device to PWM1 - tune to eg. 55 or 110 Hz, then connect to CV3 and adjust voltage to 
  // have connected VCO output X*2*2*2*2*2 Hz. CV2 should then give X*2 Hz at the oscillator output.
}

void loop() {
  digitalWrite(ledPin,LOW);
  if(millis() > next) {
    next = millis()+500;
    pwmstate += stepspersemi;
    if (pwmstate >= 252) {
      pwmstate = 0;
    }
    analogWrite(CV4,pwmstate);
    digitalWrite(ledPin,HIGH);
    delay(100);
  }
}
