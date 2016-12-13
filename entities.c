#include "entities.h"

void ent_addIdToStep(def_entity * entity) // Not included in entities.h since it is internal.
{
    if (entity)
    {
        if (ste_lastEntity)
        {
            entity->previous = ste_lastEntity;
            entity->next = NULL;
            ste_lastEntity->next = entity;
            ste_lastEntity = entity;
        }
        else
        {
            ste_lastEntity = entity;
            ste_firstEntity = entity;
            entity->previous = NULL;
            entity->next = NULL;
        }
    }
}

def_entity * ent_deleteId(def_entity * entity)
{
    if (entity)
    {
        if (entity->previous == NULL)
        {
            ste_firstEntity = entity->next;
        }
        else
        {
            entity->previous->next = entity->next;
        }

        if (entity->next == NULL)
        {
            ste_lastEntity = entity->previous;
        }
        else
        {
            entity->next->previous = entity->previous;
        }

        free(entity->data);
        free(entity);
    }

    return NULL;
}

void ent_deleteAllEntities()
{
    while (ste_lastEntity)
    {
        ent_deleteId(ste_lastEntity);
    }
}


// Entity Template:
// Comments (include //**) usually indicate what needs to be changed.
// Generate automatically at https://codepen.io/Nicholas/full/MbJPEb/.

/*
    // ENTITY entityName
    //
    // A brief description of the entity.

    typedef struct ent_templateData //**
    {
        entity * self;
        // Set up any relevant data.
    } ent_templateData; //**

    void ent_beginTemplate(ent_templateData * data) //**
    {
        // Set up any hitboxes or regions.
    }

    void ent_stepTemplate(ent_templateData * data) //**
    {
        // Do typical calculations.
    }

    void ent_endTemplate(ent_templateData * data) //**
    {
        // Draw any images.
    }

    void ent_createTemplate(initializable values) //** Add this to entities.h
    {
        ent_templateData * data = malloc(sizeof(ent_templateData)); //**

        // Initialize initializable values.

        def_entity * entity = malloc(sizeof(def_entity));

        data->self = entity;

        entity->data = data;
        entity->begin = (void *) &ent_beginTemplate; //**
        entity->step = (void *) &ent_stepTemplate; //**
        entity->end = (void *) &ent_endTemplate; //**

        ent_addIdToStep(entity);
    }
*/


// ENTITY loadRoom:
//
// Loads a room, depending on the set room value.

typedef struct ent_loadRoomData
{
    def_entity * self;
    BYTE room;
    int delay;
} ent_loadRoomData;

void ent_stepLoadRoom(ent_loadRoomData * data)
{
    data->delay -= ste_time;

    if (data->delay < 0)
    {
        BYTE room = data->room;

        reg_deleteAllRegions();
        ent_deleteAllEntities();

        glo_camera = (def_vector) {0, 0};
        glo_dead = false;

        switch (room)
        {
            case 0:
                ent_createShowLogo(3000);
                break;

            case 1:
                ent_createDeath();
                break;

            case 2:
                ent_createWin();
                break;

            case 3:
                ent_createMenuSelector();
                break;

            case 4:
                glo_level = 1;
                ent_createPlayerBike(20, 20, 0);
                ent_createLevelTimer(15000);
                ent_createFlag((def_vector) {550, 45}, 1);
				ent_createCloud(0);
				ent_createCloud(1);
				ent_createCloud(2);
                break;

            case 5:
                glo_level = 2;
                ent_createPlayerBike(0, 0, 1);
                ent_createLevelTimer(18000);
                ent_createFlag((def_vector) {375, 141}, 2);
				ent_createCloud(0);
				ent_createCloud(1);
				ent_createCloud(2);
                break;

            case 6:
                glo_level = 3;
                ent_createPlayerBike(0, 0, 2);
                ent_createLevelTimer(25000);
                ent_createFlag((def_vector) {1400, -100}, 3);
                ent_createCloud(0);
                ent_createCloud(1);
                ent_createCloud(2);
                break;

            case 7:
                glo_level = 4;
                ent_createPlayerBike(0, 0, 3);
                ent_createLevelTimer(15000);
                ent_createFlag((def_vector) {850, 30}, 4);
                ent_createCloud(0);
                ent_createCloud(1);
                ent_createCloud(2);
                break;

            case 8:
                glo_level = 5;
                ent_createPlayerBike(0, 0, 4);
                ent_createLevelTimer(20000);
                ent_createFlag((def_vector) {700, 0}, 5);
                ent_createCloud(0);
                ent_createCloud(1);
                ent_createCloud(2);
                break;

            case 9:
                glo_level = 6;
                ent_createPlayerBike(0, 0, 5);
                ent_createLevelTimer(20000);
                ent_createFlag((def_vector) {500, -25}, 6);
                ent_createCloud(0);
                ent_createCloud(1);
                ent_createCloud(2);
                break;
        }
    }
}

