#define CV1 PIN_PB0
#define CV2 PIN_PB1
#define CV3 PIN_PB2
#define CV4 PIN_PB3

int ledPin = PIN_PD7; 
unsigned long next = 0;
byte pwmstate = LOW;
#define STEPSPERSEMI 4

int noteToPWM(const char * note) {
  int duty = 12*STEPSPERSEMI * atoi(note+2);
  switch(note[0]) {
    case 'D':
      duty += 2*STEPSPERSEMI;
      break;
    case 'E':
      duty += 4*STEPSPERSEMI;
      break;
    case 'F':
      duty += 5*STEPSPERSEMI;
      break;
    case 'G':
      duty += 7*STEPSPERSEMI;
      break;
    case 'A':
      duty += 9*STEPSPERSEMI;
      break;
    case 'B':
      duty += 11*STEPSPERSEMI;
      break;
    case 'C':
    default:
      break;   
  }
  if(note[1] == '#') {
    duty += STEPSPERSEMI;
  } else if (note[1] == 'b') {
    duty -= STEPSPERSEMI;
  }
  return duty;
}

void playNote(const char *note, unsigned int duration, unsigned int gap) {
  int duty = noteToPWM(note);
  digitalWrite(ledPin,HIGH); 
  digitalWrite(CV1,HIGH); // GATE
  analogWrite(CV4,duty); // CV
  analogWrite(CV3,duty-(12*STEPSPERSEMI)); // CV3 = -Octv.
  
  delay(duration);
  digitalWrite(ledPin,LOW); 
  digitalWrite(CV1,LOW); // GATE
  delay(gap); 
}

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

}

void loop() {

/* Cmaj Apreggio 
  playNote("C 3",300,100);
  playNote("E 3",300,100);
  playNote("G 3",300,100);
  playNote("C 4",300,100);
  playNote("G 3",300,100);
  playNote("E 3",300,100);
*/

/* Wrench */
#define T_ON 100
#define T_OFF 50

// G|---11-11-9-9-8-8-6-6/9-9-8-8-6-6-4-4/6-6-4-4-1-1/4-4/6-6-4-4/6-6-6-4----

  playNote("F#3",T_ON,T_OFF); //11
  playNote("F#3",T_ON,T_OFF);
  playNote("E 3",T_ON,T_OFF); // 9
  playNote("E 3",T_ON,T_OFF);
  playNote("D#3",T_ON,T_OFF); // 8
  playNote("D#3",T_ON,T_OFF);
  playNote("C#3",T_ON,T_OFF); // 6
  playNote("C#3",T_ON,T_OFF);
  

  playNote("E 3",T_ON,T_OFF); // 9
  playNote("E 3",T_ON,T_OFF);
  playNote("D#3",T_ON,T_OFF); // 8
  playNote("D#3",T_ON,T_OFF);
  playNote("C#3",T_ON,T_OFF); // 6
  playNote("C#3",T_ON,T_OFF);
  playNote("B 2",T_ON,T_OFF); // 4
  playNote("B 2",T_ON,T_OFF);

  playNote("C#3",T_ON,T_OFF); // 6
  playNote("C#3",T_ON,T_OFF);
  playNote("B 2",T_ON,T_OFF); // 4
  playNote("B 2",T_ON,T_OFF);
  playNote("G#2",T_ON,T_OFF); // 1
  playNote("G#2",T_ON,T_OFF);
  playNote("B 2",T_ON,T_OFF); // 4
  playNote("B 2",T_ON,T_OFF);

  playNote("C#3",T_ON,T_OFF); // 6
  playNote("C#3",T_ON,T_OFF);
  playNote("B 2",T_ON,T_OFF); // 4
  playNote("B 2",T_ON,T_OFF);

  playNote("C#3",T_ON,T_OFF); // 6
  playNote("D 3",T_ON,T_OFF); // 6-bend
  playNote("C#3",T_ON,T_OFF); // 6
  playNote("B 2",T_ON,T_OFF); // 4





}
