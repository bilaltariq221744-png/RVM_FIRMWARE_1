#include <Arduino.h>
#include <DWIN_HMI.h>
#include <ESP32Servo.h>
#include <Esp32Cam_manager.h>

// ============================================================
// ESP32-CAM
// ============================================================

ESP32CamManager cam;

// ============================================================
// SERVO PINS
// ============================================================

#define SERVO1_PIN 48
#define SERVO2_PIN 4

#define SERVO_HOME 0
#define SERVO_SORT 70

Servo servo1;
Servo servo2;

// ============================================================
// ULTRASONIC SENSOR
//
// GPIO 12 = TRIG
// GPIO 13 = ECHO
// ============================================================

#define ULTRASONIC_TRIG 12
#define ULTRASONIC_ECHO 13

#define DETECTION_DISTANCE 25

// ============================================================
// BOTTLE REWARD
// ============================================================

#define BOTTLE_REWARD 10

// ============================================================
// BOTTLE COUNT / EARNING
// ============================================================

uint16_t bottleCount = 0;
uint16_t earning = 0;

// ============================================================
// DWIN UART
//
// GPIO 10 = RX
// GPIO 9  = TX
// ============================================================

HardwareSerial DWIN(1);

DWIN_HMI hmi(DWIN);

// ============================================================
// MACHINE STATES
// ============================================================

enum MachineState
{
    IDLE,
    ACCEPTING,
    DETECTING,
    SORTING,
    WAITING_FOR_CLEAR,
    WITHDRAW,
    DONATING
};

MachineState currentState = IDLE;

// ============================================================
// ULTRASONIC FUNCTION
// ============================================================

float getDistance()
{
    digitalWrite(
        ULTRASONIC_TRIG,
        LOW
    );

    delayMicroseconds(2);

    digitalWrite(
        ULTRASONIC_TRIG,
        HIGH
    );

    delayMicroseconds(10);

    digitalWrite(
        ULTRASONIC_TRIG,
        LOW
    );

    long duration = pulseIn(
        ULTRASONIC_ECHO,
        HIGH,
        30000
    );

    if (duration == 0)
    {
        return 999.0;
    }

    float distance =
        duration * 0.0343 / 2.0;

    return distance;
}

// ============================================================
// CHANGE MACHINE STATE
// ============================================================

void changeState(
    MachineState newState
)
{
    currentState = newState;

    Serial.print(
        "MACHINE STATE -> "
    );

    switch (newState)
    {
        case IDLE:
            Serial.println("IDLE");
            break;

        case ACCEPTING:
            Serial.println("ACCEPTING");
            break;

        case DETECTING:
            Serial.println("DETECTING");
            break;

        case SORTING:
            Serial.println("SORTING");
            break;

        case WAITING_FOR_CLEAR:
            Serial.println("WAITING FOR CLEAR");
            break;

        case WITHDRAW:
            Serial.println("WITHDRAW");
            break;

        case DONATING:
            Serial.println("DONATING");
            break;
    }
}

// ============================================================
// RESET TRANSACTION
// ============================================================

void resetTransaction()
{
    bottleCount = 0;
    earning = 0;

    Serial.println();
    Serial.println(
        "TRANSACTION RESET"
    );

    Serial.println(
        "Bottle Count = 0"
    );

    Serial.println(
        "Earning = Rs 0"
    );

    // Update DWIN
    hmi.setData(
        bottleCount,
        earning
    );
}

// ============================================================
// ADD BOTTLE
// ============================================================

