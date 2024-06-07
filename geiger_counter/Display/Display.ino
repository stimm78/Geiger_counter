#include <LiquidCrystal.h>

//Update frequency, in ms
const int period = 1000;
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
unsigned long t0 = micros();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Connect pulse output of counter to the pin with the interrupt (will likely want either 2 or 3)
  attachInterrupt(digitalPinToInterrupt(2), increment, RISING);

  lcd.begin(16, 2);
  lcd.print("CountsPerMinute:");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(period);
  lcd.setCursor(0, 1);
  lcd.print(0);
}

void increment() {
  unsigned long e = micros();
  unsigned long t = e - t0;
  t0 = e;
  byte buf[4] = {t & 0xFF , (t >> 8) & 0xFF, (t >> 16) & 0xFF, (t >> 24) & 0xFF};
  Serial.write(buf, 4);
}