#ifndef DWIN_HMI_H
#define DWIN_HMI_H

#include <Arduino.h>

// ======================================================
// PAGE DEFINITIONS
// ======================================================

#define PAGE_START         0
#define PAGE_QR            1
#define PAGE_MAIN          2
#define PAGE_DONATION      3
#define PAGE_ALKHIDMAT     4
#define PAGE_EDHI          5
#define PAGE_AIRUNI        6
#define PAGE_HHRD          7
#define PAGE_THANKYOU      8
#define PAGE_ERROR         9

// ======================================================
// BUTTON DEFINITIONS
// ======================================================

enum DWIN_BUTTON
{
    NO_BUTTON            = 0x00,
    START_BUTTON         = 0x01,
    WITHDRAW_BUTTON      = 0x02,
    ALKHIDMAT_BUTTON     = 0x03,
    EDHI_BUTTON          = 0x04,
    AIRUNI_BUTTON        = 0x05,
    HHRD_BUTTON          = 0x06,
    Thankyou_DONE_BUTTON = 0x07,
    DONATE_BUTTON        = 0x08,
    TRY_AGAIN_BUTTON     = 0x09,
    Cancel_Button        = 0x10,
    Earn_Done_Button      = 0x11
};

// ======================================================
// VP ADDRESSES
// ======================================================

#define VP_BOTTLE     0x1001
#define VP_EARNING    0x1002
#define VP_QR         0x1010

class DWIN_HMI
{
private:

    HardwareSerial* dwin;

    uint8_t rxBuffer[50];
    uint8_t rxIndex;

    DWIN_BUTTON lastButton;

    void parseFrame();

public:

    DWIN_HMI(HardwareSerial &serial);

    void begin(long baud, int rxPin, int txPin);

    void update();

    DWIN_BUTTON getButton();

    const char* buttonToString(DWIN_BUTTON button);

    void pageSwitch(uint8_t page);

    void writeNumber(uint16_t vp, uint16_t value);

    void writeString(uint16_t vp, String text);

    void setData(uint16_t bottles, uint16_t earning);

    void setQR(uint16_t amount);

    void reset();
};

#endif