def_entity * ent_createLoadRoom(BYTE room, int delay)
{
    ent_loadRoomData * data = malloc(sizeof(ent_loadRoomData));

    data->room = room;
    data->delay = delay;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepLoadRoom;
    entity->end = (void *) NULL;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY showLogo:
//
// Shows the logo.

typedef struct ent_showLogoData
{
    def_entity * self;
} ent_showLogoData;

void ent_stepShowLogo(ent_showLogoData * data)
{
    dis_addImageToScreen((def_vector) {0, 0}, ima_logo, 1, &ste_screen);
}

def_entity * ent_createShowLogo(int delay)
{
    ent_createLoadRoom(3, delay);
    ent_showLogoData * data = malloc(sizeof(ent_showLogoData));

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepShowLogo;
    entity->end = (void *) NULL;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY death:
//
// Displays "you died".

typedef struct ent_deathData
{
    def_entity * self;
    float height;
    float place1;
    float place2;
    int delay;
} ent_deathData;

void ent_stepDeath(ent_deathData * data)
{

    float speed = 0.08;
    data->height += ((float) ste_time) * speed;
    if (data->height > 24)
    {
        data->height = 24;
        data->delay -= ste_time;
    }

    if (data->delay < 0)
    {
        data->place1 -= ((float) ste_time) * 2 * speed;
        data->place2 += ((float) ste_time) * 2 * speed;
    }
}

void ent_endDeath(ent_deathData * data)
{
    dis_addImageToScreen((def_vector) {(data->place1 + data->place2) / 2 + ima_skull.width - 4, data->height + 18 - data->place1}, ima_skull, 100, &ste_screen);

    dis_addImageToScreen((def_vector) {data->place1 + 0 , data->height - 0}, ima_font[24], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place1 + 6 , data->height - 0}, ima_font[14], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place1 + 11, data->height - 0}, ima_font[20], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 25, data->height - 4}, ima_font[3], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 31, data->height - 2}, ima_font[8], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 33, data->height - 0}, ima_font[4], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 39, data->height - 4}, ima_font[3], 100, &ste_screen);
}

def_entity * ent_createDeath()
{
    ent_deathData * data = malloc(sizeof(ent_deathData));

    data->height = -10;
    data->place1 = 42;
    data->place2 = 42;
    data->delay = 1000;

    ent_createLoadRoom(3, 2000);

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepDeath;
    entity->end = (void *) &ent_endDeath;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY win:
//
// Shows "you won".

typedef struct ent_winData
{
    def_entity * self;
    float height;
    float place1;
    float place2;
    int delay;
} ent_winData;

void ent_stepWin(ent_winData * data)
{
    float speed = 0.08;
    data->height += ((float) ste_time) * speed;
    if (data->height > 24)
    {
        data->height = 24;
        data->delay -= ste_time;
    }

    if (data->delay < 0)
    {
        data->place1 -= ((float) ste_time) * 2 * speed;
        data->place2 += ((float) ste_time) * 2 * speed;
    }
}

void ent_endWin(ent_winData * data)
{
    dis_addImageToScreen((def_vector) {(data->place1 + data->place2) / 2 + ima_face.width - 10, data->height + 18 - data->place1}, ima_face, 100, &ste_screen);

    dis_addImageToScreen((def_vector) {data->place1 + 0 , data->height - 0}, ima_font[24], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place1 + 6 , data->height - 0}, ima_font[14], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place1 + 11, data->height - 0}, ima_font[20], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 25, data->height - 0}, ima_font[22], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 33, data->height - 0}, ima_font[14], 100, &ste_screen);
    dis_addImageToScreen((def_vector) {data->place2 + 38, data->height - 0}, ima_font[13], 100, &ste_screen);
}

