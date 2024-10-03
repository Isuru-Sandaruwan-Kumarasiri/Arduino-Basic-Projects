int bluetoothStatePin = 2;  // HC-05 State pin connected to pin 2
int ledPin = 13;            // Default LED pin on most Arduinos

void setup() {
  pinMode(bluetoothStatePin, INPUT);  // Set the HC-05 state pin as input
  pinMode(ledPin, OUTPUT);            // Set the built-in LED pin as output

  // Begin serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  int bluetoothState = digitalRead(bluetoothStatePin);  // Read HC-05 connection state
  
  if (bluetoothState == LOW) {
    // Bluetooth is disconnected, blink the LED
    blinkLed();
  } else {
    // Bluetooth is connected, turn the LED off
    digitalWrite(ledPin, LOW);
  }
}

void blinkLed() {
  digitalWrite(ledPin, HIGH);   // Turn LED on
  delay(500);                   // Wait for half a second
  digitalWrite(ledPin, LOW);    // Turn LED off
  delay(500);                   // Wait for half a second
}
