#include "DWIN_HMI.h"

// ======================================================

DWIN_HMI::DWIN_HMI(HardwareSerial &serial)
{
    dwin = &serial;

    rxIndex = 0;

    lastButton = NO_BUTTON;
}

// ======================================================

void DWIN_HMI::begin(long baud, int rxPin, int txPin)
{
    dwin->begin(baud, SERIAL_8N1, rxPin, txPin);

    Serial.println("DWIN STARTED");
}

// ======================================================

void DWIN_HMI::update()
{
    while (dwin->available())
    {
        uint8_t data = dwin->read();

        rxBuffer[rxIndex++] = data;

        if(rxIndex >= sizeof(rxBuffer))
        {
            rxIndex = 0;
        }

        if(rxIndex >= 3)
        {
            if(rxBuffer[0] != 0x5A || rxBuffer[1] != 0xA5)
            {
                for(int i=0;i<rxIndex-1;i++)
                {
                    rxBuffer[i] = rxBuffer[i+1];
                }

                rxIndex--;
                continue;
            }

            uint8_t len = rxBuffer[2];

            if(rxIndex >= (len + 3))
            {
                parseFrame();

                rxIndex = 0;
            }
        }
    }
}

// ======================================================

void DWIN_HMI::parseFrame()
{
    if(rxBuffer[3] != 0x83)
    {
        return;
    }

    uint8_t button = rxBuffer[8];

    lastButton = (DWIN_BUTTON)button;

    Serial.print("BUTTON RECEIVED: ");
    Serial.println(buttonToString(lastButton));
}

// ======================================================

DWIN_BUTTON DWIN_HMI::getButton()
{
    DWIN_BUTTON temp = lastButton;

    lastButton = NO_BUTTON;

    return temp;
}

// ======================================================

const char* DWIN_HMI::buttonToString(DWIN_BUTTON button)
{
    switch(button)
    {
        case START_BUTTON:
            return "START_BUTTON";

        case WITHDRAW_BUTTON:
            return "WITHDRAW_BUTTON";

        case ALKHIDMAT_BUTTON:
            return "ALKHIDMAT_BUTTON";

        case EDHI_BUTTON:
            return "EDHI_BUTTON";

        case AIRUNI_BUTTON:
            return "AIRUNI_BUTTON";

        case HHRD_BUTTON:
            return "HHRD_BUTTON";

        case DONATE_BUTTON:
            return "DONATE_BUTTON";

        case Thankyou_DONE_BUTTON:
            return "DONE_BUTTON";

        case TRY_AGAIN_BUTTON:
            return "TRY_AGAIN_BUTTON";

        case Cancel_Button:
            return "CANCEL_BUTTON";
        case Earn_Done_Button:
            return "EARN_DONE_BUTTON";
        default:
            return "UNKNOWN_BUTTON";
    }
}

// ======================================================

void DWIN_HMI::pageSwitch(uint8_t page)
{
    uint8_t frame[10] =
    {
        0x5A,
        0xA5,
        0x07,
        0x82,
        0x00,
        0x84,
        0x5A,
        0x01,
        0x00,
        page
    };

    dwin->write(frame,10);

    Serial.print("PAGE -> ");
    Serial.println(page);
}

// ======================================================

void DWIN_HMI::writeNumber(uint16_t vp, uint16_t value)
{
    uint8_t frame[8];

    frame[0] = 0x5A;
    frame[1] = 0xA5;
    frame[2] = 0x05;
    frame[3] = 0x82;

    frame[4] = vp >> 8;
    frame[5] = vp & 0xFF;

    frame[6] = value >> 8;
    frame[7] = value & 0xFF;

    dwin->write(frame,8);
}

// ======================================================

void DWIN_HMI::writeString(uint16_t vp, String text)
{
    uint16_t len = text.length();

    uint8_t frame[150];

    frame[0] = 0x5A;
    frame[1] = 0xA5;
    frame[2] = len + 3;

    frame[3] = 0x82;

    frame[4] = vp >> 8;
    frame[5] = vp & 0xFF;

    memcpy(&frame[6], text.c_str(), len);

    dwin->write(frame, len + 6);
}

// ======================================================

void DWIN_HMI::setData(uint16_t bottles, uint16_t earning)
{
    writeNumber(VP_BOTTLE, bottles);

    delay(10);

    writeNumber(VP_EARNING, earning);
}

// ======================================================

void DWIN_HMI::setQR(uint16_t amount)
{
    String transactionID =
        String((uint32_t)esp_random(), HEX);

    String qr =
        "RVM|" +
        String(amount) +
        "|" +
        transactionID;

    writeString(VP_QR, qr);

    Serial.println(qr);
}

// ======================================================

void DWIN_HMI::reset()
{
    lastButton = NO_BUTTON;

    rxIndex = 0;
}