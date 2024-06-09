#include <LiquidCrystal.h>

//Update frequency, in ms
const int period = 1000;
//Number of events considered
const int size = 20;
float times[size];
float currentSize = 0;
int i = 0;
float tot = 0;
float tNew = 0;
String out = "";
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
  lcd.print("0.00");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(period);
  if (tot > 0 && currentSize > 0) {
    if((micros() - t0) / 1000000.0 > 2 * tot / currentSize) {
      tot -= times[i];
      times[i] = 0;
      currentSize--;
      if (currentSize <= 0) {
        currentSize = 0;
      }
    }
  }
  if(tot > 0) {
    out = String(currentSize / tot);
  }
  else {
    out = "0.00";
  }
  for(int i = out.length(); i <= 16; i++){
    out += " ";
  }
  lcd.setCursor(0, 1);
  lcd.print(out);
}

void increment() {
  unsigned long e = micros();
  unsigned long t = e - t0;
  t0 = e;
  byte buf[4] = {t & 0xFF , (t >> 8) & 0xFF, (t >> 16) & 0xFF, (t >> 24) & 0xFF};
  Serial.write(buf, 4);
  tNew = t / 1000000.0;
  tot += tNew - times[i];
  times[i] = tNew;
  i = (i + 1) % size;
  currentSize++;
  if (currentSize >= size) {
    currentSize = size;
  }
}