#include <WiiChuck.h>
//Note: values[11]=c button, values[10]=z button
Accessory nunchuck1;

bool cState = LOW;
bool lastCState = LOW;
int midiValue = 60;
int lastMidiValue = 60;

int joyYState = 127;
int lastJoyYState = 127;
int joyXState = 127;
int lastJoyXState = 127;

int rollButtonPin = 31;
int pitchButtonPin = 32;

int xValueSwitch = 30;
int xValue = 12;

int rollValue = 0;
int rollStep = 0;
int rollCount = 0;
int rollTotal = 0;

int pitchValue = 0;
int pitchStep = 0;
int pitchCount = 0;
int pitchTotal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rollButtonPin, INPUT);
  pinMode(pitchButtonPin, INPUT);
  pinMode(xValueSwitch, INPUT);
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

void checkC() { //Play notes if C is held down
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

void checkZ() { //checks if z button is held down, and calls functions to change effects
  if (nunchuck1.values[10] == 255) {
    checkRoll();
    checkPitch();
  }
}

void checkRoll() {
  rollStep = map(nunchuck1.values[4], 68, 180, 0, 127); //converts tilt data to midi range
  if (rollStep > 127) {
    rollStep = 127;
  } else if (rollStep < 0) {
    rollStep = 0;
  }
  rollTotal = rollTotal + rollStep;
  rollCount = rollCount + 1;
  if (rollCount >= 10) { //uses an average of ten for smoothness
    rollValue = (rollTotal / 10);
    rollCount = 0;
    rollTotal = 0;
    usbMIDI.sendControlChange(1, rollValue, 0);
  }
}

void checkPitch() { //identical to checkRoll, just for the other tilt direction
  pitchStep = map(nunchuck1.values[5], 65, 180, 0, 127);
  if (pitchStep > 127) {
    pitchStep = 127;
  } else if (pitchStep < 0) {
    pitchStep = 0;
  }
  pitchTotal = pitchTotal + pitchStep;
  pitchCount = pitchCount + 1;
  if (pitchCount >= 10) {
    pitchValue = (pitchTotal / 10);
    pitchCount = 0;
    pitchTotal = 0;
    usbMIDI.sendControlChange(2, pitchValue, 0);
  }
}

void checkJoystickY() {
  lastJoyYState = joyYState;
  joyYState = nunchuck1.values[1];
  if ((joyYState == 255) and (lastJoyYState != joyYState)) {
    lastMidiValue = midiValue; //changes note before turning off previous note for smooth playing
    midiValue = midiValue + 1;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
  } else if ((joyYState == 0) and (lastJoyYState != joyYState)) {
    lastMidiValue = midiValue;
    midiValue = midiValue - 1;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
  }
}

void checkJoystickX() {
  checkXValueSwitch(); //Mostly the same as for the y direction, except it also checks the switch to determine how much to change by
  lastJoyXState = joyXState;
  joyXState = nunchuck1.values[0];
  if ((joyXState == 255) and (lastJoyXState != joyXState)) {
    lastMidiValue = midiValue;
    midiValue = midiValue + xValue;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
  } else if ((joyXState == 0) and (lastJoyXState != joyXState)) {
    lastMidiValue = midiValue;
    midiValue = midiValue - xValue;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
  }
}

void checkXValueSwitch() {
  if (digitalRead(xValueSwitch) == HIGH) {
    xValue = 2;
  } else {
    xValue = 12;
  }
}

void setEffects() { //the code for the buttons to set effect parameters. Allows you to send in the control paths one at a time.
  if (digitalRead(rollButtonPin) == HIGH) {
    usbMIDI.sendControlChange(1, rollValue, 0);
  }
  if (digitalRead(pitchButtonPin) == HIGH) {
    usbMIDI.sendControlChange(2, pitchValue, 0);
  }
}

//values[4] - roll - 69-180
//values[5] - pitch - 65-180
