# Hardware bringup test-plan

## Visual Inspect / Probe-testing
1. Ground, 5v at IC pins, 10v6 etc. etc.
2. IO header pins -> ICs.

## AVR
(Todo fuses for external crystal? Read https://www.allaboutcircuits.com/projects/atmega328p-fuse-bits-and-an-external-crystal-oscillator/ )

1. Triple-check USB-ASP programmer is jumpered for correct voltage (Vcc=5V)
2. ICSP Chip Ident with avrdude, note that the chip will be running at 1MHz internal oscillator so need 1/4 of that for SCK speed...
`avrdude -c usbasp -n -v -p atmega328p -B 0.25MHz`
3. ICSP Chip Blink PD7 LED.
4. ICSP Simple serial print/echo and use USB-TTL attached to H1 T10/12
5. Re-program fuses to 8MHz internal oscillator - retest
6. Re-program fuses to 16MHz external oscillator - retest
7. MIDI Note-On/Off = LED blink.
  1. With PC - `ttymidi -v -s /dev/ttyUSB0`, `aplaymidi -p ttymidi:1 /home/mlashley/Downloads/bachpiece.mid`
  2. With real device (set baud to 31250)


## Power
1. Apply 12v power and check 5v
2. Apply 12v power and check/adjust 10.6v

## 10v6 switching/PWM
1. Apply simple on-off to PB0..3 and check it follows at CV1..4 @ 10v6(ish)
2. Apply 25,50,75% duty cycle PWM and measure outputs.


# Results/Notes

## Ident:
```bash
mlashley@duality ~/cvduino/hardware (master) $ avrdude -c usbasp -n -v -p atmega328p -B 0.25MHz

avrdude: Version 6.3, compiled on Apr  7 2020 at 23:24:31
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/etc/avrdude.conf"
         User configuration file is "/home/mlashley/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : usb
         Using Programmer              : usbasp
         Setting bit clk period        : 4.0
         AVR Part                      : ATmega328P
         Chip Erase delay              : 9000 us
         PAGEL                         : PD7
         BS2                           : PC2
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    20     4    0 no       1024    4      0  3600  3600 0xff 0xff
           flash         65     6   128    0 yes     32768  128    256  4500  4500 0xff 0xff
           lfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           lock           0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00

         Programmer Type : usbasp
         Description     : USBasp, http://www.fischl.de/usbasp/

avrdude: set SCK frequency to 187500 Hz
avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.01s

avrdude: Device signature = 0x1e950f (probably m328p)
avrdude: safemode: hfuse reads as D9
avrdude: safemode: efuse reads as FF

avrdude: safemode: hfuse reads as D9
avrdude: safemode: efuse reads as FF
avrdude: safemode: Fuses OK (E:FF, H:D9, L:62)

avrdude done.  Thank you.
```

## Blink etc.
Install MiniCore from: https://github.com/MCUdude/MiniCore

Use minicore settings: Board=Atmega328, Programmer="USBasp slow (minicore)", Clock="Internal 1MHz", Bootloader="No Bootloader"
burn bootloader,
shift-upload blinktest.
Success = flashing per second.
```c
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
```
With programmer disconnected and USB-FTDI serial connected, select it as 'Port' in arduino (/dev/ttyUSB0 most likely) and open the serial monitor with 9600 baud.
1. Should ouput 'Loop' every second.
2. Should echo back what we send.
After success here - set Clock="Internal 8MHz", Burn Bootloader, Re-upload, repeat tests.
You can now switch the programmer to regular 'USBasp (minicore)' (non 'slow' version)
avrdude should now run with default baud rate from the cmdline
```bash
mlashley@duality ~/cvduino/hardware (master) $ avrdude -c usbasp -n -v -p atmega328p
...
avrdude: safemode: Fuses OK (E:FD, H:D7, L:E2)
```
And after 16Mhz External
```
avrdude: safemode: Fuses OK (E:FD, H:D7, L:F7)
```

## Power
With 12v applied
1. The 5v line is solid
2. The 10v6 line can be adjusted by the variable-resistor (but is kinda sensitive)

## 10v6 switching/PWM
1. Works
2. Also works

## AVR
Additional calibration and test sketches in `sketches/*`

All worked except MIDI thru the DIN connector - turns out the EasyEDA layout for the conneector is swapped (or assumed to be mounted opposite-side to 'component side')
The 2 pin jumper to breakout to breadboard was an excellent choice here... wired correctly all is good.
Check https://learn.sparkfun.com/tutorials/midi-tutorial/hardware--electronic-implementation for useful pictures.
