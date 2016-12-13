#ifndef REGIONS_H
#define REGIONS_H

#include "definitions.h"
#include "geometry.h"

def_region * reg_checkPointForRegion(def_vector position);
def_region * reg_checkPointForRegionByType(def_vector position, BYTE type);

def_region * reg_deleteRegion(def_region * region);
void reg_deleteAllRegions(); // WARNING: can cause significant errors if there are still any pointers left to regions. Probably only want to use this with deleteAllEntities.

def_region * reg_createRegion(def_lineSegment rectangle, BYTE type, void * data);

#endif