def_entity * ent_createWin()
{
    ent_winData * data = malloc(sizeof(ent_winData));

    data->height = -10;
    data->place1 = 42;
    data->place2 = 42;
    data->delay = 2000;

    glo_level++;

    ent_createLoadRoom(3, 3000);

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepWin;
    entity->end = (void *) &ent_endWin;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY menuSelector:
//
// Runs the main menu.

typedef struct ent_menuSelectorData
{
    def_entity * self;
    int selected;
    bool previous;
    bool next;
    int speed;
    float frame;
    bool frozen;
    float height;
    bool down;
} ent_menuSelectorData;

void ent_stepMenuSelector(ent_menuSelectorData * data)
{
    if (!data->frozen)
    {
        if (inp_checkButton(BUTTON_LEFT) || (inp_checkAccelerometer() < -4.0))
        {
            if (!data->previous)
            {
                data->selected--;
            }

            data->previous = TRUE;
        }
        else if (inp_checkAccelerometer() < 3.0)
        {
            data->previous = FALSE;
        }

        if (inp_checkButton(BUTTON_RIGHT) || (inp_checkAccelerometer() > 4.0))
        {
            if (!data->next)
            {
                data->selected++;
            }

            data->next = TRUE;
        }
        else if (inp_checkAccelerometer() > -3.0)
        {
            data->next = FALSE;
        }

        if (data->selected > glo_levelMax + 1)
        {
            data->selected = glo_level;
        }

        if (data->selected > glo_levelMax)
        {
            data->selected = glo_levelMax;
        }

        if (data->selected < 1)
        {
            data->selected = 1;
        }
    }

    float final = data->selected * SCREEN_WIDTH;

    float speed = (glo_camera.x - final) / data->speed;

    speed *= ste_time;

    if (fabs(speed) > fabs(glo_camera.x - final))
    {
        speed = glo_camera.x - final;
    }

    if (!data->frozen)
    {
        glo_camera.x -= speed;
    }

    int num = (glo_camera.x / SCREEN_WIDTH);
    int place = ((float) num + 0.5) * SCREEN_WIDTH;

    if (num == 1) //Compensating for the fact that 1 is skinny.
    {
        place += 2;
    }

    int height = 10;

    if (num > 0)
    {
        dis_addImageToScreen((def_vector) {place - 15, height - 3}, ima_font[11],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 13, height    }, ima_font[4],        100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 7 , height    }, ima_font[21],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 1 , height    }, ima_font[4],        100, &ste_screen);
        dis_addImageToScreen((def_vector) {place + 5 , height - 3}, ima_font[11],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place + 11, height - 2}, ima_font[26 + num], 100, &ste_screen);
    }

    num++;
    place = ((float) num + 0.5) * SCREEN_WIDTH;

    if (num == 1)
    {
        place += 2;
    }

    if (num > 0)
    {
        dis_addImageToScreen((def_vector) {place - 15, height - 3}, ima_font[11],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 13, height    }, ima_font[4],        100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 7 , height    }, ima_font[21],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place - 1 , height    }, ima_font[4],        100, &ste_screen);
        dis_addImageToScreen((def_vector) {place + 5 , height - 3}, ima_font[11],       100, &ste_screen);
        dis_addImageToScreen((def_vector) {place + 11, height - 2}, ima_font[26 + num], 100, &ste_screen);
    }

    data->frame += fabs(speed) / 5;
    while (data->frame >= 3)
    {
        data->frame -= 3;
    }

    if (data->frozen || fabs(speed) < 0.8 / ste_time)
    {
        if (data->frozen || inp_checkButton(BUTTON_UP))
        {
            dis_addImageToScreen((def_vector) {SCREEN_WIDTH / 2 - 6 + glo_camera.x, SCREEN_HEIGHT - 13 + glo_camera.y}, ima_personCelebrating, 9, &ste_screen);
            dis_addImageToScreen((def_vector) {20 + glo_camera.x, 6 + glo_camera.y}, ima_star, 9, &ste_screen);
            dis_addImageToScreen((def_vector) {SCREEN_WIDTH - 20 - ima_star.width + glo_camera.x, 6 + glo_camera.y}, ima_star, 9, &ste_screen);

            if (!data->frozen)
            {
                ent_createLoadRoom(data->selected + 3, 1000);
                data->frozen = TRUE;
            }
        }
        else
        {
            dis_addImageToScreen((def_vector) {SCREEN_WIDTH / 2 - 6 + glo_camera.x, SCREEN_HEIGHT - 13 + glo_camera.y}, ima_person, 9, &ste_screen);
        }
    }
    else
    {
        dis_addFlippedImageToScreen((def_vector) {SCREEN_WIDTH / 2 - 6 + glo_camera.x, SCREEN_HEIGHT - 13 + glo_camera.y}, ima_personRunning[(int) data->frame], (bool) (speed > 0), false, 9, &ste_screen);
    }

    if (inp_checkButton(BUTTON_DOWN))
    {
        if (!data->down)
        {
            glo_defaultColour = 1 - glo_defaultColour;

            data->down = TRUE;
        }
    }
    else
    {
        data->down = FALSE;
    }
}

