#define CLOCK_PIN 13

void setup() {
  // put your setup code here, to run once:
  pinMode(CLOCK_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(CLOCK_PIN, HIGH);
  delay(10);
  digitalWrite(CLOCK_PIN, LOW);
  delay(10);
}
