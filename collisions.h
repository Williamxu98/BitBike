#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "definitions.h"
#include "geometry.h"

def_vector col_alterVelocityLandscape(def_vector position, def_vector velocity, def_landscape landscape);
def_vector col_alterVelocityChunk(def_vector position, def_vector velocity, def_chunk chunk);
def_vector col_alterVelocity(def_vector position, def_vector velocity, def_lineSegment solid);

#endif
