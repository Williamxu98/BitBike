#include "Energia.h"

extern "C"
{
    void ser_printToSerial(const char output[])
    {
        Serial.print(output);
    }

    void ser_printFloatToSerial(float output)
    {
        Serial.println(output);
    }
}
