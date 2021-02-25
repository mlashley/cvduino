#include <MIDI.h>
#define CV1 PIN_PB0
#define CV2 PIN_PB1
#define CV3 PIN_PB2
#define CV4 PIN_PB3

// #define STEPSPERSEMI 4  // (When Calibrated for 0-5v volt-per-octave 8 bit)
#define STEPSPERSEMI 16  // (When Calibrated for 0-5v volt-per-octave 10 bit)
// #define STEPSPERSEMI 9  // (When Calibrated for 0-9v volt-per-octave 10 bit)
#define STEPSPEROCTAVE = 12*STEPSPERSEMI


MIDI_CREATE_DEFAULT_INSTANCE();

int ledPin = PIN_PD7;
long count = 0;

int noteToPWM(byte note) {
  // A0 is 21, C2 is 36, C4 is 60... we only have 5 octaves of CV range - so offset to start at C2

  byte mynote = note;
  if(mynote >= 36)
    mynote = mynote-36;
  return mynote*STEPSPERSEMI;
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
  digitalWrite(ledPin,HIGH);
  int duty = noteToPWM(note);
  digitalWrite(CV1,HIGH); // GATE

  // NB FastPWM doesn't have a zero duty-cycle...
  if(duty == 0) {
    digitalWrite(CV2,LOW);
    digitalWrite(CV3,LOW);
  } else {
    // Need to re-write control-registers again potentially clobbered by digitalWrite above.
    TCCR1A = B10100011; // COM1A[1:0] = B10, COM1B[1:0] = B10, xx, WGM11:10 = B11
    TCCR1B = TCCR1B & B11100000 | B00001001; // WGM[13:12] = B01, set timer 1 divisor (CS12:10) to 001 for PWM frequency of 15625.00 Hz (16MHz / 1024 with 10 bit mode fast PWM)

    OCR1A = duty; // CV2
    OCR1B = duty-(12*STEPSPERSEMI) ; // CV3 = -Octv.
  }
/*  analogWrite(CV4,duty); // CV4
  analogWrite(CV3,duty-(12*STEPSPERSEMI)); // CV3 = -Octv.
*/
}
void handleNoteOff(byte channel, byte note, byte velocity)
{
  digitalWrite(ledPin,LOW);
  digitalWrite(CV1,LOW); // GATE
}

void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(CV1, OUTPUT);
  pinMode(CV2, OUTPUT);
  pinMode(CV3, OUTPUT);
  pinMode(CV4, OUTPUT);

  // Timer1 WGM[13:10] = 0111 Fast PWM, , 10-bit.
  // COM1A/B[10] = 10 Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when down counting.
  // CS[12:10] = B001 , clkIO/1 (no prescaling)

  // TCCR1A = COM1A1,A0,COM1B1,B0,x,x,WGM11,10
  // TCCR1B = ICNC1, ICES2,x,WGM13,12,CS12,11,10
  TCCR1A = B10100011; // COM1A[1:0] = B10, COM1B[1:0] = B10, xx, WGM11:10 = B11
  TCCR1B = TCCR1B & B11100000 | B00001001; // WGM[13:12] = B01, set timer 1 divisor (CS12:10) to 001 for PWM frequency of 15625.00 Hz (16MHz / 1024 with 10 bit mode fast PWM)
  TCCR2B = TCCR2B & B11111000 | B00000001; // set timer 2 divisor (CS12:10) to 001 for PWM frequency of 31372.55 Hz (16MHz / 510 with 8 bit mode phase-correct PWM)

  // Selftest
  OCR1A=0xff;
  OCR1B=0x1ff;
  OCR2A=0x7f;


  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
//  Serial.begin(115200);
//  Serial.println("Starting self-test");
}

void loop() {
  MIDI.read();
  count++;
  if(count >= 320000)
    digitalWrite(ledPin,HIGH);
  if(count >= 325000) {
    digitalWrite(ledPin,LOW);
    count=0;
  }
}
