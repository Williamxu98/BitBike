#ifndef INPUT_HPP
#define INPUT_HPP

#include "definitions.h"
#include "Energia.h"

#if __cplusplus
extern "C"
{
#endif

    void inp_init();
    int inp_checkButton(BYTE buttonNo);
    float inp_checkAccelerometer();

#if __cplusplus
}
#endif

#endif
