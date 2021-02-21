#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

int ledPin = PIN_PD7;
long count = 0;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(ledPin,HIGH);
}
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  digitalWrite(ledPin,LOW);
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
  if(count >= 320000) {
    digitalWrite(ledPin,HIGH);
    delay(100);
    digitalWrite(ledPin,LOW);
    count=0;
  }
}
