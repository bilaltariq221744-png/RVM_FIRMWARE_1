#include "Esp32Cam_manager.h"

// ==========================================
// STATIC VARIABLES
// ==========================================

PredictionMessage ESP32CamManager::receivedData;

String ESP32CamManager::latestPrediction = "";

bool ESP32CamManager::newPredictionAvailable = false;

bool ESP32CamManager::predictionActive = false;

// ==========================================
// ESP32-CAM MAC ADDRESS
// ==========================================

uint8_t ESP32CamManager::camMac[6] =
{
    0x80,
    0xF3,
    0xDA,
    0x61,
    0x29,
    0xB0
};

// ==========================================
// RECEIVE CALLBACK
// ==========================================

void ESP32CamManager::onDataRecv(
    const uint8_t *mac,
    const uint8_t *incomingData,
    int len
)
{
    // ==========================================
    // CHECK PACKET SIZE
    // ==========================================

    if (len != sizeof(PredictionMessage))
    {
        return;
    }

    // ==========================================
    // IGNORE DATA OUTSIDE ACTIVE CYCLE
    // ==========================================

    if (!predictionActive)
    {
        return;
    }

    // ==========================================
    // COPY RECEIVED DATA
    // ==========================================

    memcpy(
        &receivedData,
        incomingData,
        sizeof(receivedData)
    );

    // ==========================================
    // STORE PREDICTION
    // ==========================================

    latestPrediction = String(
        receivedData.prediction
    );

    latestPrediction.trim();

    latestPrediction.toUpperCase();

    latestPrediction.replace(
        " ",
        "_"
    );

    latestPrediction.replace(
        "-",
        "_"
    );

    // ==========================================
    // NEW DATA AVAILABLE
    // ==========================================

    newPredictionAvailable = true;
}

// ==========================================
// CLEAR PREVIOUS DATA
// ==========================================

void ESP32CamManager::clearPreviousPrediction()
{
    newPredictionAvailable = false;

    latestPrediction = "";

    memset(
        &receivedData,
        0,
        sizeof(receivedData)
    );
}

// ==========================================
// BEGIN
// ==========================================

void ESP32CamManager::begin()
{
    // ==========================================
    // WIFI STATION MODE
    // ==========================================

    WiFi.mode(WIFI_STA);

    Serial.println();

    Serial.print(
        "ESP32-S3 MAC Address: "
    );

    Serial.println(
        WiFi.macAddress()
    );

    // ==========================================
    // ESP-NOW INIT
    // ==========================================

    if (esp_now_init() != ESP_OK)
    {
        Serial.println(
            "ESP-NOW INIT FAILED"
        );

        return;
    }

    // ==========================================
    // REGISTER RECEIVE CALLBACK
    // ==========================================

    esp_now_register_recv_cb(
        onDataRecv
    );

    // ==========================================
    // ADD ESP32-CAM AS PEER
    // ==========================================

    esp_now_peer_info_t peerInfo = {};

    memcpy(
        peerInfo.peer_addr,
        camMac,
        6
    );

    peerInfo.channel = 0;

    peerInfo.encrypt = false;

    // ==========================================
    // ADD PEER
    // ==========================================

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println(
            "FAILED TO ADD ESP32-CAM PEER"
        );
    }
    else
    {
        Serial.println(
            "ESP32-CAM PEER ADDED"
        );
    }

    // ==========================================
    // INITIAL STATE
    // ==========================================

    predictionActive = false;

    clearPreviousPrediction();

    Serial.println(
        "ESP32CAM MANAGER READY"
    );
}

// ==========================================
// START PREDICTION
// ==========================================

void ESP32CamManager::startPrediction()
{
    // ==========================================
    // IMPORTANT:
    // STOP ACCEPTING OLD DATA FIRST
    // ==========================================

    predictionActive = false;

    // ==========================================
    // CLEAR ALL OLD PREDICTIONS
    // ==========================================

    clearPreviousPrediction();

    // ==========================================
    // SMALL DELAY
    // ==========================================

    delay(50);

    // ==========================================
    // SEND START COMMAND
    // ==========================================

    CommandMessage cmd;

    memset(
        &cmd,
        0,
        sizeof(cmd)
    );

    strcpy(
        cmd.command,
        "START"
    );

    esp_err_t result = esp_now_send(
        camMac,
        (uint8_t *)&cmd,
        sizeof(cmd)
    );

    if (result == ESP_OK)
    {
        Serial.println();
        Serial.println(
            "START COMMAND SENT"
        );
    }
    else
    {
        Serial.println();
        Serial.println(
            "FAILED TO SEND START"
        );
    }

    // ==========================================
    // NOW ACCEPT NEW PREDICTIONS
    // ==========================================

    predictionActive = true;
}