def_entity * ent_createMenuSelector()
{
    ent_menuSelectorData * data = malloc(sizeof(ent_menuSelectorData));

    data->selected = glo_level;
    data->previous = TRUE;
    data->next = TRUE;
    data->speed = 250;
    data->frame = 0;
    data->frozen = FALSE;
    data->down = TRUE;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepMenuSelector;
    entity->end = (void *) NULL;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY playerBike:
//
// The player and his bike

typedef struct ent_playerBikeData
{
    def_entity * self;
    def_dynamic wheel1;
    def_dynamic wheel2;
    float width;
    float angle;
    def_vector normal1;
    def_vector normal2;
    int lean;
    float height;
    bool dead;
    int landscape;
} ent_playerBikeData;

void ent_stepPlayerBike(ent_playerBikeData * data)
{
    data->wheel1.acceleration = (def_vector) {0, 0};
    data->wheel2.acceleration = (def_vector) {0, 0};

    def_vector gravity = {0, 50}; // How much gravity there is.
    float springConstant = 1024; // How stiff the bike is. High is good, but too high causes glitches.
    float dragFactor = 32; // How soft the bike is. Ideally should be the square root of the spring constant, but doesn't have to be.
    float gasFactor = 200; // How fast you go when you accelerate.
    float frictionFactor = 0.1; // How much friction there is.
    float rotationFactor = 80; // How easily you can rotate.
    float rotationFrictionFactor = 1; // How much rotational friction there is.
    float maxRotation = 4; // Maximum allowed input from the accelerometer.

    if (data->dead)
    {
        springConstant = 0;
        dragFactor = 0;
        gasFactor = 0;
        rotationFactor = 0;
        rotationFrictionFactor = 0;
    }

    // Deal with gravity:
    data->wheel1.acceleration = geo_addVector(data->wheel1.acceleration, gravity);
    data->wheel2.acceleration = geo_addVector(data->wheel2.acceleration, gravity);

    // Make the bike itself reasonably rigid:
    def_vector relativePosition = geo_addVector(data->wheel1.position, geo_multiplyVector(data->wheel2.position, -1));

    data->angle = geo_getVectorAngle(relativePosition);
    float length = geo_getVectorLength(relativePosition);

    float springForce = springConstant * (length - data->width);

    def_vector springForceVector = geo_createPolarVector(springForce, data->angle);
    def_vector relativeVelocity = geo_addVector(data->wheel1.velocity, geo_multiplyVector(data->wheel2.velocity, -1));
    def_vector dragForceVector = geo_multiplyVector(geo_projectVector(relativeVelocity, data->angle), dragFactor);
    def_vector totalForce = geo_addVector(springForceVector, dragForceVector);

    data->wheel1.acceleration = geo_addVector(data->wheel1.acceleration, geo_multiplyVector(totalForce, -1));
    data->wheel2.acceleration = geo_addVector(data->wheel2.acceleration, totalForce);

    // Allow controls and rotation:
    // Right wheel gas:
    if (geo_getVectorLength(data->normal1))
    {
        float gasForce = gasFactor * inp_checkButton(BUTTON_RIGHT) - gasFactor * inp_checkButton(BUTTON_LEFT);

        if (gasForce > 0)
        {
            gasForce = 0;
        }

        def_vector gasForceVector = geo_createPolarVector(gasForce, geo_getVectorAngle(data->normal1) + PI/2);
        gasForceVector = geo_addVector(gasForceVector, geo_multiplyVector(data->wheel1.velocity, -frictionFactor));
        data->wheel1.acceleration = geo_addVector(data->wheel1.acceleration, gasForceVector);
    }

    // Right wheel gas:
    if (geo_getVectorLength(data->normal2))
    {
        float gasForce = gasFactor * inp_checkButton(BUTTON_RIGHT) - gasFactor * inp_checkButton(BUTTON_LEFT);

        if (gasForce < 0)
        {
            gasForce = 0;
        }

        def_vector gasForceVector = geo_createPolarVector(gasForce, geo_getVectorAngle(data->normal2) + PI / 2);
        gasForceVector = geo_addVector(gasForceVector, geo_multiplyVector(data->wheel2.velocity, -frictionFactor));
        data->wheel2.acceleration = geo_addVector(data->wheel2.acceleration, gasForceVector);
    }

    // Rotation:
    float rotation = inp_checkAccelerometer() / maxRotation;

    if (rotation > 1)
    {
        rotation = 1;
    }
    else if (rotation < -1)
    {
        rotation = -1;
    }

    rotation = rotation * sqrt(rotation * rotation); // Distributes the accelerometer input more nicely.

    float rotationForce = rotation * rotationFactor;
    def_vector rotationForceVector = geo_createPolarVector(rotationForce, data->angle + PI / 2);

    rotationForceVector = geo_addVector(rotationForceVector, geo_multiplyVector(geo_projectVector(relativeVelocity, data->angle + PI / 2), -rotationFrictionFactor));

    data->wheel1.acceleration = geo_addVector(data->wheel1.acceleration, rotationForceVector);
    data->wheel2.acceleration = geo_addVector(data->wheel2.acceleration, geo_multiplyVector(rotationForceVector, -1));

    // Update velocities:
    data->wheel1.realAcceleration = geo_multiplyVector(data->wheel1.acceleration, ((float) ste_time) / 1000);
    data->wheel2.realAcceleration = geo_multiplyVector(data->wheel2.acceleration, ((float) ste_time) / 1000);
    data->wheel1.velocity = geo_addVector(data->wheel1.velocity, data->wheel1.realAcceleration);
    data->wheel2.velocity = geo_addVector(data->wheel2.velocity, data->wheel2.realAcceleration);

    data->wheel1.realVelocity = geo_multiplyVector(data->wheel1.velocity, ((float) ste_time) / 1000);
    data->wheel2.realVelocity = geo_multiplyVector(data->wheel2.velocity, ((float) ste_time) / 1000);

    // Do collisions:
    def_vector originalVelocity1 = data->wheel1.realVelocity;
    data->wheel1.realVelocity = col_alterVelocityLandscape(data->wheel1.position, data->wheel1.realVelocity, lan_landscape[data->landscape]);
    data->normal1 = geo_addVector(data->wheel1.realVelocity, geo_multiplyVector(originalVelocity1, -1));
    data->normal1 = geo_multiplyVector(data->normal1, (1000) / ((float) ste_time));

    def_vector originalVelocity2 = data->wheel2.realVelocity;
    data->wheel2.realVelocity = col_alterVelocityLandscape(data->wheel2.position, data->wheel2.realVelocity, lan_landscape[data->landscape]);
    data->normal2 = geo_addVector(data->wheel2.realVelocity, geo_multiplyVector(originalVelocity2, -1));
    data->normal2 = geo_multiplyVector(data->normal2, (1000) / ((float) ste_time));

    // Check if the head has hit anything:
    def_vector cmPosition = geo_multiplyVector(geo_addVector(data->wheel1.position, data->wheel2.position), 0.5);
    def_vector headPosition = geo_addVector(cmPosition, geo_createPolarVector(data->height, data->angle - PI / 2));
    def_vector cmVelocity = geo_multiplyVector(geo_addVector(data->wheel1.realVelocity, data->wheel2.realVelocity), 0.5);
    def_vector cmRotationalVelocity = geo_addVector(data->wheel1.realVelocity, geo_multiplyVector(data->wheel2.realVelocity, -1));
    def_vector headRotationalVelocity = geo_createPolarVector(data->height / data->width * geo_getVectorLength(cmRotationalVelocity), geo_getVectorAngle(cmRotationalVelocity) - PI / 2);
    def_vector headVelocity = geo_addVector(cmVelocity, headRotationalVelocity);
    headVelocity = geo_multiplyVector(headVelocity, ((float) 1) / geo_getVectorLength(headVelocity));

    if (!data->dead && (data->wheel2.position.y > 200 || geo_getVectorLength(geo_addVector(headVelocity, geo_multiplyVector(col_alterVelocityLandscape(headPosition, headVelocity, lan_landscape[data->landscape]), -1))) > 0.001))
    {
        data->dead = TRUE;
        glo_dead = TRUE;
        ent_createLoadRoom(1, 2000);
    }

    // Update positions:
    data->wheel1.velocity = geo_multiplyVector(data->wheel1.realVelocity, (1000) / ((float) ste_time));
    data->wheel2.velocity = geo_multiplyVector(data->wheel2.realVelocity, (1000) / ((float) ste_time));

    data->wheel1.position = geo_addVector(data->wheel1.position, data->wheel1.realVelocity);
    data->wheel2.position = geo_addVector(data->wheel2.position, data->wheel2.realVelocity);

    // Decide out the leaning animation:
    if (fabs(rotation) < 0.2)
    {
        data->lean = 0;
    }
    else if (rotation > 0.4)
    {
        data->lean = 1;
    }
    else if (rotation < -0.4)
    {
        data->lean = -1;
    }
}

void ent_endPlayerBike(ent_playerBikeData * data)
{
    dis_addImageToScreen(geo_addVector(data->wheel1.position, (def_vector) {-2, -2}), ima_wheel, 9, &ste_screen);
    dis_addImageToScreen(geo_addVector(data->wheel2.position, (def_vector) {-2, -2}), ima_wheel, 9, &ste_screen);

    def_vector imagePosition = geo_multiplyVector(geo_addVector(data->wheel1.position, data->wheel2.position), 0.5);
    imagePosition = geo_addVector(imagePosition, geo_multiplyVector(geo_addVector(data->wheel1.realVelocity, data->wheel2.realVelocity), 0.5)); // Compensating for lag at high speeds.

    // Move the camera for scrolling (if the player is alive):
	if (!data->dead)
	{
		glo_camera = geo_addVector(imagePosition, (def_vector) {-60, -20});
	}

	if (!data->dead)
    {
        def_matrix transformation = geo_getRotationMatrix(data->angle);

        dis_addTransformedImageToScreen(imagePosition, (def_vector) {ima_bike[1 + data->lean].width / 2, ima_bike[1 + data->lean].height}, transformation, ima_bike[1 + data->lean], 50, &ste_screen);
    }

    dis_addLandscapeToScreen(lan_landscape[data->landscape], 1, 1, &ste_screen);
}

def_entity * ent_createPlayerBike(float x, float y, int landscape)
{
    ent_playerBikeData * data = malloc(sizeof(ent_playerBikeData));

    data->width = 10;

    data->wheel1 = (def_dynamic) {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
    data->wheel2 = (def_dynamic) {{0, 0}, {0, 0}, {0, 0}, {0, 0}};

    data->wheel1.position.x = x;
    data->wheel1.position.y = y;

    data->wheel2.position.x = x - data->width;
    data->wheel2.position.y = y;

    data->height = 6;

    data->dead = FALSE;
    data->lean = 0;

    data->landscape = landscape;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepPlayerBike;
    entity->end = (void *) &ent_endPlayerBike;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY levelTimer:
//
// The countdown timer for the current level

typedef struct ent_levelTimerData
{
    def_entity * self;
	int timeLeft; // In milliseconds

} ent_levelTimerData;

void ent_stepLevelTimer(ent_levelTimerData * data)
{
    data->timeLeft -= ste_time;
    if (data->timeLeft < 0)
    {
        data->timeLeft = 0;
        ent_createLoadRoom(1, 0);
    }
}

void ent_endLevelTimer(ent_levelTimerData * data)
{
    BYTE onesDigit = (data->timeLeft/1000) % 10;
	BYTE tensDigit = (data->timeLeft/10000) % 10;

	int x = (int)(glo_camera.x+0.5);
	int y = (int)(glo_camera.y+0.5);
	int x_increment = 0;
	int y_increment = 0;

	dis_addImageToScreen((def_vector) {x+(x_increment+=1), y+(y_increment+=1)}, ima_font[19], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+=4), y+(y_increment+=2)}, ima_font[8], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+=2), y+(y_increment+=2)}, ima_font[12], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+=8), y+(y_increment)}, ima_font[4], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+=6), y+(y_increment)}, ima_font[39], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+=3), y+(y_increment-=2)}, ima_font[26+tensDigit], 100, &ste_screen);
	dis_addImageToScreen((def_vector) {x+(x_increment+= (tensDigit == 1 ? 2 :6)), y+(y_increment)}, ima_font[26+onesDigit], 100, &ste_screen);
}

