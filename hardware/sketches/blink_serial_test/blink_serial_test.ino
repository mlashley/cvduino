int ledPin = PIN_PD7; 
boolean newData = false;
const byte numChars = 32;
char c[numChars];
unsigned long next = 0;

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

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Starting self-test");
}

void loop() {
  if(millis() > next) {
    next = millis()+1000;
    Serial.println("Loop");
    recvWithnewline();
    showNewData();
    digitalWrite(ledPin,HIGH);
    delay(100);
    digitalWrite(ledPin,LOW);
  }
}
