#include <TheThingsNetwork.h>

const char *appEui = "70B3D57ED0010CCE";
const char *appKey = "1566D81EA970E674EFF8766BBBAA3705";

#define loraSerial Serial1
#define debugSerial Serial

#define freqPlan TTN_FP_EU868

#define loraSpeed 57600
#define debugSpeed 9600

#define maxMillis 10000

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void message(const uint8_t* payload, size_t length, port_t port) {
  debugSerial.println("-- MESSAGE");

  if(length == 1) {
    if(!payload[0]) {
      debugSerial.println("LED: off");
      digitalWrite(LED_BUILTIN, LOW);
    } else if(payload[0] == 1) {
      debugSerial.println("LED: on");
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      debugSerial.println("Error: invalid message!");
    }
    return;
  }

  debugSerial.println("Error: only accepting messages with length 1!");
}

void setup() {
  loraSerial.begin(loraSpeed);
  debugSerial.begin(debugSpeed);

  pinMode(LED_BUILTIN, OUTPUT);

  while(!debugSerial && millis() < maxMillis);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.onMessage(message);
}

void loop() {
  debugSerial.println("-- LOOP");

  byte data[1];

  data[0] = digitalRead(LED_BUILTIN);

  ttn.sendBytes(data, sizeof(data));

  delay(10000);
}