void addBottle()
{
    // Increase bottle count
    bottleCount++;

    // Add Rs 10
    earning += BOTTLE_REWARD;

    Serial.println();
    Serial.println(
        "================================"
    );

    Serial.println(
        "BOTTLE ACCEPTED"
    );

    Serial.print(
        "Bottle Count: "
    );

    Serial.println(
        bottleCount
    );

    Serial.print(
        "Earning: Rs "
    );

    Serial.println(
        earning
    );

    Serial.println(
        "================================"
    );

    // Update DWIN
    hmi.setData(
        bottleCount,
        earning
    );
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
    // ========================================================
    // SERIAL
    // ========================================================

    Serial.begin(115200);

    delay(500);

    // ========================================================
    // ULTRASONIC
    // ========================================================

    pinMode(
        ULTRASONIC_TRIG,
        OUTPUT
    );

    pinMode(
        ULTRASONIC_ECHO,
        INPUT
    );

    digitalWrite(
        ULTRASONIC_TRIG,
        LOW
    );

    // ========================================================
    // DWIN
    // ========================================================

    hmi.begin(
        115200,
        10,
        9
    );

    // ========================================================
    // SERVOS
    // ========================================================

    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);

    servo1.attach(
        SERVO1_PIN,
        500,
        2400
    );

    servo2.attach(
        SERVO2_PIN,
        500,
        2400
    );

    // Initial position
    servo1.write(
        SERVO_HOME
    );

    servo2.write(
        SERVO_HOME
    );

    delay(500);

    // ========================================================
    // ESP32-CAM
    // ========================================================

    cam.begin();

    delay(500);

    // ========================================================
    // RESET TRANSACTION
    // ========================================================

    resetTransaction();

    // ========================================================
    // START PAGE
    // ========================================================

    hmi.pageSwitch(
        PAGE_START
    );

    // ========================================================
    // INITIAL STATE
    // ========================================================

    changeState(
        IDLE
    );

    // ========================================================
    // SYSTEM INFORMATION
    // ========================================================

    Serial.println();
    Serial.println(
        "================================"
    );

    Serial.println(
        "RVM SYSTEM READY"
    );

    Serial.println(
        "================================"
    );

    Serial.println(
        "DWIN RX       = GPIO 10"
    );

    Serial.println(
        "DWIN TX       = GPIO 9"
    );

    Serial.println(
        "Servo 1       = GPIO 4"
    );

    Serial.println(
        "Servo 2       = GPIO 48"
    );

    Serial.println(
        "Ultrasonic TRIG = GPIO 12"
    );

    Serial.println(
        "Ultrasonic ECHO = GPIO 13"
    );

    Serial.println(
        "Bottle Reward = Rs 10"
    );

    Serial.println(
        "================================"
    );
}

// ============================================================
// LOOP
// ============================================================

