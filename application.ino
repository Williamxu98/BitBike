extern "C"
{
    // Standard headers:
    #include <assert.h>
    #include <math.h>
    #include <stdlib.h>

    // Library headers:
    #include "OrbitOled.h"

    // These are presently unnecessary:
    //#include "delay.h"
    //#include "FillPat.h"
    //#include "I2CEEPROM.h"
    //#include "LaunchPad.h"
    //#include "OrbitBoosterPackDefs.h"
    //#include "OrbitOledChar.h"
    //#include "OrbitOledGrph.h"

    // Custom headers:
    #include "collisions.h"
    #include "definitions.h"
    #include "display.h"
    #include "entities.h"
    #include "geometry.h"
    #include "images.h"
    #include "landscape.h"
    #include "regions.h"
    #include "serial.h"
    #include "step.h"
    #include "time.h"
}

// C++ headers:
#include "input.hpp"

def_entity * ent;

void setup()
{
    OrbitOledInit();
	inp_init();
    Serial.begin(9600);
    Serial.println("~start~");
	randomSeed(10);
    ent_createLoadRoom(0, 0);
}

void loop()
{
    ste_step();
}
