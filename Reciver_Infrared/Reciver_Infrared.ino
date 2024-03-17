#include <ESP32Servo.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#define servo_door 18
#define servo_window 19


const int IR_RECV_PIN = 15;
int door_sensor = 4;
int photo_sensor = 2;


IRrecv irrecv(IR_RECV_PIN);
decode_results results;
int value_of_ir_remote = 0;
Servo doorServo;
Servo windowServo;
void setup() {
    Serial.begin(9600);
    pinMode(door_sensor,INPUT);
    doorServo.attach(servo_door);
    windowServo.attach(servo_window);
    irrecv.enableIRIn();
    pinMode(23,OUTPUT);
}
float count_time_door = 0;
int is_open = 0;
int is_open_window = 1;
void loop() {
  // Code for door sensor
  int value_door_sensor = analogRead(door_sensor);
  Serial.print("value_door_sensor = ");
  Serial.println(value_door_sensor);
  
  if(value_door_sensor >= 2400){
    is_open = 1;
  }
  if(is_open == 1 && count_time_door <= 3){
    doorServo.write(0);
    count_time_door += 0.1;
  }
  else if(count_time_door >= 3 && value_door_sensor < 2400 ){
    doorServo.write(180);
    is_open = 0;
    count_time_door = 0;
  }
  Serial.println(count_time_door);


  // Window_photo_transistor
  int value_photo_sensor = analogRead(photo_sensor);
  Serial.print("value_photo_sensor = ");
  Serial.println(value_photo_sensor);
  Serial.println(is_open_window);
  if(is_open_window == 1) {
    if(value_photo_sensor >= 2000) {
      windowServo.write(0);
    } else {
      windowServo.write(180);
    }
  } 
  else {
    windowServo.write(0); // Or any other appropriate action if the window is closed
  }


  // remote infrared;
  if (irrecv.decode(&results) && (value_of_ir_remote != 1 || value_of_ir_remote != 2 || value_of_ir_remote != 3)) {
    Serial.print("Received IR data: ");
    Serial.println(results.value);
    value_of_ir_remote = results.value;
    irrecv.resume();
  }
  Serial.print("value_of_ir_remote :");
  Serial.println(value_of_ir_remote);

  // Infrared just one detect
  if(value_of_ir_remote == 1){
    is_open = 1;
    digitalWrite(23,HIGH);
    value_of_ir_remote = 0;
  }
  else if(value_of_ir_remote == 2){
    digitalWrite(23,HIGH);
    if(is_open_window == 0){
      is_open_window = 1;
    }
    else{
      is_open_window = 0;
    }
    value_of_ir_remote = 0;
  }
  else{
    digitalWrite(23,LOW);
  }

  // analogReadSound
  delay(100);
  
}
