#include "input.hpp"
#include "Wire.h"

static TwoWire orbitWire(0);

static float const    SensorMaximumReading= 512.0;
static float const    SensorMaximumAccel  = 9.81 * 4.0;
static uint8_t const  SensorAccelerometer = 0x1D;
static uint32_t const ShakeThreshold      = 16;
static float          ShakeAccumulator    = 0;

void inp_wireInit();
void inp_wireRequestArray(int address, int* buffer, int amount);
void inp_wireWriteRegister(int address, uint8_t reg, uint8_t value);
void inp_wireWriteByte(int address, uint8_t value);

void inp_init()
{
	// Buttons
	pinMode(PUSH1, INPUT_PULLUP);
	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(PD_2, INPUT);
	pinMode(PE_0, INPUT);

	// Wire
	inp_wireInit();

	// Accelerometer
	inp_wireWriteRegister(SensorAccelerometer, 0x31, 1);
	inp_wireWriteRegister(SensorAccelerometer, 0x2D, 1 << 3);
}

int inp_checkButton(BYTE button)
{
	switch (button)
	{
		case BUTTON_LEFT:
			return !digitalRead(PUSH1);
			break;

		case BUTTON_RIGHT:
			return !digitalRead(PUSH2);
			break;

		case BUTTON_DOWN:
			return digitalRead(PD_2);
			break;

		case BUTTON_UP:
			return digitalRead(PE_0);
			break;
	}

	return 0;
}

// 0 for no tilt, positive number for tilt right, and negative number for tilt left.

float inp_checkAccelerometer()
{
    size_t const DataLength = 6;
    int data[DataLength] = {0};

    inp_wireWriteByte(SensorAccelerometer, 0x32);
    inp_wireRequestArray(SensorAccelerometer, data, DataLength);

    uint16_t xi = (data[1] << 8) | data[0];
    //uint16_t yi = (data[3] << 8) | data[2]; // Removed y and z since they are unused.
    //uint16_t zi = (data[5] << 8) | data[4];
    float x = *(int16_t*)(&xi) / SensorMaximumReading * SensorMaximumAccel;
    // float y = *(int16_t*)(&yi) / SensorMaximumReading * SensorMaximumAccel;
    // float z = *(int16_t*)(&zi) / SensorMaximumReading * SensorMaximumAccel;
    return -x;
}

void inp_wireInit()
{
    orbitWire.begin();
}

void inp_wireWriteByte(int address, uint8_t value)
{
    orbitWire.beginTransmission(address);
    orbitWire.write(value);
    orbitWire.endTransmission();
}

void inp_wireWriteRegister(int address, uint8_t reg, uint8_t value)
{
    orbitWire.beginTransmission(address);
    orbitWire.write(reg);
    orbitWire.write(value);
    orbitWire.endTransmission();
}

// inp_wireRequestArray(int address, uint32_t* buffer, uint8_t amount)
// address - I2C Address of Chip
// buffer - Buffer to store data
// amount - Bytes of information to store

void inp_wireRequestArray(int address, int* buffer,int amount)
{
    orbitWire.requestFrom(address, amount);

    do
    {
        while(!orbitWire.available());
        *(buffer++) = orbitWire.read();
    } while(--amount > 0);
}
