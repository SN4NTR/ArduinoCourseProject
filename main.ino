#include <LiquidCrystal.h>

#define D4 12
#define D5 11
#define D6 10
#define D7 9
#define E 7
#define RS 6
#define DIGITAL_SIGNAL 2
#define INTERRUPT_PORT 0

const long REFRESH_RATE = 500000L;
const long MINUTE_IN_MICROS = 60000000L;
const long TIMEOUT_IN_MICROS = 1000000L;

const int CHANGE_RATE = 9600;
const int RPM_MIN = 500;
const int RPM_MAX = 8000;
const int RPM_STEP = 500;

const byte LCD_COLUMNS_AMOUNT = 16;
const byte LCD_ROWS_AMOUNT = 1;
const byte CURSOR_COLUMN_POSITION = 0;
const byte CURSOR_ROW_POSITION = 0;
const byte CURSOR_RPM_POSITION = 5;

const String spaces = "     ";

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

volatile unsigned long lastSignalTime;
volatile unsigned long lastDisplayTime;

unsigned int rpm;

void setup()
{
    Serial.begin(CHANGE_RATE);
    attachInterrupt(INTERRUPT_PORT, sensor, RISING);
    pinMode(DIGITAL_SIGNAL, INPUT);

    lcd.begin(LCD_COLUMNS_AMOUNT, LCD_ROWS_AMOUNT);
    lcd.setCursor(CURSOR_COLUMN_POSITION, CURSOR_ROW_POSITION);
    lcd.print("RPM: ");
}

void sensor()
{
    unsigned long currentTime = micros();
    rpm = (MINUTE_IN_MICROS / (currentTime - lastSignalTime));
    lastSignalTime = currentTime;
}

void loop()
{
    checkRotation();

    if (rpm > RPM_MAX)
    {
        displayNumber(RPM_MAX);
    }
    else
    {
        int rpmWithStep = round(rpm / RPM_STEP) * RPM_STEP;
        displayNumber(rpmWithStep);
    }
}

void displayNumber(int number)
{
    unsigned long currentTime = micros();

    if (currentTime - lastDisplayTime > REFRESH_RATE)
    {
        lcd.setCursor(CURSOR_RPM_POSITION, CURSOR_ROW_POSITION);
        lcd.print(number);
        lcd.print(spaces);

        lastDisplayTime = currentTime;
    }
}

void checkRotation()
{
    unsigned long currentTime = micros();

    if ((currentTime - lastSignalTime) > TIMEOUT_IN_MICROS)
    {
        rpm = 0;
    }
}
