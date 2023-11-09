#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <Wire.h>
MPU6050 mpu;
SoftwareSerial mySerial(3, 2); // RX, TX
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
    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_4G)) {
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
        if (i++ > 10) {
            Serial.println("Failed to init MPU6050");
            break;
        }
    }
    initSimThing();
    etPhoneHome("01", "7038250271", "Hello World");
    mpu.calibrateGyro();
    checkGyroSettings();
}
void checkGyroSettings() {
    Serial.println();

    Serial.print(" * Sleep Mode:        ");
    Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

    Serial.print(" * Clock Source:      ");
    switch (mpu.getClockSource()) {
    case MPU6050_CLOCK_KEEP_RESET:
        Serial.println("Stops the clock and keeps the timing generator in reset");
        break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ:
        Serial.println("PLL with external 19.2MHz reference");
        break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ:
        Serial.println("PLL with external 32.768kHz reference");
        break;
    case MPU6050_CLOCK_PLL_ZGYRO:
        Serial.println("PLL with Z axis gyroscope reference");
        break;
    case MPU6050_CLOCK_PLL_YGYRO:
        Serial.println("PLL with Y axis gyroscope reference");
        break;
    case MPU6050_CLOCK_PLL_XGYRO:
        Serial.println("PLL with X axis gyroscope reference");
        break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:
        Serial.println("Internal 8MHz oscillator");
        break;
    }

    Serial.print(" * Gyroscope:         ");
    switch (mpu.getScale()) {
    case MPU6050_SCALE_2000DPS:
        Serial.println("2000 dps");
        break;
    case MPU6050_SCALE_1000DPS:
        Serial.println("1000 dps");
        break;
    case MPU6050_SCALE_500DPS:
        Serial.println("500 dps");
        break;
    case MPU6050_SCALE_250DPS:
        Serial.println("250 dps");
        break;
    }

    Serial.print(" * Gyroscope offsets: ");
    Serial.print(mpu.getGyroOffsetX());
    Serial.print(" / ");
    Serial.print(mpu.getGyroOffsetY());
    Serial.print(" / ");
    Serial.println(mpu.getGyroOffsetZ());
}

void loop() {
    while (1) {
        Serial.println("ur mom");
        delay(1000);
        Vector normGyro = mpu.readNormalizeGyro();
        Vector normAccel = mpu.readNormalizeAccel();
        Serial.print(" Xgyro = ");
        Serial.print(normGyro.XAxis);
        Serial.print(" Ygyro = ");
        Serial.print(normGyro.YAxis);
        Serial.print(" Zgyro = ");
        Serial.println(normGyro.ZAxis);
        Serial.print(" Xaccel = ");
        Serial.print(normAccel.XAxis);
        Serial.print(" Yaccel = ");
        Serial.print(normAccel.YAxis);
        Serial.print(" Zaccel = ");
        Serial.println(normAccel.ZAxis);
    }
}
