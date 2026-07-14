#pragma once
#include <cmath>
#include "../lib/raylib/include/raylib.h"
#include "../lib/raylib/include/raymath.h"
#include "../lib/raylib/include/rlgl.h"


#define TWO_PI (2*PI)

float angleFromVector(Vector2 v);
Vector2 setVectorMagnitude(Vector2 v, float magnitude);
Vector2 clampDistance(Vector2 v, Vector2 anchor, float maxDistance);
float clampAngle(float angle, float anchor, float maxAngle);
float relativeAngleDifference(float angle, float anchor);
float simplifyAngle(float angle);
Vector2 vectorFromAngle(float angle);

void DrawEllipseRotated(Vector2 center, float radiusH, float radiusV, float rotationDeg, Color color);
void DrawEllipseCustom(Vector2 center, float radiusH, float radiusV, float rotationDeg, Color fillColor, Color outlineColor, float outlineThick);