#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 190 // sets speed of DC motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

void setup() {
  myservo.attach(10);
  myservo.write(115);
  delay(2000);
  initializeDistance();
}

void loop() {
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(40);

  if (distance <= 15) {
    haltMovement();
    delay(100);
    moveBackward();
    delay(300);
    haltMovement();
    delay(200);
    distanceRight = scanRight();
    delay(200);
    distanceLeft = scanLeft();
    delay(200);

    if (distanceRight >= distanceLeft) {
      turnRight();
      haltMovement();
    } else {
      turnLeft();
      haltMovement();
    }
  } else {
    moveForward();
  }
  updateDistance();
}

int scanRight() {
  myservo.write(50);
  delay(500);
  int distance = readDistance();
  delay(100);
  myservo.write(115);
  return distance;
}

int scanLeft() {
  myservo.write(170);
  delay(500);
  int distance = readDistance();
  delay(100);
  myservo.write(115);
  return distance;
}

int readDistance() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void haltMovement() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  if (!goesForward) {
    goesForward = true;
    initiateForwardMotion();
  }
}

void moveBackward() {
  goesForward = false;
  initiateBackwardMotion();
}

void turnRight() {
  initiateRightTurn();
  delay(500);
  initiateForwardMotion();
}

void turnLeft() {
  initiateLeftTurn();
  delay(500);
  initiateForwardMotion();
}

void initiateForwardMotion() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  adjustSpeed();
}

void initiateBackwardMotion() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  adjustSpeed();
}

void initiateRightTurn() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
  initiateForwardMotion();
}

void initiateLeftTurn() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
  initiateForwardMotion();
}

void adjustSpeed() {
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
    setMotorSpeeds();
    delay(5);
  }
}

void setMotorSpeeds() {
  motor1.setSpeed(speedSet);
  motor2.setSpeed(speedSet);
  motor3.setSpeed(speedSet);
  motor4.setSpeed(speedSet);
}

void initializeDistance() {
  distance = readDistance();
  delay(100);
  distance = readDistance();
  delay(100);
  distance = readDistance();
  delay(100);
  distance = readDistance();
  delay(100);
}

void updateDistance() {
  distance = readDistance();
}
