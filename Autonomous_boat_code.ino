#include <Servo.h> //Include the Servo library
const int trigPinFront=12; //Trig is plugged into 12, Yellow Wires
const int echopinFront=13; //Echo is plugged into 13, Yellow Wires
const int trigPinLeft=10;  //TrigLeft is plugged into 10, Blue Wires
const int echopinLeft=11;  //EchoLeft is plugged into 11, Blue Wires
const int servoPin=9;      //Servo is plugged into 9
const int trigPinRight=3;  //TrigRight is plugged into 3
const int echopinRight=2;  //EchoRight is plugged into 2 
Servo myServo;
Servo motor;
const int motorPin = 6;
const int restPoint = 1480; 

void setup()
{
  Serial.begin(9600);
  pinMode(trigPinFront,OUTPUT);
  pinMode(echopinFront,INPUT);
  pinMode(trigPinLeft,OUTPUT);
  pinMode(echopinLeft,INPUT);
  pinMode(trigPinRight,OUTPUT);
  pinMode(echopinRight,INPUT);
  myServo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
  motor.attach(motorPin);
  Serial.println("we have just started the awesome programme!");
} 

void loop()
{
  int shortestFront = checkDistance(trigPinFront, echopinFront);
  int shortestLeft = checkDistance(trigPinLeft, echopinLeft);
  int shortestRight = checkDistance(trigPinRight, echopinRight);

  Serial.println(shortestFront);
  Serial.println(shortestLeft);
  Serial.println(shortestRight);
  Serial.println("This is an awesome comparision to see who's wrong!");

  gostraight(); 
  
  if (shortestFront < 100){   //if the front distance of the cleaner to the wall is less than 100cm, it will execute the loop
    stopBoat(); //The boat will stop to reset rudder to 90 deg position and to shut down motor
  }
    //The boat will then proceed to turn. It is now deciding whether to turn left or turn right.
    else if (shortestRight < 40){   //If right distance of cleaner to wall is less than 40cm, it will turn left to avoid the wall.
        turnLeftBig();        //turning left in progress (may need to tweak delays to find the best time needed to acheive a 90deg turn
        Serial.println("shortest distance: " + shortestRight);
       } 
       
    else if (shortestLeft <40){   //If left distance of cleaner to wall is less than 40cm, it will turn right to avoid the wall.
      turnRightBig();
   } 

    else {                       //If in a hypothtetical situation where the boat is in the middle of no where with no SIDE walls as a guide to where it should turn, the boat will auto turn right in hopes to avoid thefront wall
      turnRightBig();
    }
}   

int checkDistance(int trig, int echo)
{
  long duration;
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  return (duration*0.034)/2;
  
}

void turnRightBig(){                  //Code for a Boat to make 90deg turn towards Right
  int i;
  myServo.write(135);               //May need to tweak angle 
  for (i = 0; i <100; i++) {      //accelerating the brushless motor
    motor.write(restPoint+i);         
    delay(5);                        //WARNING, this is untested and may be dangerous, because the motor will be accelerating faster than usual from 15 milliseconds to 5 milliseconds
  } 
  delay(4500); //time for the motor to run at 1550         //Can be tweaked to find the best time needed to make the boat turn exactly at 90deg
  motor.write(restPoint);
   Serial.println("turnRight");
}

void turnLeftBig(){                  //Code for a Boat to make 90deg turn towards Left
  int i;
  myServo.write(45);               //May need to tweak angle 
  for (i = 0; i <100; i++) {      //accelerating the brushless motor
    motor.write(restPoint+i);         
    delay(5);          //WARNING, this is untested and may be dangerous, because the motor will be accelerating faster than usual from 15 milliseconds to 5 milliseconds
  } 
  delay(4500); //time for the motor to run at 1550   //Can be tweaked to find the best time needed to make the boat turn exactly at 90deg
  motor.write(restPoint);
  Serial.println("turnLeft");
}


void gostraight(){
  int i;
  myServo.write(90);
  for (i = 0; i <100; i++) {      //accelerating the brushless motor
    motor.write(restPoint+i);         
    delay(5);
    } 
  delay(200); //time for the motor to run at 1550  //YeeKian cutdown from 5000 milliseconds to 200 milliseconds, so that the arduino can escape quickly from the code when needed to such as when it is quick approaching a wall.
  motor.write(restPoint);
  Serial.println("goStraight");
}

void stopBoat() {    //Instantaneous temporary stop of the boat in order to reset motor speed and rudder of boat (to be done before turns)
  motor.write (0); //assumption that restpoint = 0 means motor not moving
  myServo.write(90);
  Serial.println("stopBoat");
   }







/*


hello to future users! Please remember to check everything in your machines. Firstly, check whether dumb grammatical errors have  been destroyed. <<< This is especially important, just saying...
 */




