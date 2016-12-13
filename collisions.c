#include "collisions.h"

def_vector col_alterVelocityLandscape(def_vector position, def_vector velocity, def_landscape landscape)
{
    for (int i = 0; i < landscape.numberOfChunks; i++)
    {
        velocity = col_alterVelocityChunk(position, velocity, landscape.chunks[i]);
    }

    return velocity;
}

def_vector col_alterVelocityChunk(def_vector position, def_vector velocity, def_chunk chunk)
{
    if (geo_checkInRectangle(chunk.range, position) || geo_checkInRectangle(chunk.range, geo_addVector(position, velocity)))
    {
        def_vector previousVelocity = {0, 0};
        int maxIterations = 20;

        while ((previousVelocity.x != velocity.x || previousVelocity.y != velocity.y) && maxIterations--)
        {
            previousVelocity = velocity;

            for (int i = 0; i < chunk.numberOfLineSegments; i++)
            {
                velocity = col_alterVelocity(position, velocity, chunk.lineSegments[i]);
            }
        }

        if (!maxIterations)
        {
            // Usually even in complex situations there will only be 4 iterations. This is nearly impossible and would be a sign something funny happened in level design.
            ser_printToSerial("Error: Collision iteration maximum has been exceeded.\n");
        }
    }

    return velocity;
}

def_vector col_alterVelocity(def_vector position, def_vector velocity, def_lineSegment solid)
{
    float thickness = 0.2; // For dealing with floating point errors. Prevents phasing through lines.
    float extension = 5; // For dealing with floating point errors. Prevents phasing through corners.

    def_lineSegment movement;
    movement.start = position;
    movement.end = geo_addVector(position, velocity);

    def_vector translation;
    translation = geo_multiplyVector(solid.start, -1);

    solid = geo_translateLineSegment(solid, translation);
    movement = geo_translateLineSegment(movement, translation);

    float rotation = -atan2(solid.end.y, solid.end.x);

    solid = geo_rotateLineSegment(solid, rotation);
    movement = geo_rotateLineSegment(movement, rotation);

    float maxX = solid.end.x;

    // The whole system has been translated so that the solid is on the x-axis, starting for the origin and ending in the positive.
    // The movement line segment can now be analyzed almost independantly.
    // The line segment is only solid from the negative y direction.

    // If the movement does not reach the line segment.
    if (movement.start.y < 0 && movement.end.y < 0)
    {
        return velocity;
    }
    else
    // If the movement misses the line segment.
    if ((movement.start.x < 0 && movement.end.x < 0) || (movement.start.x > solid.end.x && movement.end.x > solid.end.x))
    {
        return velocity;
    }
    else
    // If the movement crosses the line segment.
    if (movement.start.y <= 0 && movement.end.y >= 0)
    {
        float zeroPoint = movement.start.x - movement.start.y * ((movement.start.x - movement.end.x) / (movement.start.y - movement.end.y));
        // If the movement actually crosses the line segment.
        if (-extension <= zeroPoint && zeroPoint <= maxX + extension)
        {
            movement.end.y = 0;
        }
        else
        // If the movement doesn't actually cross the line segment.
        {
            return velocity;
        }
    }
    else
    // If the movement is going from the positive side of the line segment.
    if (0 >= movement.start.y && 0 <= movement.end.y)
    {
        return velocity;
    }
    else
    // If the movement is is on the positive side of the line segment but probably shouldn't be because of floating point errors.
    {
        if (movement.start.y < thickness && movement.start.y < movement.end.y)
        {
            movement.end.y = 0;
        }
        else
        {
            return velocity;
        }
    }

    // Fixes direction and goes back to the origin.
    movement = geo_rotateLineSegment(movement, -rotation);
    movement = geo_translateLineSegment(movement, geo_multiplyVector(movement.start, -1));

    return movement.end;
}
