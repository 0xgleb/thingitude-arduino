#include <TheThingsNetwork.h>

const char *appEui = "70B3D57ED0010CCE";
const char *appKey = "4845AF28AF7353751D0D1C6B60FE73E3";

#define loraSerial Serial1
#define debugSerial Serial

#define freqPlan TTN_FP_EU868

const int loraSpeed = 57600;
const int debugSpeed = 9600;

const int maxMillis = 10000;

const int touchPin = 3;
const int ledPin = 4;

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup() {
  loraSerial.begin(loraSpeed);
  debugSerial.begin(debugSpeed);

  pinMode(LED_BUILTIN, OUTPUT);

  while(!debugSerial && millis() < maxMillis);

  /* debugSerial.println("-- STATUS"); */
  /* ttn.showStatus(); */

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  debugSerial.println("-- JOINED");

  pinMode(touchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

int prevSensorValue = 0;
int sensorValue;
byte data[1];

void loop() {
  /* debugSerial.println("-- LOOP"); */

  sensorValue = digitalRead(touchPin);

  if(sensorValue) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  if(prevSensorValue != sensorValue) {
    data[0] = sensorValue;
    ttn.sendBytes(data, sizeof(data));
  }

  prevSensorValue = sensorValue;
}
