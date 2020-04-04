#include <LiquidCrystal.h>

#define D4 12
#define D5 11
#define D6 10
#define D7 9
#define E 7
#define RS 6
#define DIGITAL_SIGNAL 2
#define INTERRUPT_PORT 0

const long MINUTE_IN_MILLIS = 60000;

const int TIMEOUT_IN_MILLIS = 1000;
const int CHANGE_RATE = 9600;
const int RPM_MIN = 500;
const int RPM_MAX = 8000;
const int RPM_STEP = 500;

const byte LCD_COLUMNS_AMOUNT = 16;
const byte LCD_ROWS_AMOUNT = 1;
const byte CURSOR_COLUMN_POSITION = 0;
const byte CURSOR_ROW_POSITION = 0;
const byte CURSOR_RPM_POSITION = 5;

const String spaces = "    ";

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

volatile unsigned long lastSignalTime;
volatile unsigned long timeBeetweenRotates;

unsigned int rpm;

void setup()
{
    Serial.begin(CHANGE_RATE);
    attachInterrupt(INTERRUPT_PORT, sensor, RISING);

    lcd.begin(LCD_COLUMNS_AMOUNT, LCD_ROWS_AMOUNT);
    lcd.setCursor(CURSOR_COLUMN_POSITION, CURSOR_ROW_POSITION);
    lcd.print("RPM: ");

    pinMode(DIGITAL_SIGNAL, INPUT);
}

void sensor()
{
    unsigned long currentTime = millis();
    rpm = (MINUTE_IN_MILLIS / (currentTime - lastSignalTime));
    lastSignalTime = currentTime;
}

void loop()
{
    lcd.setCursor(CURSOR_RPM_POSITION, CURSOR_ROW_POSITION);
    lcd.print(rpm);
    lcd.print(spaces);
    unsigned long currentTime = millis();

    if ((currentTime - lastSignalTime) > TIMEOUT_IN_MILLIS)
    {
        rpm = 0;
    }
}
