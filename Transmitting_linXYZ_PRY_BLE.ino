#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <String.h>
float initialx=0;
float initialy=0;
float initialz=0;

float initialpitch=0;
float initialroll=0;
float initialyaw=0;

String address="";
float currentx=0;
float currenty=0;
float currentz=0;

float currentpitch=0;
float currentroll=0;
float currentyaw=0;

float x, y, z;
float pitch, roll, yaw;
long previousMillis = 0; 

void accelerometer();
// Bluetooth® Low Energy accelerometer service
BLEService acceleroService("ABCD");

//void gyroscope();
// Bluetooth® Low Energy gyroscope service
//BLEService gyroService("CDEF");

// Bluetooth® Low Energy accelerometer linear X Y Z Characteristic
BLEFloatCharacteristic linearCharX("AAAA",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic linearCharY("BBBB",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic linearCharZ("CCCC",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

// Bluetooth® Low Energy gyroscope angular pitch roll yaw Characteristic
BLEFloatCharacteristic angularCharPitch("DDDD",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic angularCharRoll("EEEE",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic angularCharYaw("FFFF",  // standard 16-bit characteristic UUID
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
  BLE.setDeviceName("Smart_Helmet");
  BLE.setLocalName("Smart_Helmet_1");
  BLE.setAdvertisedService(acceleroService); // add the service UUID
  //BLE.setAdvertisedService(gyroService); // add the service UUID

  acceleroService.addCharacteristic(linearCharX); // add the accelerometer linear X characteristic
  acceleroService.addCharacteristic(linearCharY); // add the accelerometer linear Y characteristic
  acceleroService.addCharacteristic(linearCharZ); // add the accelerometer linear Z characteristic

  acceleroService.addCharacteristic(angularCharPitch); 
  acceleroService.addCharacteristic(angularCharRoll);
  acceleroService.addCharacteristic(angularCharYaw);

  //gyroService.addCharacteristic(angularCharPitch);
  //gyroService.addCharacteristic(angularCharRoll);
  //gyroService.addCharacteristic(angularCharYaw);

  BLE.addService(acceleroService); // Add the accelerometer service
 // BLE.addService(gyroService);

  linearCharX.writeValue(currentx); // set initial value for this characteristic
  linearCharX.writeValue(currentx); // set initial value for this characteristic
  linearCharX.writeValue(currentx); // set initial value for this characteristic

  angularCharPitch.writeValue(currentpitch); // set initial value for this characteristic
  angularCharRoll.writeValue(currentroll); // set initial value for this characteristic
  angularCharYaw.writeValue(currentyaw); // set initial value for this characteristic

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
          if (IMU.accelerationAvailable() || IMU.gyroscopeAvailable()) {
            IMU.readAcceleration(x, y, z);
            IMU.readGyroscope(pitch, roll, yaw);
            currentx = x;
            currenty = y;
            currentz = z;
            
            currentpitch = pitch;
            currentroll = roll;
            currentyaw = yaw;
          }
          if ((initialx != currentx) || (initialy != currenty) || (initialz != currentz) || (initialpitch != currentpitch) || (initialroll != currentroll) || (initialyaw != currentyaw))
          {
          Serial.println(address);
          Serial.print(currentx);
          Serial.println(" X");
          Serial.print(currenty);
          Serial.println(" Y");
          Serial.print(currentz);
          Serial.println(" Z");

          Serial.print(currentpitch);
          Serial.println(" Pitch");
          Serial.print(currentroll);
          Serial.println(" Roll");
          Serial.print(currentyaw);
          Serial.println(" Yaw");

          linearCharX.writeValue(currentx);  // and update the characteristic
          linearCharY.writeValue(currenty);  // and update the characteristic
          linearCharZ.writeValue(currentz);  // and update the characteristic

          angularCharPitch.writeValue(currentpitch);  // and update the characteristic
          angularCharRoll.writeValue(currentroll);  // and update the characteristic
          angularCharYaw.writeValue(currentyaw);  // and update the characteristic
          }
  }

      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
}