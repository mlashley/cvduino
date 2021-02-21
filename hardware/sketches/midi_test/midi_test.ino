#include <MIDI.h>
#define CV1 PIN_PB0
#define CV2 PIN_PB1
#define CV3 PIN_PB2
#define CV4 PIN_PB3

#define STEPSPERSEMI 4  // (When Calibrated for 0-5v volt-per-octave)
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
  analogWrite(CV4,duty); // CV
  analogWrite(CV3,duty-(12*STEPSPERSEMI)); // CV3 = -Octv.

}
void handleNoteOff(byte channel, byte note, byte velocity)
{
  digitalWrite(ledPin,LOW);
  digitalWrite(CV1,LOW); // GATE
}

void setup() {
  
  pinMode(ledPin, OUTPUT);
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
