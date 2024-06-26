//Globals
#define STEPPIN 27
#define DIRPIN 14
#define ENAPIN 12
#define BUTTONPIN 25
// #define RELAYPIN 26
#define forward_servo 33
#define rotate_servo 32
#define limitSwitchPin 26

int button_state = 0;
int button_stage = 0;
int limitSwitchState = 0;


#include <ESP32Servo.h>
Servo myservo1;
Servo myservo2;

const int STEPTIME = 100;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(STEPPIN,OUTPUT);
  pinMode(DIRPIN,OUTPUT);
  pinMode(ENAPIN,OUTPUT);

  pinMode(BUTTONPIN,INPUT);
  // pinMode(RELAYPIN,OUTPUT);
  // digitalWrite(RELAYPIN, LOW);

  pinMode(limitSwitchPin, INPUT_PULLUP); // Use the internal pull-up resistor

  digitalWrite(ENAPIN,LOW);//ENABLE IS ACTIVE LOW
  digitalWrite(DIRPIN,HIGH);//SET DIRECTION 

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo1.setPeriodHertz(50);    // standard 50 hz servo
	myservo1.attach(forward_servo, 500, 2400);
  myservo2.setPeriodHertz(50);    // standard 50 hz servo
	myservo2.attach(rotate_servo, 500, 2400);
  
  myservo1.write(0);
  myservo2.write(0);
}

void loop() {

  checkButton();

  // forward(1000);

  // if (Serial.available() > 0) {
  //   // Read the input as a string
  //   String input = Serial.readStringUntil('\n');
  //   input.trim(); // Remove any leading or trailing whitespace
  //   int val = input.toInt();
  //   if(val > 0){
  //     Serial.println("Forward");
  //     forward(val);
  //   } else if(val < 0){
  //     Serial.println("Reverse");
  //     reverse(6000);
  //   }
  // }
  delay(100);
}

void checkButton() {
  int curr_button_state = digitalRead(BUTTONPIN);
  // check if the button is pressed
  if (curr_button_state == LOW && button_state == HIGH) {
    doButtonShit();
  }
  
  // update the lastButtonState with the current state
  button_state = curr_button_state;
}

int tikCounter = 0;
int check_every_x_tiks = 200;

void doButtonShit(){
  switch(button_stage) {
    case 0:
      //turn relay on
      // digitalWrite(RELAYPIN, HIGH);
      myservo1.write(45);
      delay(500);
      myservo2.write(90);

      //turn motor
      break;
    case 1:
        digitalWrite(DIRPIN,HIGH);//SET DIRECTION 
        // tikCounter = 0;
        // limitSwitchState = digitalRead(limitSwitchPin);
        // while(limitSwitchState == HIGH) {
        //   Serial.println("Limit switch not pressed");
        //   for(int i = 0; i < check_every_x_tiks; i++) {
        //       digitalWrite(STEPPIN, HIGH);
        //       delayMicroseconds(50);
        //       digitalWrite(STEPPIN, LOW);
        //       delayMicroseconds(50);
        //   }
        //   tikCounter += check_every_x_tiks;
        //   limitSwitchState = digitalRead(limitSwitchPin);
        // }
        forward(5750);
      //turn motor back
      break;
    case 2:
      // reverse(tikCounter);
      reverse(5750);
      break;
    case 3:
      //resets the relay
      myservo1.write(0);
      myservo2.write(0);
      // digitalWrite(RELAYPIN, LOW);
  }

  if(button_stage == 3) {
    button_stage = 0;
  } else {
    button_stage += 1;
  }
}

void forward(int steps){

  digitalWrite(DIRPIN,HIGH);//SET DIRECTION 

  // digitalWrite(ENAPIN,HIGH);//DISABLE STEPPER
  int tiks = steps;
  for(int i = 0; i < tiks; i++) {
        digitalWrite(STEPPIN, HIGH);
        delayMicroseconds(50);
        digitalWrite(STEPPIN, LOW);
        delayMicroseconds(50);
    }
}

void reverse(int steps){
  digitalWrite(DIRPIN,LOW);//SET DIRECTION 

  // digitalWrite(ENAPIN,HIGH);//DISABLE STEPPER
  int tiks = steps;
  for(int i = 0; i < tiks; i++) {
        digitalWrite(STEPPIN, HIGH);
        delayMicroseconds(50);
        digitalWrite(STEPPIN, LOW);
        delayMicroseconds(50);
    }
}
