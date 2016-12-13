#ifndef DISPLAY_H
#define DISPLAY_H

#include "definitions.h"
#include "OrbitOled.h"
#include "OrbitOledGrph.h"
#include "serial.h"
#include "geometry.h"
#include "global.h"

def_screen dis_getBlankScreen();

void dis_drawScreen(def_screen * output);

def_screen * dis_addPixelToScreen(float x, float y, BYTE colour, BYTE zValue, def_screen * screen);
def_screen * dis_addLineToScreen(def_lineSegment segment, BYTE colour, BYTE zValue, def_screen * screen);
def_screen * dis_addChunkToScreen(def_chunk chunk, BYTE colour, BYTE zValue, def_screen * screen);
def_screen * dis_addLandscapeToScreen(def_landscape landscape, BYTE colour, BYTE zValue, def_screen * screen);
def_screen * dis_addImageToScreen(def_vector position, def_image toAdd, BYTE zValue, def_screen * screen);
def_screen * dis_addFlippedImageToScreen(def_vector position, def_image image, bool xFlip, bool yFlip, BYTE zValue, def_screen * screen);
def_screen * dis_addTransformedImageToScreen(def_vector position, def_vector origin, def_matrix matrix, def_image toAdd, BYTE zValue, def_screen * screen);

#endif
