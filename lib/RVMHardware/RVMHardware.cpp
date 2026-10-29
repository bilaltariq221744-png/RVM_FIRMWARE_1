#include "RVMHardware.h"

// =========================
// CONSTRUCTOR
// =========================
RVMHardware::RVMHardware(
    int conveyor,
    int crusher,
    int s1,
    int s2,
    int t1,
    int e1,
    int t2,
    int e2,
    int ir
)
{
    conveyorPin = conveyor;
    crusherPin = crusher;

    servo1Pin = s1;
    servo2Pin = s2;

    trig1 = t1;
    echo1 = e1;

    trig2 = t2;
    echo2 = e2;

    // =========================
    // IR PIN
    // =========================
    irPin = ir;
}

// =========================
// INIT
// =========================
void RVMHardware::begin()
{
    pinMode(conveyorPin, OUTPUT);
    pinMode(crusherPin, OUTPUT);

    digitalWrite(conveyorPin, LOW);
    digitalWrite(crusherPin, LOW);

    pinMode(trig1, OUTPUT);
    pinMode(echo1, INPUT);

    pinMode(trig2, OUTPUT);
    pinMode(echo2, INPUT);

    // =========================
    // IR SENSOR INIT
    // =========================
    pinMode(irPin, INPUT);

    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);

    servo1.attach(servo1Pin, 500, 2400);
    servo2.attach(servo2Pin, 500, 2400);

    servo1.write(0);
    servo2.write(0);

    servo1Current = 0;
    servo2Current = 0;

    delay(300);

    servo1.detach();
    servo2.detach();
}

// =========================
// CONVEYOR
// =========================
void RVMHardware::Conveyor(bool state)
{
    digitalWrite(conveyorPin, state);
}

void RVMHardware::Conveyor(bool state, unsigned long timeMs)
{
    digitalWrite(conveyorPin, state);

    if (timeMs > 0)
    {
        delay(timeMs);
        digitalWrite(conveyorPin, LOW);
    }
}

// =========================
// CRUSHER
// =========================
void RVMHardware::Crusher(bool state)
{
    digitalWrite(crusherPin, state);
}

void RVMHardware::Crusher(bool state, unsigned long timeMs)
{
    digitalWrite(crusherPin, state);

    if (timeMs > 0)
    {
        delay(timeMs);
        digitalWrite(crusherPin, LOW);
    }
}

// =========================
// SERVO 1 (ANGLE ONLY)
// =========================
void RVMHardware::servo1set(int angle)
{
    servo1.attach(servo1Pin, 500, 2400);

    moveServoSmooth(servo1, servo1Current, angle);

    servo1Current = angle;
}

// =========================
// SERVO 1 (WITH TIME)
// =========================
void RVMHardware::servo1set(int angle, unsigned long timeMs)
{
    servo1.attach(servo1Pin, 500, 2400);

    moveServoSmooth(servo1, servo1Current, angle);

    servo1Current = angle;

    delay(timeMs);
}

// =========================
// SERVO 2 (ANGLE ONLY)
// =========================
void RVMHardware::servo2set(int angle)
{
    servo2.attach(servo2Pin, 500, 2400);

    moveServoSmooth(servo2, servo2Current, angle);

    servo2Current = angle;
}

// =========================
// SERVO 2 (WITH TIME)
// =========================
void RVMHardware::servo2set(int angle, unsigned long timeMs)
{
    servo2.attach(servo2Pin, 500, 2400);

    moveServoSmooth(servo2, servo2Current, angle);

    servo2Current = angle;

    delay(timeMs);
}

// =========================
// SMOOTH MOVEMENT
// =========================
void RVMHardware::moveServoSmooth(Servo &s, int from, int to)
{
    int step = (from < to) ? 1 : -1;

    for (int i = from; i != to; i += step)
    {
        s.write(i);
        delay(10);
    }

    s.write(to);
}

// =========================
// ULTRASONIC 1
// =========================
float RVMHardware::ultrasonic1read()
{
    digitalWrite(trig1, LOW);
    delayMicroseconds(2);

    digitalWrite(trig1, HIGH);
    delayMicroseconds(10);

    digitalWrite(trig1, LOW);

    long duration = pulseIn(echo1, HIGH, 30000);

    return duration * 0.034 / 2;
}

// =========================
// ULTRASONIC 2
// =========================
float RVMHardware::ultrasonic2read()
{
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);

    digitalWrite(trig2, HIGH);
    delayMicroseconds(10);

    digitalWrite(trig2, LOW);

    long duration = pulseIn(echo2, HIGH, 30000);

    return duration * 0.034 / 2;
}

// =========================
// IR SENSOR READ
// =========================
// Returns:
// true  = object detected
// false = no object
// =========================
bool RVMHardware::readIR()
{
    return digitalRead(irPin) == LOW;
}