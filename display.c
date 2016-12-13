#include "display.h"

def_screen dis_getBlankScreen()
{
    def_screen output;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            output.pixels[x][y] = 0;
        }
    }

    return output;
}

void dis_drawScreen(def_screen * screen)
{
    BYTE output[SCREEN_SIZE / 8] = {0x00};
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            output[x + (y / 8) * SCREEN_WIDTH] |= ((screen->pixels[x][y] + glo_defaultColour) % 2) << (y % 8);
        }
    }

    OrbitOledPutBmp(SCREEN_WIDTH, SCREEN_HEIGHT, output);
    OrbitOledUpdate();
}

def_screen * dis_addPixelToScreen(float x0, float y0, BYTE colour, BYTE zValue, def_screen * screen)
{
	int y = (int)(y0 - glo_camera.y + 0.5);
	int x = (int)(x0- glo_camera.x + 0.5);
    if (colour != 0 && y < SCREEN_HEIGHT && y >= 0 && x < SCREEN_WIDTH && x >= 0)
    {
        colour += zValue * 2;
        if ((screen->pixels[x][y] - 1) / 2 <= zValue)
        {
            screen->pixels[x][y] = colour;
        }
    }

    return screen;
}

def_screen * dis_addLineToScreen(def_lineSegment segment, BYTE colour, BYTE zValue, def_screen * screen)
{
    if (fabs(segment.start.x - segment.end.x) >= fabs(segment.start.y - segment.end.y))
    {
        int flip = 1;
        if (segment.start.x > segment.end.x)
        {
            def_vector temp = segment.start;
            segment.start = segment.end;
            segment.end = temp;
            flip = -1;
        }

        for (int x = (int) (segment.start.x - 0.5); x <= (segment.end.x + 1.5); x++)
        {
            int y = (int) (x - segment.start.x) * (segment.end.y - segment.start.y) / (segment.end.x - segment.start.x) + segment.start.y + 0.5;
            dis_addPixelToScreen(x, y + 2 * flip, colour, zValue, screen);
            dis_addPixelToScreen(x, y + 3 * flip, colour, zValue, screen);
        }
    }
    else
    {
        int flip = -1;
        if (segment.start.y > segment.end.y)
        {
            def_vector temp = segment.start;
            segment.start = segment.end;
            segment.end = temp;
            flip = 1;
        }

        for (int y = (int) (segment.start.y - 0.5); y <= (segment.end.y + 1.5); y++)
        {
            int x = (int) (y - segment.start.y) * (segment.end.x - segment.start.x) / (segment.end.y - segment.start.y) + segment.start.x + 0.5;
            dis_addPixelToScreen(x + 2 * flip, y, colour, zValue, screen);
            dis_addPixelToScreen(x + 3 * flip, y, colour, zValue, screen);
        }

    }

    return screen;
}

def_screen * dis_addChunkToScreen(def_chunk chunk, BYTE colour, BYTE zValue, def_screen * screen)
{
    for (int i = 0; i < chunk.numberOfLineSegments; i++)
    {
        dis_addLineToScreen(chunk.lineSegments[i], colour, zValue, screen);
    }

    return screen;
}

def_screen * dis_addLandscapeToScreen(def_landscape landscape, BYTE colour, BYTE zValue, def_screen * screen)
{
    for (int i = 0; i < landscape.numberOfChunks; i++)
    {
        dis_addChunkToScreen(landscape.chunks[i], colour, zValue, screen);
    }

    return screen;
}

def_screen * dis_addImageToScreen(def_vector position, def_image image, BYTE zValue, def_screen * screen)
{
    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            BYTE newValue = image.pixels[x + y * image.width];
            dis_addPixelToScreen(x + position.x, y + position.y, newValue, zValue, screen);
        }
    }

    return screen;
}

def_screen * dis_addFlippedImageToScreen(def_vector position, def_image image, bool xFlip, bool yFlip, BYTE zValue, def_screen * screen)
{
    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            int xc;
            int yc;

            if (xFlip)
            {
                xc = image.width - x - 1;
            }
            else
            {
                xc = x;
            }

            if (yFlip)
            {
                yc = image.height - y - 1;
            }
            else
            {
                yc = y;
            }

            BYTE newValue = image.pixels[xc + yc * image.width];
            dis_addPixelToScreen(x + position.x, y + position.y, newValue, zValue, screen);
        }
    }

    return screen;
}

def_screen * dis_addTransformedImageToScreen(def_vector position, def_vector origin, def_matrix matrix, def_image image, BYTE zValue, def_screen * screen)
{
    def_vector topLeft = {0, 0};
    def_vector topRight = {image.width, 0};
    def_vector bottomLeft = {0, image.height};
    def_vector bottomRight = {image.width, image.height};

    topLeft = geo_mapPoint(topLeft, origin, position, matrix);
    topRight = geo_mapPoint(topRight, origin, position, matrix);
    bottomLeft = geo_mapPoint(bottomLeft, origin, position, matrix);
    bottomRight = geo_mapPoint(bottomRight, origin, position, matrix);

    def_vector min;
    def_vector max;

    min.x = fmin(topLeft.x, fmin(topRight.x, fmin(bottomRight.x, bottomLeft.x)));
    min.y = fmin(topLeft.y, fmin(topRight.y, fmin(bottomRight.y, bottomLeft.y)));

    max.x = fmax(topLeft.x, fmax(topRight.x, fmax(bottomRight.x, bottomLeft.x)));
    max.y = fmax(topLeft.y, fmax(topRight.y, fmax(bottomRight.y, bottomLeft.y)));

    matrix = geo_invertMatrix(matrix);

    for (float x = min.x; x <= -(int) -max.x; x++)
    {
        for (float y = min.y; y <= -(int) -max.y; y++)
        {
            def_vector place = {x, y};
            place = geo_mapPoint(place, position, origin, matrix);
            place.x = (int) (place.x + 0.5);
            place.y = (int) (place.y + 0.5);

            if (place.x >= 0 && place.x < image.width && place.y >= 0 && place.y < image.height)
            {
                dis_addPixelToScreen(x, y, image.pixels[(int) (place.x + place.y * image.width)], zValue, screen);
            }
        }
    }

    return screen;
}