// ==========================================
// STOP PREDICTION
// ==========================================

void ESP32CamManager::stopPrediction()
{
    // ==========================================
    // FIRST STOP ACCEPTING PREDICTIONS
    // ==========================================

    predictionActive = false;

    // ==========================================
    // CLEAR ANY UNUSED DATA
    // ==========================================

    newPredictionAvailable = false;

    latestPrediction = "";

    // ==========================================
    // SEND STOP COMMAND
    // ==========================================

    CommandMessage cmd;

    memset(
        &cmd,
        0,
        sizeof(cmd)
    );

    strcpy(
        cmd.command,
        "STOP"
    );

    esp_err_t result = esp_now_send(
        camMac,
        (uint8_t *)&cmd,
        sizeof(cmd)
    );

    if (result == ESP_OK)
    {
        Serial.println();
        Serial.println(
            "STOP COMMAND SENT"
        );
    }
    else
    {
        Serial.println();
        Serial.println(
            "FAILED TO SEND STOP"
        );
    }
}

// ==========================================
// GET FINAL PREDICTION
// ==========================================

BottleType ESP32CamManager::getFinalPrediction()
{
    // ==========================================
    // COUNTERS
    // ==========================================

    int greenCount = 0;

    int whiteCount = 0;

    int unknownCount = 0;

    int predictionsReceived = 0;

    // ==========================================
    // COLLECT EXACTLY 3 NEW PREDICTIONS
    // ==========================================

    while (predictionsReceived < 3)
    {
        // ==========================================
        // CHECK FOR NEW DATA
        // ==========================================

        if (newPredictionAvailable)
        {
            // ==========================================
            // COPY CURRENT PREDICTION
            // ==========================================

            String prediction =
                latestPrediction;

            // ==========================================
            // IMMEDIATELY CLEAR FLAG
            // ==========================================

            newPredictionAvailable = false;

            // ==========================================
            // COUNT THIS PREDICTION
            // ==========================================

            predictionsReceived++;

            Serial.print(
                "Prediction "
            );

            Serial.print(
                predictionsReceived
            );

            Serial.print(
                ": "
            );

            Serial.println(
                prediction
            );

            // ==========================================
            // GREEN
            // ==========================================

            if (
                prediction.indexOf(
                    "GREEN"
                ) != -1
            )
            {
                greenCount++;
            }

            // ==========================================
            // WHITE
            // ==========================================

            else if (
                prediction.indexOf(
                    "WHITE"
                ) != -1
            )
            {
                whiteCount++;
            }

            // ==========================================
            // UNKNOWN
            // ==========================================

            else
            {
                unknownCount++;
            }
        }

        // ==========================================
        // SMALL DELAY
        // ==========================================

        delay(5);
    }

    // ==========================================
    // SHOW COUNTS
    // ==========================================

    Serial.println();
    Serial.println(
        "-----------------------------"
    );

    Serial.print(
        "GREEN: "
    );

    Serial.println(
        greenCount
    );

    Serial.print(
        "WHITE: "
    );

    Serial.println(
        whiteCount
    );

    Serial.print(
        "UNKNOWN: "
    );

    Serial.println(
        unknownCount
    );

    Serial.println(
        "-----------------------------"
    );

    // ==========================================
    // MAJORITY VOTE
    // ==========================================

    if (greenCount >= 2)
    {
        Serial.println(
            "FINAL PREDICTION: GREEN BOTTLE"
        );

        return GREEN_BOTTLE;
    }

    if (whiteCount >= 2)
    {
        Serial.println(
            "FINAL PREDICTION: WHITE BOTTLE"
        );

        return WHITE_BOTTLE;
    }

    // ==========================================
    // NO MAJORITY
    // ==========================================

    Serial.println(
        "FINAL PREDICTION: UNKNOWN"
    );

    return UNKNOWN_BOTTLE;
}