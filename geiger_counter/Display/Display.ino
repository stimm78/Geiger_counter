#include <LiquidCrystal.h>

const int size = 20;
const int period = 100;
int i = 0;
volatile long current;
long counter[size];
long count;
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const long t0 = micros();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), increment, FALLING);

  lcd.begin(16, 2);
  lcd.print("CountsPerMinute:");

  for(int k = 0; k < size; k++) {
    delay(period);
    cli();
    counter[i] = current;
    current = 0;
    count = 0;
    for(int j = 0; j <= k; j++) {
      count += counter[j];
    }
    //Serial.println(60 * 1000.0 * count / ((k+1) * period));
    lcd.setCursor(0, 1);
    lcd.print(60 * 1000.0 * count / ((k+1) * period));
    i = (i + 1) % size;
    counter[i] = 0;
    sei();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(period);
  cli();
  counter[i] = current;
  current = 0;
  count = 0;
  for(long j: counter) {
    count += j;
  }
  //Serial.println(60 * 1000.0 * count / (size * period));
  lcd.setCursor(0, 1);
  lcd.print(60 * 1000.0 * count / (size * period));
  i = (i + 1) % size;
  counter[i] = 0;
  sei();
}

void increment() {
  current++;
  unsigned long t = (micros() - t0);
  byte buf[4] = {t & 0xFF , (t >> 8) & 0xFF, (t >> 16) & 0xFF, (t >> 24) & 0xFF};
  Serial.write(buf, 4);
  //Serial.println((micros() - t0));
}