#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <WString.h>
#include <Wire.h>

#define PIN_OSC 12
#define TIMEOUT 3000
#define PIN_SERVO A0
#define IIC_SDA A4
#define IIC_SCL A5

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo svo;
int pos = 0;

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {{'0', '1', '2', '3'},
                             {'4', '5', '6', '7'},
                             {'8', '9', 'A', 'B'},
                             {'C', 'D', 'E', 'F'}};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char k = NO_KEY;
String valstr;
int idx = 0;
unsigned long tsli = 0; // time since last interaction, ms
unsigned long toli = 0; // time OF last interaction, ms

void setup() {
    // put your setup code here, to run once:

    Serial.begin(9600);
    yield();

    pinMode(PIN_OSC, OUTPUT);

    lcd.init();
    lcd.backlight();

    svo.attach(PIN_SERVO);
}

void loop() {
    // put your main code here, to run repeatedly:

    if ((tsli = millis() - toli) > TIMEOUT) {
        idx = 0;
    }
    if ((k = kpd.getKey()) != NO_KEY) {
        toli = millis();
        if (!idx)
            valstr = "";
        valstr[idx] = k;
        idx++;
    }

    svo.write(valstr.toInt());
    svo.readMicroseconds();

    lcd.clear();
    for (unsigned n = 0; n < valstr.length(); ++n)
        lcd.write(valstr.c_str()[n]);

    // for graphing
    digitalWrite(PIN_OSC, !digitalRead(PIN_OSC));
}
