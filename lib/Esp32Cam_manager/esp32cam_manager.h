#ifndef ESP32CAM_MANAGER_H
#define ESP32CAM_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// ==========================================
// BOTTLE TYPES
// ==========================================

enum BottleType
{
    GREEN_BOTTLE,
    WHITE_BOTTLE,
    UNKNOWN_BOTTLE,
    NO_NEW_DATA
};

// ==========================================
// DATA RECEIVED FROM ESP32-CAM
// ==========================================

typedef struct
{
    char prediction[20];
    unsigned long timestamp;
} PredictionMessage;

// ==========================================
// COMMAND SENT TO ESP32-CAM
// ==========================================

typedef struct
{
    char command[10];
} CommandMessage;

// ==========================================
// CLASS
// ==========================================

class ESP32CamManager
{
private:

    // ==========================================
    // RECEIVED DATA
    // ==========================================

    static PredictionMessage receivedData;

    static String latestPrediction;

    static bool newPredictionAvailable;

    // ==========================================
    // CURRENT PREDICTION CYCLE
    // ==========================================

    static bool predictionActive;

    // ==========================================
    // ESP32-CAM MAC ADDRESS
    // ==========================================

    static uint8_t camMac[6];

    // ==========================================
    // ESP-NOW RECEIVE CALLBACK
    // ==========================================

    static void onDataRecv(
        const uint8_t *mac,
        const uint8_t *incomingData,
        int len
    );

    // ==========================================
    // CLEAR OLD DATA
    // ==========================================

    static void clearPreviousPrediction();

public:

    // ==========================================
    // INITIALIZATION
    // ==========================================

    void begin();

    // ==========================================
    // GET FINAL PREDICTION
    // COLLECTS 3 NEW PREDICTIONS
    // ==========================================

    BottleType getFinalPrediction();

    // ==========================================
    // START / STOP CAMERA
    // ==========================================

    void startPrediction();

    void stopPrediction();
};

#endif