#include "step.h"

def_entity * ste_firstEntity = NULL;
def_entity * ste_lastEntity = NULL;
int ste_currentTime = 0;
int ste_previousTime = 0;
int ste_time = 0;
def_screen ste_screen;

void ste_step()
{
    ste_currentTime = tim_getTime();
    ste_time = tim_subtractTime(ste_previousTime, ste_currentTime);
    ste_previousTime = ste_currentTime;

    if(ste_time > 20)
    {
        ste_time = 20;
    }

    ste_screen = dis_getBlankScreen();

    def_entity * currentEntity = ste_firstEntity;
    def_entity * nextEntity;

    while (currentEntity)
    {
        nextEntity = currentEntity->next;

        if(currentEntity->begin != NULL)
        {
            currentEntity->begin(currentEntity->data);
        }

        currentEntity = nextEntity;
    }


    currentEntity = ste_firstEntity;
    while (currentEntity)
    {
        nextEntity = currentEntity->next;

        if(currentEntity->step != NULL)
        {
            currentEntity->step(currentEntity->data);
        }

        currentEntity = nextEntity;
    }

    currentEntity = ste_firstEntity;
    while (currentEntity)
    {
        nextEntity = currentEntity->next;

        if(currentEntity->end != NULL)
        {
            nextEntity = currentEntity->next;
            currentEntity->end(currentEntity->data);
        }

        currentEntity = nextEntity;
    }

    dis_drawScreen(&ste_screen);
}
