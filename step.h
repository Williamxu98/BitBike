#ifndef STEP_H
#define STEP_H

#include "definitions.h"
#include "images.h"
#include "time.h"
#include "display.h"

#include <stdlib.h>

extern def_entity * ste_firstEntity;
extern def_entity * ste_lastEntity;
extern int ste_time;
extern def_screen ste_screen;
extern def_vector glo_camera;

void ste_step();

#endif
