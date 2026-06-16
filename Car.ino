/*  Code to test the motors on the Elegoo Robot Car 4
    Martin H.
    Southern Illinois University, for Automotive Technologies
    
    Download, disconnect cable, place on floor with room to drive (about 4 feet clearance in all directions)
    Press the mode button, wait 2 seconds for it to begin
*/
#include <Wire.h>
#include <Servo.h>  
Servo myservo; 
// define IO pin
#define PWMA 5    // Controls power to right motor
#define PWMB 6    // Controls power to left motor
#define AIN 7     // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#define BIN 8     // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#define STBY 3    // Place H-Bridge in standby if LOW, Run if HIGH
#define FOREWARD 2
#define RIGHT 3
#define BACKWARD 4
#define LEFT 5
#define MODE_DRIVE 6
#define MODE_AUTO 7
#define modeSwitch 2 // Mode Switch input
#define TRIG 13  
#define ECHO 12 
#define carSpeed 75
float k = 45.0;
int pos = 0;
int x = 0;
int start = 0;
float duration, distance;
int mode = 0;
int turnTime = k / carSpeed * 1000; // ms
// int period = 5000;
unsigned long time_now = 0;
float left_dist = 0;
float right_dist = 0;

void forward()
{
    analogWrite(PWMA, carSpeed);   // Full power on Right
    analogWrite(PWMB, carSpeed);   // Full power on Left 
    digitalWrite(AIN, HIGH);    // Forward direction on Right
    digitalWrite(BIN, HIGH);    // Forward direction on Left

} 
void backward()
{
    analogWrite(PWMA, carSpeed);   // Full power on Right
    analogWrite(PWMB, carSpeed);   // Full power on Left 
    digitalWrite(AIN, LOW);    // Reverse direction on Right
    digitalWrite(BIN, LOW); 
}
void left()
{
    analogWrite(PWMA, carSpeed); 
    analogWrite(PWMB, carSpeed); 
    digitalWrite(BIN, LOW);    // Forward direction
    digitalWrite(AIN, HIGH);    // Forward direction
}
void right()
{
    analogWrite(PWMA, carSpeed); 
    analogWrite(PWMB, carSpeed); 
    digitalWrite(AIN, LOW);    // Backwards direction
    digitalWrite(BIN, HIGH);    // Forward direction
}
void stop()
{
    digitalWrite(PWMA, LOW); 
    analogWrite(PWMB, LOW); 
}

//init the car
void setup() {
  pinMode(PWMA, OUTPUT);     //set IO pin mode OUTPUT
  pinMode(PWMB, OUTPUT);
  pinMode(BIN, OUTPUT);
  pinMode(AIN, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);  //Enable Motors to run
  digitalWrite(PWMA, LOW);  // Fully on 
  Serial.begin(9800);
 // digitalWrite(PWMA, HIGH);  // Fully on

 	pinMode(TRIG, OUTPUT);  
	pinMode(ECHO, INPUT); 

  myservo.attach(10);

  Wire.begin(9);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent);
  myservo.write(90);
}
void setMode(){
  if(x==MODE_DRIVE){
    mode = MODE_DRIVE;
  }
  else if(x==MODE_AUTO){
    mode = MODE_AUTO;
  }
}

//main loop
void driveMode(){
  if(x == FOREWARD && distance > 5){
    forward();
  }
  else if(x == BACKWARD){
    backward();
  }
  else if(x == LEFT){
    left();
  }
  else if(x == RIGHT){
    right();
  }
  else{
    stop();
  }
}
float getdist_left()
{
  Serial.println("LEFT IR");
  myservo.write(175);
  return getdistance();
}
float getdist_right()
{
  Serial.println("RIGHT IR");
  myservo.write(5);
  return getdistance();
}
float getdistance()
{
  digitalWrite(TRIG, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIG, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(TRIG, LOW); 
  duration = pulseIn(ECHO, HIGH);
  distance = (duration*.0343)/2;
  return distance;
}

void autoMode(){
  delay(10);
  distance = getdistance();
  Serial.println(distance);
  
  if(distance > 20.0){
    forward();
    Serial.println("FOREWARD");
    setMode();
    return; 
  }
  else if(distance < 20.0){
    stop();
    left_dist = getdist_left();
    delay(50);
    right_dist = getdist_right();
    delay(50);
    int direction = left_dist < right_dist ? 3 : 1;
    if(direction == 1){
      
      time_now = millis();
      while(millis() < time_now + turnTime){
        Serial.println("right");
        right();
        setMode();
      }
      myservo.write(90);
    }
    else if(direction == 3){
      time_now = millis();
      while(millis() < time_now + turnTime){
        Serial.println("LEFT");
        left();
        setMode();
      }
    }
    myservo.write(90);
  }
  else{
    while(millis() < time_now + 2*turnTime){
      Serial.println("BACK");
      left();
      setMode();
    }
    myservo.write(90);
    return;
  }

}

void loop() {
  setMode();
  Serial.println(getdistance());
  while(start == 0){
    if(digitalRead(modeSwitch) == 0){
      mode = 0;
      start = !start;
    }
  }
  if(mode == MODE_DRIVE){
    driveMode();
  }
  else if(mode == MODE_AUTO){
    autoMode();
  }
  
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
}