//www.elegoo.com

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;  
int Trig = A5; 

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define carSpeed 250
#define carturnSpeed 150
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
float sensor_speed = 50;
int sensor_step = 20;
int distance = 0;
int sensor_distance = 50;
int sensor_angle = 70;


void forward(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carturnSpeed);
  analogWrite(ENB, carturnSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carturnSpeed);
  analogWrite(ENB, carturnSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
} 

//Ultrasonic distance measurement Sub function
int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
}  

void setup() { 
  myservo.attach(3,700,2400);  // attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
  forward();
}


float scan(int sensor_pos, float min_distance){
    myservo.write(sensor_pos);
    delay(sensor_speed);
    distance = Distance_test();
    if (distance < min_distance){
        min_distance = distance;
    }
    return min_distance;
}

void dir(){
    stop();
    myservo.write(90);
    delay(360);
    int distance_middle = Distance_test();
    if (distance_middle < (sensor_distance/1.5)){
        back();
        delay(420);
        stop();
    }
    
    myservo.write(0);
    delay(360);
    int distance_right = Distance_test();
    if (distance_right < (sensor_distance/1.5)){
      back();
      delay(600);
      stop();
    }

    myservo.write(180);
    delay(360);
    int distance_left = Distance_test();
    if (distance_left < (sensor_distance/1.5)){
      back();
      delay(600);
      stop();
    }
    myservo.write(90);
    delay(360);
    int turn_time = 35;
    if(distance_right > distance_left) {
        while(1==1){
          right();
          delay(turn_time);
          myservo.write(90);
          int distance_mid = Distance_test();
          if (distance_mid > sensor_distance*2){
            break;
          
          }
        }
        
        //right();
        //delay(100);
    }
    else if(distance_right < distance_left) {
      while(1==1){
          left();
          delay(turn_time);
          myservo.write(90);
          int distance_mid = Distance_test();
          if (distance_mid > sensor_distance*2){
            break;
          
          }
        }
        //left();
        //delay(100);
    }
    else{
      while(1==1){
          left();
          delay(turn_time);
          myservo.write(90);
          int distance_mid = Distance_test();
          if (distance_mid > sensor_distance*2){
            break;
          
          }
        }
      
    }
}


void loop() {
    int sensor_pos;
    for (sensor_pos=90-sensor_angle; sensor_pos < 90+sensor_angle; sensor_pos+=sensor_step){  
      myservo.write(sensor_pos);
      delay(sensor_speed);
      distance = Distance_test();
      if (distance <= sensor_distance){
        dir();
      }
      else{
        forward();
      }
    }
    for (sensor_pos=90+sensor_angle; sensor_pos > 90-sensor_angle; sensor_pos-=sensor_step){ 
      myservo.write(sensor_pos);
      delay(sensor_speed);
      distance = Distance_test();
      if (distance <= sensor_distance){
        dir();
      }
      else{
        forward();
      }
    }

}
