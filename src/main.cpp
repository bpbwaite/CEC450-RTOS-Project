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
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Initialization

Servo svo; //motor 
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
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //keypad setup, defined as kpd object
char k = NO_KEY; //null key
char valstr[4]; // array of characters to be printed to lcd
char prevstr[4]; // compare 
int idx = 0;
int tar = 0;
int last = 0; // last motor pos in degrees before buffer


unsigned long tsli = 0; // time since last interaction, ms
unsigned long toli = 0; // time OF last interaction, ms

void setup() {
    // put your setup code here, to run once:
    // System Initializations:

    Serial.begin(9600);
    yield();

    pinMode(PIN_OSC, OUTPUT);

    // Object Initializations:
    kpd.setDebounceTime(10);
    //LCD Start initialization
    lcd.init();
    lcd.backlight();
    lcd.blink_on();
    lcd.clear();

    svo.attach(PIN_SERVO);
    strcpy(valstr, "000");
    svo.write(0); //starts at pos 0
}

void loop() {
    // put your main code here, to run repeatedly:

    if ((tsli = millis() - toli) > TIMEOUT) {
        strcpy(valstr, "000");
        idx = 0;
    }

    if ((k = kpd.getKey()) != NO_KEY) {
        toli = millis();
        Serial.println(valstr);
        if (k >= '0' && k <= '9') {
            valstr[idx] = k;
            idx++;
        }
        if (idx >= 3) {
            idx = 0;
        }
    }

    //move cursor to second column
    //Serial.println(k);
    if (strcmp(prevstr, valstr)) {
        strcpy(prevstr, valstr);
        lcd.setCursor(0, 0);
        for (int c = 0; c < 3; ++c) {
            if (valstr[c] != '\0')
                lcd.write(valstr[c]);
        }
        lcd.setCursor(idx, 1);
    }
    tar = String(valstr).substring(0, 2 + 1).toInt() % 180;
    if (tar != last && tsli > 10) {
        last = tar; 
        svo.write(tar);
        Serial.println(tar);
    }
   

    svo.readMicroseconds();

    // for graphing
    delay(1);
    digitalWrite(PIN_OSC, !digitalRead(PIN_OSC));
}
