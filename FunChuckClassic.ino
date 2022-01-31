#include <WiiChuck.h>
Accessory nunchuck1;

int lastLeftJoyX = 128;
int leftJoyX = 128;

int lastLeftJoyY = 128;
int leftJoyY = 128;

int lastRightJoyX = 128;
int rightJoyX = 128;

int lastRightJoyY = 128;
int rightJoyY = 128;

void setup() {
  Serial.begin(9600);
  nunchuck1.begin();
  nunchuck1.type = WIICLASSIC;
}

void loop() {
  nunchuck1.readData();
  checkLeftX();
  checkLeftY();
  checkRightX();
  checkRightY();
}

void checkLeftX() {
  leftJoyX = nunchuck1.values[0];
  if (leftJoyX != lastLeftJoyX) {
    Serial.println("Left Joy X");
    Serial.println(leftJoyX);
  }
  lastLeftJoyX = leftJoyX;
}

void checkLeftY() {
  leftJoyY = nunchuck1.values[1];
  if (leftJoyY != lastLeftJoyY) {
    Serial.println("Left Joy Y");
    Serial.println(leftJoyY);
  }
  lastLeftJoyY = leftJoyY;
}

void checkRightX() {
  rightJoyX = nunchuck1.values[2];
  if (rightJoyX != lastRightJoyX) {
    Serial.println("Right Joy X");
    Serial.println(rightJoyX);
  }
  lastRightJoyX = rightJoyX;
}

void checkRightY() {
  rightJoyY = nunchuck1.values[3];
  if (rightJoyY != lastRightJoyY) {
    Serial.println("Right Joy Y");
    Serial.println(rightJoyY);
  }
  lastRightJoyY = rightJoyY;
}

// values[0]=JoyXLeft
