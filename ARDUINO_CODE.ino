#pragma GCC optimize("-O3")
#pragma GCC optimize("-j4")

#include "mcp2515.h"

MCP2515 mcp = MCP2515(10);
can_frame f;
char rxBuffer[255] = {0x00};
DATA_MSG payload = {0x00};

void send(DATA_MSG *d) {
}

// Serial1 in this example is a hardware Serial
// That talks to a secondary arduino,
// Which then relays debug statements to my Linux host laptop,
// Not the windows VM running DAS

bool readMessage(DATA_MSG *d) {
    if (Serial.available() >= 4) {
        memset(d, 0x00, sizeof(d));
        d->type = Serial.read();
        d->data_len = Serial.read();
        for (int i = 0; i < d->data_len; i++) {
            d->buffer[i] = Serial.read();
        }
        Serial1.print("Data. Type ");
        Serial1.print(d->type, DEC);
        Serial1.print(" , Size: ");
        Serial1.print(d->data_len, DEC);
        Serial1.print(", DATA: [");
        for (int i = 0; i < d->data_len; i++) {
            Serial.print(d->buffer[i], HEX);
            Serial.print(" ");
        }
        Serial1.print("]");
        Serial1.println();
        return true;
    }
    return false;
}





void setup() {
    Serial.begin(115200); 
    Serial1.begin(57600); // Debugging output
    Serial.clearWriteError();
    Serial1.clearWriteError();
    delay(10);
    SPI.begin();
    mcp.reset();
    mcp.setBitrate(CAN_500KBPS);
    mcp.setNormalMode();
    Serial1.println("DBG - Ready!");
    while(Serial.available()) {
        Serial.read();
    }
}

void loop() {
    readMessage(&payload);
}