// Don't forget to add this to entities.h. **
def_entity * ent_createLevelTimer(int levelTime)
{
    ent_levelTimerData * data = malloc(sizeof(ent_levelTimerData));

	data->timeLeft = levelTime;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;
    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepLevelTimer;
    entity->end = (void *) &ent_endLevelTimer;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY flag:
//
// The flag at the end of the level

typedef struct ent_flagData
{
    def_entity * self;
    def_vector position;
    int level;
} ent_flagData;

void ent_beginFlag(ent_flagData * data)
{
	if (glo_camera.x - glo_cameraRelativePosition.x + 4 >= data->position.x && glo_camera.y - glo_cameraRelativePosition.y <= data->position.y +30)
	{
		if (!glo_dead)
        {
            ent_createLoadRoom(2, 0);
        }
	}
}

void ent_endFlag(ent_flagData * data)
{
   dis_addImageToScreen(geo_addVector(data->position, (def_vector) {0, - ima_flag.height + 4}), ima_flag, 1, &ste_screen);
}

def_entity * ent_createFlag(def_vector position, int level)
{
    ent_flagData * data = malloc(sizeof(ent_flagData));

	data->position = position;
    data->level = level;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;

    entity->data = data;

    entity->begin = (void *) &ent_beginFlag;
    entity->step = (void *) NULL;
    entity->end = (void *) &ent_endFlag;

    ent_addIdToStep(entity);

    return entity;
}


// ENTITY cloud:
//
// Clouds floating in the background.

typedef struct ent_cloudData
{
    def_entity * self;
    def_vector position;
	BYTE cloudNo;
	float velocity;
} ent_cloudData;

void ent_stepCloud(ent_cloudData * data)
{
    data->position.x += data->velocity * (float)(ste_time)/1000;
	if (data->position.x > SCREEN_WIDTH)
	{
		data->cloudNo = (BYTE)(random() % 3);
		data->position.x = -ima_clouds[data->cloudNo].width;
		data->position.y = random() % 30-3;
		data->velocity = (float)(random() % 500 + 200)/100;
	}
}

void ent_endCloud(ent_cloudData * data)
{
    dis_addImageToScreen((def_vector) {glo_camera.x + data->position.x, glo_camera.y + data->position.y}, ima_clouds[data->cloudNo] , 0, &ste_screen);
}

// Don't forget to add this to entities.h. **
def_entity * ent_createCloud(BYTE cloudNo)
{

    ent_cloudData * data = malloc(sizeof(ent_cloudData));

	data->cloudNo = cloudNo;
	data->position.x = random() % 128 - 20;
	data->position.y = random() % 30 - 3;
	data->velocity = (float)(random() % 800 + 200)/100;

    def_entity * entity = malloc(sizeof(def_entity));

    data->self = entity;
    entity->data = data;

    entity->begin = (void *) NULL;
    entity->step = (void *) &ent_stepCloud;
    entity->end = (void *) &ent_endCloud;

    ent_addIdToStep(entity);

    return entity;
}
