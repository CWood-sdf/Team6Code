#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <Wire.h>
struct PVector {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double mag() {
        return sqrt(x * x + y * y + z * z);
    }
};
MPU6050 mpu;
SoftwareSerial mySerial(3, 2); // RX, TX
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
void checkGyroSettings();
void updateSerial() {
    delay(500);
    while (Serial.available()) {
        mySerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
    }
    while (mySerial.available()) {
        Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port
    }
}
void etPhoneHome(char* countrycode, char* phonenumber, char* message) {
    mySerial.println("AT"); // Once the handshake test is successful, it will back to OK
    updateSerial();
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.print("AT+CMGS=\"+"); // change ZZ with country code and xxxxxxxxxxx with phone number to sms
    mySerial.print(countrycode);
    mySerial.print(phonenumber);
    mySerial.println("\"");
    updateSerial();
    mySerial.print(message); // text content
    updateSerial();
    mySerial.write(26);
}
void initSimThing() {
    mySerial.println("AT"); // Once the handshake test is successful, it will back to OK
    updateSerial();
    mySerial.println("AT+CSQ"); // Signal quality test, value range is 0-31 , 31 is the best
    updateSerial();
    mySerial.println("AT+CCID"); // Read SIM information to confirm whether the SIM is plugged
    updateSerial();
    mySerial.println("AT+CREG?"); // Check whether it has registered in the network
    updateSerial();
}
void setup() {
    mySerial.begin(9600);
    Serial.begin(9600);
    Serial.println("Hello World");
    Serial.println("Init MPU");
    int i = 0;
    // initSimThing();
    // etPhoneHome("01", "7038250271", "Hello World");
    // mpu.calibrateGyro();
    if (!accel.begin()) {
        Serial.println("no acc :(");
    } else {
        Serial.println("acc :)");
    }
}

int time = 0;
int sleep = 50;
int lastFallTime = -10000;
int fallBreak = 2000;
const char* message = "fall yeet";
void loop() {
    while (1) {
        // Serial.println("ur mom");
        // delay(1000);
        sensors_event_t event;
        accel.getEvent(&event);
        // Serial.print("X: ");
        // Serial.print(event.acceleration.x);
        // Serial.print("");
        // Serial.print(" Y: ");
        // Serial.print(event.acceleration.y);
        // Serial.print("");
        // Serial.print(" Z: ");
        // Serial.print(event.acceleration.z);
        // Serial.print("");
        // Serial.println("m/s^2 ");
        PVector sdf;
        sdf.x = event.acceleration.x;
        sdf.y = event.acceleration.y;
        sdf.z = event.acceleration.z;
        // Serial.print("mag: ");
        // Serial.println(sdf.mag() - 9.8);
        if (sdf.mag() - 10.0 > 3.0) {
            if (time - lastFallTime > fallBreak) {
                Serial.println(message);
                lastFallTime = time;
            }
        }
        if (abs(sdf.mag() - 9.8) < 1.5 && (abs(sdf.x) > abs(sdf.z) || abs(sdf.y) > abs(sdf.z) || (abs(sdf.z) > 4.5 && sdf.z < 0))) {
            if (time - lastFallTime > fallBreak) {
                Serial.println(message);
                lastFallTime = time;
            }
        }
        delay(sleep);
        time += sleep;
    }
}
