#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <Wire.h>
struct Vector3D {
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
        String serialIn = Serial.readString();
        Serial.print("Serial in: ");
        Serial.println(serialIn);
        mySerial.println(serialIn); // Forward what Serial received to Software Serial Port
    }
    int i = 2000;
    while (mySerial.available() || i < 100) {
        String serialOut = mySerial.readString();
        Serial.println(serialOut); // Forward what Software Serial received to Serial Port
        i++;
        delay(50);
    }
}
void etPhoneHome(char* countrycode, char* phonenumber, char* message) {
    mySerial.println("AT"); // Once the handshake test is successful, it will back to OK
    updateSerial();
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    // updateSerial();
    mySerial.print("AT+CMGS=\"+"); // change ZZ with country code and xxxxxxxxxxx with phone number to sms
    mySerial.print(countrycode);
    mySerial.print(phonenumber);
    mySerial.println("\"");
    // updateSerial();
    mySerial.print(message); // text content
    // updateSerial();
    mySerial.write(26);
    mySerial.println();
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
    // Serial.println("Hello World");
    // Serial.println("Init MPU");
    delay(5000);
    int i = 0;
    Serial.println("Start msg");
    // etPhoneHome("01", "7038250271", "Hsdfsdello World");
    Serial.println("Msg sent");
    while (1) {
        updateSerial();
    }
    // mySerial.println("AT"); // Once the handshake test is successful, it will back to OK
    // delay(2000);
    // updateSerial();
    // mySerial.println("AT+CSQ"); // Signal quality test, value range is 0-31 , 31 is the best
    // updateSerial();
    // mySerial.println("AT+CCID"); // Read SIM information to confirm whether the SIM is plugged
    // updateSerial();
    // mySerial.println("AT+CREG?"); // Check whether it has registered in the network
    // updateSerial();
    // etPhoneHome("01", "7038250271", "Hello World");
    // mpu.calibrateGyro();
    if (!accel.begin()) {
        Serial.println("no acc :(");
    } else {
        Serial.println("acc :)");
    }
    sensors_event_t event;
    accel.getEvent(&event);
    Vector3D sdf;
    sdf.x = event.acceleration.x;
    sdf.y = -event.acceleration.z;
    sdf.z = -event.acceleration.y;
    if (abs(10 - sdf.z) > 2) {
        Serial.println("Incorrect acc orientation");
        Serial.print("X: ");
        Serial.print(sdf.x);
        Serial.print(", Y: ");
        Serial.print(sdf.y);
        Serial.print(", Z: ");
        Serial.println(sdf.z);
    }
}

long time = 0;
int sleep = 50;
long lastFallTime = -10000;
long fallBreak = 4000;
const char* message = "u good bro?";
void loop() {
    // if (time % 10000 == 0) {
    //     updateSerial();
    //     Serial.println("update bro");
    // }
    // if (time == 30000) {
    //     Serial.println("sdfaksd");
    //     etPhoneHome("01", "7038250271", "Hello World");
    // }

    // Serial.println("ur mom");
    // delay(1000);
    sensors_event_t event;
    accel.getEvent(&event);
    // Serial.print("X: ");
    // Serial.println("m/s^2 ");
    Vector3D acc;
    acc.x = event.acceleration.x;
    acc.y = -event.acceleration.z;
    acc.z = -event.acceleration.y;
    // Serial.print(sdf.x);
    // Serial.print(", ");
    // // Serial.print(" Y: ");
    // Serial.print(sdf.y);
    // Serial.print(", ");
    // // Serial.print(" Z: ");
    // Serial.print(sdf.z);
    // Serial.print(",\n");
    // Serial.print("mag: ");
    // Serial.println(sdf.mag() - 9.8);
    if (acc.mag() - 10.0 > 3.0) {
        if (time - lastFallTime > fallBreak) {
            Serial.println(message);
            lastFallTime = time;
        }
    }
    if (abs(acc.mag() - 9.8) < 1.5 && (abs(acc.x) > abs(acc.z) || abs(acc.y) > abs(acc.z) || (abs(acc.z) > 4.5 && acc.z < 0))) {
        if (time - lastFallTime > fallBreak) {
            Serial.println(message);
            lastFallTime = time;
        }
    }
    delay(sleep);
    time += sleep;
}
