const int threshold = 1;
const int size = 5;
const int period = 100;
int i = 0;
volatile long current;
long counter[size];
long count;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), increment, FALLING);
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
  Serial.println(1000.0 * count / (size * period));
  i = (i + 1) % size;
  counter[i] = 0;
  sei();
}

void increment() {
  current++;
}
