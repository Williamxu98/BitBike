#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Energia.h"
#include "serial.h"

#define TRUE 1
#define FALSE 0
#define BYTE char

#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1
#define BUTTON_DOWN 2
#define BUTTON_UP 3

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT

#define DEBUG_ENABLED FALSE

// Image definitions:
typedef struct def_screen
{
    BYTE pixels[SCREEN_WIDTH][SCREEN_HEIGHT];
}
def_screen;

typedef struct def_image
{
    int width;
    int height;
    const BYTE * pixels;
} def_image;

typedef struct def_matrix
{
    float a;
    float b;
    float c;
    float d;
} def_matrix;

// Geometry definitions:
typedef struct def_vector
{
    float x;
    float y;
} def_vector;

typedef struct def_lineSegment
{
    def_vector start;
    def_vector end;
} def_lineSegment;

// Landscape definitions:
typedef struct def_chunk
{
    int numberOfLineSegments;
    def_lineSegment range; //Points define the max corners for the chunk.
    const def_lineSegment * lineSegments;
} def_chunk;

typedef struct def_landscape
{
    int numberOfChunks;
    const def_chunk * chunks;
} def_landscape;

// Entity definitions:
typedef struct def_entity
{
    struct def_entity * previous;
    void (* begin) (void *);
    void (* step) (void *);
    void (* end) (void *);
    void * data;
    struct def_entity * next;
} def_entity;

// Region definitions:
typedef struct def_region
{
    struct def_region * previous;
    def_lineSegment rectangle;
    BYTE type;
    void * data;
    struct def_region * next;
} def_region;

// Physics definitions:
typedef struct def_dynamic
{
    def_vector position;
    def_vector velocity;
    def_vector realVelocity;
    def_vector acceleration;
    def_vector realAcceleration;
} def_dynamic;

#endif
