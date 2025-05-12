/*
  Motion tracker

  This example creates a Bluetooth® Low Energy peripheral with the standard accelerometer service and
  linear X, Y & Z acceleration characteristics.

  The circuit:
  - Arduino Nano 33 IoT.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <String.h>
float initialx=0;
float initialy=0;
float initialz=0;

String address="";
float currentx=0;
float currenty=0;
float currentz=0;

float x, y, z;
long previousMillis = 0; 

void accelerometer();
// Bluetooth® Low Energy accelerometer service
BLEService acceleroService("ABCD");

// Bluetooth® Low Energy accelerometer linear X Characteristic
BLEFloatCharacteristic linearCharX("AAAA",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic linearCharY("BBBB",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic linearCharZ("CCCC",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

void setup() {
  Serial.begin(9600);    // initialize serial communication
  //while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  digitalWrite(LED_BUILTIN, LOW);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  /* Set a local name for the Bluetooth® Low Energy device
     This name will appear in advertising packets
     and can be used by remote devices to identify this Bluetooth® Low Energy device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setDeviceName("Smart_Helmet_1");
  BLE.setLocalName("Smart_Helmet_User_1");
  BLE.setAdvertisedService(acceleroService); // add the service UUID
  acceleroService.addCharacteristic(linearCharX); // add the accelerometer linear X characteristic
  acceleroService.addCharacteristic(linearCharY); // add the accelerometer linear Y characteristic
  acceleroService.addCharacteristic(linearCharZ); // add the accelerometer linear Z characteristic
  BLE.addService(acceleroService); // Add the accelerometer service
  linearCharX.writeValue(currentx); // set initial value for this characteristic
  linearCharX.writeValue(currentx); // set initial value for this characteristic
  linearCharX.writeValue(currentx); // set initial value for this characteristic

  address = BLE.address();

  Serial.print("Local address is: ");
  Serial.println(address);

  /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
     advertising packets and will be visible to remote Bluetooth® Low Energy central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  //Serial.println("Bluetooth® device active, waiting for connections...");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {

  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // while the central is connected:
      while (central.connected()) {
        long currentMillis = millis();
        // if 100 ms have passed, check the linear acceleration in X direction:
        if (currentMillis - previousMillis >= 100) {
        previousMillis = currentMillis;
          if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(x, y, z);
            currentx = x;
            currenty = y;
            currentz = z;
          }
          if ((initialx != currentx) || (initialy != currenty) || (initialz != currentz))
          {
          Serial.println(address);
          Serial.print(currentx);
          Serial.println(" X");
          Serial.print(currenty);
          Serial.println(" Y");
          Serial.print(currentz);
          Serial.println(" Z");
          linearCharX.writeValue(currentx);  // and update the battery level characteristic
          linearCharY.writeValue(currenty);  // and update the battery level characteristic
          linearCharZ.writeValue(currentz);  // and update the battery level characteristic
          }
  }

      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
}