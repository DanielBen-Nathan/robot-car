#include <Servo.h> //servo library
Servo myservo; // create servo object to control servo

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
float sensor_delay = 50;
int sensor_step = 20;
int distance = 0;
int sensor_distance = 50;
int sensor_angle = 70;
float reverse_sensor_ratio = 0.45;
float forward_sensor_ratio = 1.75;
int reverse_time_forward = 300;
int reverse_time_side = 350;

void forward()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Forward");
}

void back()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Back");
}

void left()
{
    analogWrite(ENA, carturnSpeed);
    analogWrite(ENB, carturnSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Left");
}

void right()
{
    analogWrite(ENA, carturnSpeed);
    analogWrite(ENB, carturnSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Right");
}

void stop()
{
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
    Serial.println("Stop!");
}

//Ultrasonic distance measurement Sub function
int Distance_test()
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(Trig, LOW);
    float Fdistance = pulseIn(Echo, HIGH);
    Fdistance = Fdistance / 58;
    return (int)Fdistance;
}

void setup()
{
    myservo.attach(3, 700, 2400); // attach servo on pin 3 to servo object
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

int check_reverse(int sensor_dir, int back_time)
{
    myservo.write(sensor_dir);
    delay(360);
    int distance = Distance_test();
    if (distance < (sensor_distance * reverse_sensor_ratio)) {
        back();
        delay(back_time);
        stop();
    }
    return distance;
}

void turn(int turn_time, void (*direction)())
{
    for (;;) {
        direction();
        delay(turn_time);
        int distance_mid = Distance_test();
        if (distance_mid > sensor_distance * forward_sensor_ratio) {
            break;
        }
    }
}

void choose_dir()
{
    stop();
    check_reverse(90, reverse_time_forward);

    int distance_right = check_reverse(0, reverse_time_side);
    int distance_left = check_reverse(180, reverse_time_side);

    myservo.write(90);
    delay(360);
    int turn_time = 35;
    if (distance_right > distance_left) {
        turn(turn_time, &right);
    }
    else if (distance_right < distance_left) {
        turn(turn_time, &left);
    }
    else {
        turn(turn_time, &left);
    }
}

void scan(int sensor_pos)
{
    myservo.write(sensor_pos);
    delay(sensor_delay);
    distance = Distance_test();
    if (distance <= sensor_distance) {
        choose_dir();
    }
    else {
        forward();
    }
}

void loop()
{
    int sensor_pos;
    for (sensor_pos = 90 - sensor_angle; sensor_pos < 90 + sensor_angle; sensor_pos += sensor_step) {
        scan(sensor_pos);
    }
    for (sensor_pos = 90 + sensor_angle; sensor_pos > 90 - sensor_angle; sensor_pos -= sensor_step) {
        scan(sensor_pos);
    }
}
