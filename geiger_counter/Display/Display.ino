#include <LiquidCrystal.h>

//Update frequency, in ms
const int period = 1000;
//Number of events considered
const int size = 20;
float rates[size];
int i = 0;
float cps = 0;
float rNew;
bool isLong = false;
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
unsigned long t0 = micros();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Connect pulse output of counter to the pin with the interrupt (will likely want either 2 or 3)
  attachInterrupt(digitalPinToInterrupt(2), increment, RISING);

  lcd.begin(16, 2);
  lcd.print("CountsPerSecond:");
  lcd.setCursor(0, 1);
  lcd.print(cps / size);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(period);
  if(isLong) {
    cps -= rates[i];
    if(cps <= 0) {
      cps = 0;
    }
    rates[i] = 0;
    i = (i+ 1) % size;
    isLong = false;
  }
  String out = String(cps / size);
  for(int i = out.length(); i <= 16; i++){
    out += " ";
  }
  lcd.setCursor(0, 1);
  lcd.print(out);
  isLong = true;
}

void increment() {
  unsigned long e = micros();
  unsigned long t = e - t0;
  t0 = e;
  byte buf[4] = {t & 0xFF , (t >> 8) & 0xFF, (t >> 16) & 0xFF, (t >> 24) & 0xFF};
  Serial.write(buf, 4);
  rNew = 1000000.0 / t;
  cps += rNew - rates[i];
  if(cps <= 0) {
      cps = 0;
    }
  rates[i] = rNew;
  i = (i + 1) % size;
  isLong = false;
}