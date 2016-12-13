#include "geometry.h"

float geo_getVectorLength(def_vector vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float geo_getVectorAngle(def_vector vector)
{
	if (vector.x || vector.y)
	{
		return atan2(vector.y, vector.x);
	}
	return 0;
}

def_vector geo_projectVector(def_vector vector, float angle)
{
	double vectorLength = geo_getVectorLength(vector) * cos(geo_getVectorAngle(vector) - angle);
	vector.x = vectorLength * cos(angle);
	vector.y = vectorLength * sin(angle);
	return vector;
}

def_vector geo_createPolarVector(float length, float angle)
{
	return (def_vector) {length * cos(angle), length * sin(angle)};
}

def_vector geo_multiplyVector(def_vector vector, float factor)
{
    vector.x *= factor;
    vector.y *= factor;

    return vector;
}

def_vector geo_addVector(def_vector vectorA, def_vector vectorB)
{
    vectorA.x += vectorB.x;
    vectorA.y += vectorB.y;

    return vectorA;
}

def_vector geo_rotateVector(def_vector vector, float radians)
{
    def_vector output;

    output.x = vector.x * cos(radians) - vector.y * sin(radians);
    output.y = vector.x * sin(radians) + vector.y * cos(radians);

    return output;
}

float geo_getLineSegmentLength(def_lineSegment segment)
{
    return geo_getVectorLength((def_vector) {segment.end.x - segment.start.x, segment.end.y - segment.start.y});
}

def_lineSegment geo_flipLineSegment(def_lineSegment segment)
{
    def_vector hold;

    hold = segment.start;
    segment.start = segment.end;
    segment.end = hold;

    return segment;
}

def_lineSegment geo_translateLineSegment(def_lineSegment segment, def_vector translate)
{
    segment.start = geo_addVector(segment.start, translate);
    segment.end = geo_addVector(segment.end, translate);

    return segment;
}

def_lineSegment geo_rotateLineSegment(def_lineSegment segment, float radians)
{
    def_lineSegment output;

    output.start.x = segment.start.x * cos(radians) - segment.start.y * sin(radians);
    output.start.y = segment.start.x * sin(radians) + segment.start.y * cos(radians);

    output.end.x = segment.end.x * cos(radians) - segment.end.y * sin(radians);
    output.end.y = segment.end.x * sin(radians) + segment.end.y * cos(radians);

    return output;
}

bool geo_checkEquals(def_vector a, def_vector b)
{
	return a.x == b.x && a.y == b.y;
}

bool geo_checkInRectangle(def_lineSegment segment, def_vector vector)
{
    return ((segment.start.x <= vector.x && vector.x <= segment.end.x) || (segment.start.x >= vector.x && vector.x >= segment.end.x)) && ((segment.start.y <= vector.y && vector.y <= segment.end.y) || (segment.start.y >= vector.y && vector.y >= segment.end.y));
}

def_matrix geo_getIdentityMatrix()
{
    return (def_matrix) {1, 0, 0, 1};
}

def_matrix geo_getRotationMatrix(float angle)
{
    float cosOfAngle = cos(angle);
    float sinOfAngle = sin(angle);
    return (def_matrix) {cosOfAngle, -sinOfAngle, sinOfAngle, cosOfAngle};
}

def_matrix geo_getScalingMatrix(float value)
{
    return (def_matrix) {value, 0, 0, value};
}

def_matrix geo_multiplyMatrix(def_matrix matrixA, def_matrix matrixB)
{
    def_matrix output;

    output.a = matrixA.a * matrixB.a + matrixA.b * matrixB.c;
    output.b = matrixA.a * matrixB.b + matrixA.b * matrixB.d;
    output.c = matrixA.c * matrixB.a + matrixA.d * matrixB.c;
    output.d = matrixA.c * matrixB.b + matrixA.d * matrixB.d;

    return output;
}

def_matrix geo_invertMatrix(def_matrix matrix)
{
    float determinant = matrix.a * matrix.d - matrix.b * matrix.c;
    def_matrix output = {0};

    if (determinant)
    {
        output.a = 1 / determinant * matrix.d;
        output.b = -1 / determinant * matrix.b;
        output.c = -1 / determinant * matrix.c;
        output.d = 1 / determinant * matrix.a;
    }

    return output;
}

def_vector geo_mapVector(def_vector vector, def_matrix matrix)
{
    def_vector output;
    output.x = vector.x * matrix.a + vector.y * matrix.b;
    output.y = vector.x * matrix.c + vector.y * matrix.d;

    return output;
}

def_vector geo_mapPoint(def_vector vector, def_vector offset, def_vector translation, def_matrix matrix)
{
    vector = geo_addVector(vector, geo_multiplyVector(offset, -1));
    vector = geo_mapVector(vector, matrix);
    vector = geo_addVector(vector, translation);

    return vector;
}
