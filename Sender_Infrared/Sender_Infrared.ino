#include <IRremoteESP8266.h>
#include <IRsend.h>

const int IR_LED_PIN = 23; 
const int button_pin = 22;  
const int debounceDelay = 30;  // Adjust this value as needed for your button
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
int buttonState = LOW ;

IRsend irsend(IR_LED_PIN);

byte pin[] = {15, 2, 4, 5, 18, 19, 21}; // Seven segment pin is connected to Digital pin 15, 2, 4, 5, 18, 19, 21 to a, b, c, d, e, f, g
const byte count[10] = {  // Data code for Seven segment 
  //GFEDCBA
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
};

void setup() {
  Serial.begin(9600);
  pinMode(pin[0], OUTPUT);  // Set every pin as output
  pinMode(pin[1], OUTPUT);
  pinMode(pin[2], OUTPUT);
  pinMode(pin[3], OUTPUT);
  pinMode(pin[4], OUTPUT);
  pinMode(pin[5], OUTPUT);
  pinMode(pin[6], OUTPUT);
  pinMode(button_pin, INPUT);
  pinMode(IR_LED_PIN, OUTPUT);
}
int i = 1;
void loop() {
   int reading = digitalRead(button_pin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        i++;
        if(i>3){
          i=1;
        }
        Serial.println(i);
      }
    }
  }
  lastButtonState = reading;
  irsend.sendNEC(i, 32); // Sending NEC signal

  SevenSeg(count[i-1]); // Display the value on the seven-segment display

 
}

void SevenSeg(byte Bit) { // Sending data on each pin
  for (int i = 0; i < 8; i++) {
    digitalWrite(pin[i], bitRead(Bit, i));
  }
}