void loop()
{
    // ========================================================
    // UPDATE DWIN
    // ========================================================

    hmi.update();

    // ========================================================
    // READ DWIN BUTTON
    // ========================================================

    DWIN_BUTTON button =
        hmi.getButton();

    // ========================================================
    // BUTTON HANDLING
    // ========================================================

    if (button != NO_BUTTON)
    {
        Serial.print(
            "BUTTON PRESSED: "
        );

        Serial.println(
            hmi.buttonToString(button)
        );

        // ====================================================
        // START BUTTON
        // ====================================================

        if (button == START_BUTTON)
        {
            if (currentState == IDLE)
            {
                Serial.println();
                Serial.println(
                    "START BUTTON PRESSED"
                );

                // --------------------------------------------
                // Start a NEW transaction
                // --------------------------------------------

                resetTransaction();

                // --------------------------------------------
                // Open MAIN page
                // --------------------------------------------

                hmi.pageSwitch(
                    PAGE_MAIN
                );

                delay(100);

                // --------------------------------------------
                // Start accepting bottles
                // --------------------------------------------

                changeState(
                    ACCEPTING
                );

                Serial.println(
                    "READY TO ACCEPT BOTTLES"
                );
            }
            else
            {
                Serial.println(
                    "START IGNORED - MACHINE BUSY"
                );
            }
        }

        // ====================================================
        // WITHDRAW BUTTON
        // ====================================================

        else if (
            button == WITHDRAW_BUTTON
        )
        {
            Serial.println();
            Serial.println(
                "WITHDRAW BUTTON PRESSED"
            );

            // Stop camera if running
            if (
                currentState == DETECTING
            )
            {
                cam.stopPrediction();
            }

            // Stop accepting bottles
            changeState(
                WITHDRAW
            );

            // --------------------------------------------
            // Open QR page
            // --------------------------------------------

            hmi.pageSwitch(
                PAGE_QR
            );

            delay(100);

            // --------------------------------------------
            // Generate QR for ACTUAL earning
            // --------------------------------------------

            Serial.print(
                "QR PAYMENT AMOUNT = Rs "
            );

            Serial.println(
                earning
            );

            hmi.setQR(
                earning
            );
        }

        // ====================================================
        // DONATE BUTTON
        // ====================================================

        else if (
            button == DONATE_BUTTON
        )
        {
            Serial.println();
            Serial.println(
                "DONATE BUTTON PRESSED"
            );

            if (
                currentState == DETECTING
            )
            {
                cam.stopPrediction();
            }

            changeState(
                DONATING
            );

            hmi.pageSwitch(
                PAGE_DONATION
            );
        }

        // ====================================================
        // OTHER BUTTONS
        // ====================================================

        else
        {
            switch (button)
            {
                // ----------------------------------------
                // ALKHIDMAT
                // ----------------------------------------

                case ALKHIDMAT_BUTTON:

                    Serial.println(
                        "ALKHIDMAT selected"
                    );

                    hmi.pageSwitch(
                        PAGE_ALKHIDMAT
                    );

                    break;

                // ----------------------------------------
                // EDHI
                // ----------------------------------------

                case EDHI_BUTTON:

                    Serial.println(
                        "EDHI selected"
                    );

                    hmi.pageSwitch(
                        PAGE_EDHI
                    );

                    break;

                // ----------------------------------------
                // AIR UNIVERSITY
                // ----------------------------------------

                case AIRUNI_BUTTON:

                    Serial.println(
                        "AIR UNIVERSITY selected"
                    );

                    hmi.pageSwitch(
                        PAGE_AIRUNI
                    );

                    break;

                // ----------------------------------------
                // HHRD
                // ----------------------------------------

                case HHRD_BUTTON:

                    Serial.println(
                        "HHRD selected"
                    );

                    hmi.pageSwitch(
                        PAGE_HHRD
                    );

                    break;

                // ----------------------------------------
                // EARNING DONE
                // ----------------------------------------

                case Earn_Done_Button:

                    Serial.println(
                        "EARNING / DONATION COMPLETED"
                    );

                    hmi.pageSwitch(
                        PAGE_THANKYOU
                    );

                    break;

                // ----------------------------------------
                // THANK YOU DONE
                // ----------------------------------------

                case Thankyou_DONE_BUTTON:

                    Serial.println(
                        "TRANSACTION COMPLETED"
                    );

                    Serial.println(
                        "Returning to START"
                    );

                    // Reset transaction
                    resetTransaction();

                    hmi.pageSwitch(
                        PAGE_START
                    );

                    changeState(
                        IDLE
                    );

                    break;

                // ----------------------------------------
                // TRY AGAIN
                // ----------------------------------------

                case TRY_AGAIN_BUTTON:

                    Serial.println(
                        "TRY AGAIN"
                    );

                    hmi.pageSwitch(
                        PAGE_START
                    );

                    resetTransaction();

                    changeState(
                        IDLE
                    );

                    break;

                // ----------------------------------------
                // CANCEL
                // ----------------------------------------

                case Cancel_Button:

                    Serial.println(
                        "CANCEL"
                    );

                    hmi.pageSwitch(
                        PAGE_START
                    );

                    resetTransaction();

                    changeState(
                        IDLE
                    );

                    break;

                default:

                    break;
            }
        }
    }

    // ========================================================
    // MACHINE STATE MACHINE
    // ========================================================

    switch (currentState)
    {

        // ====================================================
        // IDLE
        // ====================================================

        case IDLE:
        {
            // Waiting for START

            break;
        }

        // ====================================================
        // ACCEPTING
        // ====================================================

        case ACCEPTING:
        {
            // -----------------------------------------------
            // Check ultrasonic sensor
            // -----------------------------------------------

            float distance =
                getDistance();

            Serial.print(
                "ACCEPTING - Distance: "
            );

            Serial.print(
                distance
            );

            Serial.println(
                " cm"
            );

            // -----------------------------------------------
            // Bottle detected
            // -----------------------------------------------

            if (
                distance < DETECTION_DISTANCE
            )
            {
                Serial.println();
                Serial.println(
                    "BOTTLE DETECTED"
                );

                changeState(
                    DETECTING
                );
            }

            delay(100);

            break;
        }

        // ====================================================
        // DETECTING
        // ====================================================

        case DETECTING:
        {
            Serial.println();
            Serial.println(
                "================================"
            );

            Serial.println(
                "ESP32-CAM DETECTION STARTED"
            );

            Serial.println(
                "================================"
            );

            // -----------------------------------------------
            // Start camera
            // -----------------------------------------------

            cam.startPrediction();

            delay(500);

            // -----------------------------------------------
            // Get final result
            // -----------------------------------------------

            Serial.println(
                "WAITING FOR 3 PREDICTIONS..."
            );

            BottleType result =
                cam.getFinalPrediction();

            // -----------------------------------------------
            // Stop camera
            // -----------------------------------------------

            cam.stopPrediction();

            delay(100);

            // -----------------------------------------------
            // Move to SORTING
            // -----------------------------------------------

            changeState(
                SORTING
            );

            // =================================================
            // WHITE BOTTLE
            // =================================================

            if (
                result == WHITE_BOTTLE
            )
            {
                Serial.println();
                Serial.println(
                    "FINAL RESULT = WHITE BOTTLE"
                );

                Serial.println(
                    "SERVO 1 -> 70"
                );

                servo1.write(
                    SERVO_SORT
                );

                delay(1000);

                servo1.write(
                    SERVO_HOME
                );

                Serial.println(
                    "SERVO 1 -> 0"
                );

                // --------------------------------------------
                // Add bottle + Rs 10
                // --------------------------------------------

                addBottle();
            }

            // =================================================
            // GREEN BOTTLE
            // =================================================

            else if (
                result == GREEN_BOTTLE
            )
            {
                Serial.println();
                Serial.println(
                    "FINAL RESULT = GREEN BOTTLE"
                );

                Serial.println(
                    "SERVO 2 -> 70"
                );

                servo2.write(
                    SERVO_SORT
                );

                delay(1000);

                servo2.write(
                    SERVO_HOME
                );

                Serial.println(
                    "SERVO 2 -> 0"
                );

                // --------------------------------------------
                // Add bottle + Rs 10
                // --------------------------------------------

                addBottle();
            }

            // =================================================
            // UNKNOWN / INVALID
            // =================================================

            else
            {
                Serial.println();
                Serial.println(
                    "FINAL RESULT = UNKNOWN"
                );

                Serial.println(
                    "Bottle NOT counted"
                );

                Serial.println(
                    "No earning added"
                );
            }

            // -----------------------------------------------
            // Wait for bottle to leave inlet
            // -----------------------------------------------

            changeState(
                WAITING_FOR_CLEAR
            );

            break;
        }

        // ====================================================
        // SORTING
        // ====================================================

        case SORTING:
        {
            // Sorting is currently performed inside
            // DETECTING.

            changeState(
                WAITING_FOR_CLEAR
            );

            break;
        }

        // ====================================================
        // WAITING FOR CLEAR
        // ====================================================

        case WAITING_FOR_CLEAR:
        {
            float distance =
                getDistance();

            Serial.print(
                "WAITING FOR CLEAR - Distance: "
            );

            Serial.print(
                distance
            );

            Serial.println(
                " cm"
            );

            // -----------------------------------------------
            // Bottle has left
            // -----------------------------------------------

            if (
                distance >= DETECTION_DISTANCE
            )
            {
                Serial.println(
                    "INLET CLEAR"
                );

                Serial.println(
                    "READY FOR NEXT BOTTLE"
                );

                changeState(
                    ACCEPTING
                );
            }

            delay(200);

            break;
        }

        // ====================================================
        // WITHDRAW
        // ====================================================

        case WITHDRAW:
        {
            // DWIN handles QR/payment flow.

            break;
        }

        // ====================================================
        // DONATING
        // ====================================================

        case DONATING:
        {
            // DWIN handles donation flow.

            break;
        }
    }
}