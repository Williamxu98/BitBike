#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "definitions.h"
#include <stdlib.h>
#include <math.h>

float geo_getVectorLength(def_vector vector);
float geo_getVectorAngle(def_vector vector);

def_vector geo_projectVector(def_vector vector, float angle);
def_vector geo_createPolarVector(float length, float angle);

def_vector geo_multiplyVector(def_vector vector, float factor);
def_vector geo_addVector(def_vector vectorA, def_vector vectorB);
def_vector geo_rotateVector(def_vector vector, float radians);
def_vector geo_createPolarVector(float length, float angle);

float geo_getLineSegmentLength(def_lineSegment segment);
def_lineSegment geo_flipLineSegment(def_lineSegment segment);
def_lineSegment geo_translateLineSegment(def_lineSegment segment, def_vector translate);
def_lineSegment geo_rotateLineSegment(def_lineSegment segment, float radians);

bool geo_checkInRectangle(def_lineSegment segment, def_vector vector);
bool geo_checkEquals(def_vector a, def_vector b);

def_matrix geo_getIdentityMatrix();
def_matrix geo_getRotationMatrix(float angle);
def_matrix geo_getScalingMatrix(float value);

def_matrix geo_multiplyMatrix(def_matrix matrixA, def_matrix matrixB);

def_matrix geo_invertMatrix(def_matrix matrix);

def_vector geo_mapVector(def_vector vector, def_matrix matrix);
def_vector geo_mapPoint(def_vector vector, def_vector offset, def_vector translation, def_matrix matrix);

#endif
