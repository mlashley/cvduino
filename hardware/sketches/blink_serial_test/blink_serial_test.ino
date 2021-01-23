// Uncomment to test MIDI, else test serial-print/echo.
// #define TEST_MIDI 1

#ifdef TEST_MIDI
  #include <MIDI.h>
  MIDI_CREATE_DEFAULT_INSTANCE();
#endif

#define CV1 PIN_PB0
#define CV2 PIN_PB1
#define CV3 PIN_PB2
#define CV4 PIN_PB3

int ledPin = PIN_PD7; 
boolean newData = false;
const byte numChars = 32;
char c[numChars];
unsigned long next = 0;
byte pwmstate = LOW;

void recvWithnewline() {
    static byte i = 0;
    char newline = '\n';
    char rc;
   
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != newline) {
            c[i] = rc;
            i++;
            if (i >= numChars) {
                i = numChars - 1;
            }
        }
        else {
            c[i] = '\0'; // terminate the string
            i = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("Serial echo: ");
        Serial.println(c);
        newData = false;
    }
}

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
  pinMode(CV1, OUTPUT);
  pinMode(CV2, OUTPUT);
  pinMode(CV3, OUTPUT);
  pinMode(CV4, OUTPUT);

#ifdef TEST_MIDI
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
#endif

  Serial.begin(115200);
  Serial.println("Starting self-test");
}

void loop() {

#ifndef TEST_MIDI
  /* Serial Test */
  if(millis() > next) {
    next = millis()+1000;
    Serial.println("Loop");
    recvWithnewline();
    showNewData();
    digitalWrite(ledPin,HIGH);
    delay(100);
    digitalWrite(ledPin,LOW);

    /* also cycle PWM pins on/off every second */
    if(pwmstate == LOW) {
      pwmstate = HIGH;
    } else {
      pwmstate = LOW;
    }
    digitalWrite(CV1,pwmstate);
    digitalWrite(CV2,pwmstate);
    digitalWrite(CV3,pwmstate);
    digitalWrite(CV4,pwmstate);
  }
#else
  MIDI.read();
#endif
}
