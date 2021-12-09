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
  rollStep = map(nunchuck1.values[4], 68, 180, 0, 127);
  if (rollStep > 127) {
    rollStep = 127;
  } else if (rollStep < 0) {
    rollStep = 0;
  }
  rollTotal = rollTotal + rollStep;
  rollCount = rollCount + 1;
  if (rollCount >= 10) {
    rollValue = (rollTotal / 10);
    rollCount = 0;
    rollTotal = 0;
    usbMIDI.sendControlChange(1, rollValue, 0);
  }
}

void checkPitch() {
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
    lastMidiValue = midiValue;
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
  lastJoyXState = joyXState;
  joyXState = nunchuck1.values[0];
  if ((joyXState == 255) and (lastJoyXState != joyXState)) {
    lastMidiValue = midiValue;
    midiValue = midiValue + 12;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
  } else if ((joyXState == 0) and (lastJoyXState != joyXState)) {
    lastMidiValue = midiValue;
    midiValue = midiValue - 12;
    if (cState == HIGH) {
      usbMIDI.sendNoteOn(midiValue, 127, 1);
      usbMIDI.sendNoteOff(lastMidiValue, 0, 1);
    }
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
