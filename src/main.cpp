#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <WString.h>
#include <Wire.h>

// DEFINES

#define PIN_CYCLIC 13 // oscilloscope connection pins
#define PIN_TASK_K 10
#define PIN_TASK_S 11
#define PIN_TASK_L 12

#define INPUTLEN 3
#define TIMEOUT 10000
#define TIMEOUT_SOFT 1000
#define PIN_SERVO A0
#define IIC_SDA A4
#define IIC_SCL A5

// GLOBAL OBJECT CREATION

// sets the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Initialization

Servo svo;

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

// charbufs to be printed to lcd
const char *str_default = "---";
const char *lcd_mask1 = "ANGLE: ### <   ";
const char *lcd_mask2 = "X##  Y### _....";
char mu = 0b11100100; // Greek Mu character in LCDspeak

char inputStr[(INPUTLEN + 1)];
char usstr[(INPUTLEN + 1)];
char prevstr[(INPUTLEN + 1)];

uint8_t cPtr = 0;      // lcd cursor pointer offset
int16_t tar = 0;       // target motor position, degrees
int16_t last = 0;      // last motor position, degrees
boolean valid = false; // is the angle value string a valid angle

int jdx = 0;            // global simple iterator variable
unsigned long toli = 0; // time OF last interaction, ms

// TASK FUNCTIONS

inline unsigned long tsli() {
    // time since last interaction, ms
    // helper function
    return millis() - toli;
}

inline boolean isNum(char c) { return c >= '0' && c <= '9'; }

inline void scanKeypad() {
    digitalWrite(PIN_TASK_K, HIGH);
    if ((k = kpd.getKey()) != NO_KEY) {
        toli = millis();
        Serial.println(inputStr);
        if (k >= '0' && k <= '9') {
            inputStr[cPtr] = k;
            cPtr++;
        }
        if (cPtr >= INPUTLEN) {
            cPtr = 0x0;
        }
    }
    digitalWrite(PIN_TASK_K, LOW);
}
void servoTask() {
    digitalWrite(PIN_TASK_S, HIGH);
    valid = true;
    for (jdx = 0; jdx < INPUTLEN; ++jdx) {
        if (!isNum(inputStr[jdx])) {
            valid = false;
            break;
        }
    }
    if (valid) {
        // syntax is substring(start index, # chars to read)
        tar = String(inputStr).substring(0, INPUTLEN).toInt() % 180;
        if (tar != last && tsli() >= TIMEOUT_SOFT) {
            last = tar;
            svo.write(tar);
        }
    }
    digitalWrite(PIN_TASK_S, LOW);
}
void updateLCDTask() {
    digitalWrite(PIN_TASK_L, HIGH);
    const uint8_t inputStart = 7;
    static boolean needsUpdate = false;

    if ((needsUpdate = (strcmp(prevstr, inputStr) != 0))) {
        // 1/3: angle:
        strcpy(prevstr, inputStr);
        lcd.setCursor(inputStart, 0);
        lcd.print(inputStr);
        // 2/3: x&y locations
        lcd.setCursor(1, 1);
        lcd.print(cos(DEG_TO_RAD * tar), 1);
        lcd.setCursor(6, 1);
        lcd.print(sin(DEG_TO_RAD * tar), 1);
        // 3/3: microseconds reading
        lcd.setCursor(11, 1);
        lcd.print(svo.readMicroseconds());

        // position the cursor:
        lcd.setCursor(cPtr + inputStart, 0);
        digitalWrite(PIN_TASK_L, LOW);
    }
}

// PROGRAM EXECUTION BEGINS BELOW

void setup() {
    // System Initializations:

    Serial.begin(9600);
    yield();

    pinMode(PIN_TASK_K, OUTPUT);
    pinMode(PIN_TASK_L, OUTPUT);
    pinMode(PIN_TASK_S, OUTPUT);
    pinMode(PIN_CYCLIC, OUTPUT);

    // Object Initializations:

    kpd.setDebounceTime(2);

    svo.attach(PIN_SERVO);
    strcpy(inputStr, str_default);
    svo.write(0); //starts at pos 0

    lcd.init();
    lcd.backlight();
    lcd.blink_on();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(lcd_mask1);
    lcd.setCursor(0, 1);
    lcd.print(lcd_mask2);
    lcd.setCursor(0xA, 1);
    lcd.write(mu);
}

// CYCLIC EXECUTIVE
void loop() {
    static const unsigned long H = 250; // Hyperperiod, ms

    do {

    } while (millis() % H <= 5);
    do {
        servoTask();
    } while (millis() % H <= 6);
    do {
        updateLCDTask();
    } while (millis() % H <= 104);

    // enter slack time:
    do {
        scanKeypad(); // idle task is polling server
    } while (millis() % H > 104);

    // watchdog for system resets every {TIMEOUT} seconds
    if (tsli() > TIMEOUT) {
        strcpy(inputStr, str_default);
        cPtr = 0;
    }
    // oscilloscope timing helper
    digitalWrite(PIN_CYCLIC, !digitalRead(PIN_CYCLIC));
}
