#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <WString.h>
#include <Wire.h>

#define PIN_OSC 12
#define TIMEOUT 6000
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
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char k = NO_KEY;
char valstr[33];
int idx = 0;
unsigned long tsli = 0; // time since last interaction, ms
unsigned long toli = 0; // time OF last interaction, ms

void setup() {
    // put your setup code here, to run once:

    Serial.begin(9600);
    yield();

    pinMode(PIN_OSC, OUTPUT);

    kpd.setDebounceTime(10);

    lcd.init();
    lcd.backlight();

    svo.attach(PIN_SERVO);
    lcd.print("CEC450 RTOS");
    delay(4000);
    lcd.clear();
}

void loop() {
    // put your main code here, to run repeatedly:

    if ((tsli = millis() - toli) > TIMEOUT) {
        idx = 0;
    }

    if ((k = kpd.getKey()) != NO_KEY) {
        toli = millis();
        Serial.println(valstr);
        valstr[idx] = k;
        idx++;
        if (idx >= 33) {
            strcpy(valstr, "                                           ");
            idx = 0;
        }
    }

    //move cursor to second column
    //Serial.println(k);

    lcd.setCursor(0, 0);
    for (int c = 0; c < 16; c++) {
        if (valstr[c] != '\0')
            lcd.write(valstr[c]);
    }
    lcd.setCursor(0, 1);
    for (int c = 16; c < 32; c++) {
        if (valstr[c] != '\0')
            lcd.write(valstr[c]);
    }
    svo.write(0); //starts at pos 0
    svo.write(String(valstr).substring(0, 2).toInt());

    svo.readMicroseconds();

    // for graphing
    digitalWrite(PIN_OSC, !digitalRead(PIN_OSC));
}
