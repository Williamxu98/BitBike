#include "regions.h"

def_region * reg_firstRegion = NULL;
def_region * reg_lastRegion = NULL;

def_region * reg_checkPointForRegion(def_vector position)
{
    def_region * currentRegion = reg_firstRegion;

    while (currentRegion)
    {
        if (geo_checkInRectangle(currentRegion->rectangle, position))
        {
            return currentRegion;
        }

        currentRegion = currentRegion->next;
    }

    return NULL;
}

def_region * reg_checkPointForRegionByType(def_vector position, BYTE type)
{
    def_region * currentRegion = reg_firstRegion;

    while (currentRegion)
    {
        if (geo_checkInRectangle(currentRegion->rectangle, position) && currentRegion->type == type)
        {
            return currentRegion;
        }

        currentRegion = currentRegion->next;
    }

    return NULL;
}

def_region * reg_deleteRegion(def_region * region)
{
    if (region->previous == NULL)
    {
        reg_firstRegion = region->next;
    }
    else
    {
        region->previous->next = region->next;
    }

    if (region->next == NULL)
    {
        reg_lastRegion = region->previous;
    }
    else
    {
        region->next->previous = region->previous;
    }

    free(region->data);
    free(region);

    return NULL;
}

void reg_deleteAllRegions()
{
    while (reg_lastRegion)
    {
        reg_deleteRegion(reg_lastRegion);
    }
}

void reg_addRegion(def_region * region) // Not included in entities.h since it is internal.
{
    if (region)
    {
        if (reg_lastRegion)
        {
            region->previous = reg_lastRegion;
            region->next = NULL;
            reg_lastRegion->next = region;
            reg_lastRegion = region;
        }
        else
        {
            reg_lastRegion = region;
            reg_firstRegion = region;
            region->previous = NULL;
            region->next = NULL;
        }
    }
}

def_region * reg_createRegion(def_lineSegment rectangle, BYTE type, void * data)
{
    def_region * region = malloc(sizeof(def_region));

    region->rectangle = rectangle;
    region->type = type;
    region->data = data;

    reg_addRegion(region);

    return region;
}
