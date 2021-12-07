#include <WiiChuck.h>
//Note: values[11]=c button, values[10]=z button
Accessory nunchuck1;

bool cState = LOW;
bool lastCState = LOW;
int midiValue = 60;

int joyYState = 127;
int lastJoyYState = 127;
int joyXState = 127;
int lastJoyXState = 127;

int rollButtonPin = 31;
int pitchButtonPin = 32;

int rollValue = 0;
int pitchValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rollButtonPin, INPUT);
  pinMode(pitchButtonPin, INPUT);
  nunchuck1.begin();
  nunchuck1.type = NUNCHUCK;
}

void loop() {
  usbMIDI.read();
  nunchuck1.readData();
  checkC();
  checkJoystickY();
  checkJoystickX();
  checkZ();
  setEffects();
}

void checkC() {
  lastCState = cState;
  if (nunchuck1.values[11] == 0) {
    cState = LOW;
  } else if (nunchuck1.values[11] == 255) {
    cState = HIGH;
  }
  if (cState == HIGH and lastCState == LOW) {
    usbMIDI.sendNoteOn(midiValue, 127, 1);
    delay(5);
  } else if (cState == LOW and lastCState == HIGH) {
    usbMIDI.sendNoteOff(midiValue, 0, 1);
    delay(5);
  }
}

void checkZ() {
  if (nunchuck1.values[10] == 255) {
    checkRoll();
    checkPitch();
  }
}

void checkRoll() {
  rollValue = map(nunchuck1.values[4], 68, 180, 0, 127);
  if (rollValue > 127) {
    rollValue = 127;
  } else if (rollValue < 0) {
    rollValue = 0;
  }
  usbMIDI.sendControlChange(1, rollValue, 0);
}

void checkPitch() {
  pitchValue = map(nunchuck1.values[5], 65, 180, 0, 127);
  if (pitchValue > 127) {
    pitchValue = 127;
  } else if (pitchValue < 0) {
    pitchValue = 0;
  }
  usbMIDI.sendControlChange(2, pitchValue, 0);
}

void checkJoystickY() {
  lastJoyYState = joyYState;
  joyYState = nunchuck1.values[1];
  if ((joyYState == 255) and (lastJoyYState != joyYState)) {
    usbMIDI.sendNoteOff(midiValue, 0, 1);
    midiValue = midiValue + 1;
  } else if ((joyYState == 0) and (lastJoyYState != joyYState)) {
    usbMIDI.sendNoteOff(midiValue, 0, 1);
    midiValue = midiValue - 1;
  }
}

void checkJoystickX() {
  lastJoyXState = joyXState;
  joyXState = nunchuck1.values[0];
  if ((joyXState == 255) and (lastJoyXState != joyXState)) {
    usbMIDI.sendNoteOff(midiValue, 0, 1);
    midiValue = midiValue + 12;
  } else if ((joyXState == 0) and (lastJoyXState != joyXState)) {
    usbMIDI.sendNoteOff(midiValue, 0, 1);
    midiValue = midiValue - 12;
  }
}

void setEffects() {
  if (digitalRead(rollButtonPin) == HIGH) {
    usbMIDI.sendControlChange(1, rollValue, 0);
  }
  if (digitalRead(pitchButtonPin) == HIGH) {
    usbMIDI.sendControlChange(2, pitchValue, 0);
  }
}

//values[4] - roll - 69-180
//values[5] - pitch - 65-180
