#ifndef RVM_HARDWARE_H
#define RVM_HARDWARE_H

#include <Arduino.h>
#include <ESP32Servo.h>

class RVMHardware
{
private:

    int conveyorPin;
    int crusherPin;

    int servo1Pin;
    int servo2Pin;

    int trig1, echo1;
    int trig2, echo2;

    // =========================
    // IR SENSOR PIN
    // =========================
    int irPin;

    Servo servo1;
    Servo servo2;

    int servo1Current = 0;
    int servo2Current = 0;

    void moveServoSmooth(Servo &s, int from, int to);

public:

    RVMHardware(
        int conveyor,
        int crusher,
        int s1,
        int s2,
        int t1,
        int e1,
        int t2,
        int e2,
        int ir
    );

    void begin();

    // =========================
    // CONVEYOR
    // =========================
    void Conveyor(bool state);
    void Conveyor(bool state, unsigned long timeMs);

    // =========================
    // CRUSHER
    // =========================
    void Crusher(bool state);
    void Crusher(bool state, unsigned long timeMs);

    // =========================
    // SERVO 1
    // =========================
    void servo1set(int angle);
    void servo1set(int angle, unsigned long timeMs);

    // =========================
    // SERVO 2
    // =========================
    void servo2set(int angle);
    void servo2set(int angle, unsigned long timeMs);

    // =========================
    // ULTRASONIC
    // =========================
    float ultrasonic1read();
    float ultrasonic2read();

    // =========================
    // IR SENSOR
    // =========================
    bool readIR();
};

#endif