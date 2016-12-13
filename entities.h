#ifndef ENTITIES_H
#define ENTITIES_H

#include "definitions.h"
#include "step.h"
#include "serial.h"
#include "display.h"
#include "images.h"
#include "landscape.h"
#include "input.hpp"
#include "collisions.h"
#include "regions.h"
#include "global.h"

def_entity * ent_deleteId(def_entity * entity);
void ent_deleteAllEntities();

def_entity * ent_createLoadRoom(BYTE room, int delay);

def_entity * ent_createShowLogo(int delay);
def_entity * ent_createDeath();
def_entity * ent_createWin();

def_entity * ent_createMenuSelector();

def_entity * ent_createPlayerBike(float x, float y, int landscape);

def_entity * ent_createLevelTimer(int levelTime);
def_entity * ent_createFlag(def_vector position, int level);

def_entity * ent_createCloud(BYTE cloudNo);

#endif
