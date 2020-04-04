#include <LiquidCrystal.h>

#define D4 12
#define D5 11
#define D6 10
#define D7 9
#define E 7
#define RS 6
#define DIGITAL_SIGNAL 2
#define INTERRUPT_PORT 0

const int TIMEOUT = 3000;
const int CHANGE_RATE = 9600;

const byte LCD_COLUMNS_AMOUNT = 16;
const byte LCD_ROWS_AMOUNT = 1;

const byte CURSOR_COLUMN_POSITION = 0;
const byte CURSOR_ROW_POSITION = 0;

// initialize library with pin nubers
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

volatile unsigned long lastSignalTime;
volatile unsigned long timeBeetweenRotates;
volatile unsigned long lastDisplayTime;

unsigned int rpm;

String spaces = "    ";

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
    rpm = (60000 / (currentTime - lastSignalTime));
    lastSignalTime = currentTime;
}

void loop()
{
    lcd.setCursor(5, 0);
    lcd.print(rpm);
    lcd.print(spaces);
    lastDisplayTime = millis();

    if ((millis() - lastSignalTime) > 1000) {
        rpm = 0;
    }
}
