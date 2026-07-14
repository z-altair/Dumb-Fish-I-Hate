#include "../include/util.hpp"

float angleFromVector(Vector2 v){
    return atan2(v.y, v.x);
}

Vector2 setVectorMagnitude(Vector2 v, float magnitude){
    return Vector2Scale(Vector2Normalize(v), magnitude);
}

Vector2 clampDistance(Vector2 v, Vector2 anchor, float maxDistance){
    return Vector2Add(anchor, setVectorMagnitude(Vector2Subtract(v, anchor), maxDistance));
}

float clampAngle(float angle, float anchor, float maxAngle){
    if (abs(relativeAngleDifference(angle, anchor)) <= maxAngle) {
        return simplifyAngle(angle);
    }

    if (relativeAngleDifference(angle, anchor) > maxAngle) {
      return simplifyAngle(anchor - maxAngle);
    }

    return simplifyAngle(anchor + maxAngle);
}

float relativeAngleDifference(float angle, float anchor){
    angle = simplifyAngle(angle + PI - anchor);
    anchor = PI;
    return anchor - angle;
}

float simplifyAngle(float angle){
    while(angle>=TWO_PI){
        angle -= TWO_PI;
    }

    while(angle < 0){
        angle += TWO_PI;
    }
    return angle;
}

Vector2 vectorFromAngle(float angle){
    return {cos(angle), sin(angle)};
}

void DrawEllipseRotated(Vector2 center, float radiusH, float radiusV, float rotationDeg, Color color) {
    rlPushMatrix();                             // pushMatrix()
    rlTranslatef(center.x, center.y, 0.0f);     // translate()
    rlRotatef(rotationDeg, 0.0f, 0.0f, 1.0f);   // rotate() (eje Z)
    DrawEllipse(0, 0, radiusH, radiusV, color); // ellipse()
    rlPopMatrix();                              // popMatrix()
}

void DrawEllipseCustom(Vector2 center, float radiusH, float radiusV, float rotationDeg, Color fillColor, Color outlineColor, float outlineThick) {
    rlPushMatrix();
    rlTranslatef(center.x, center.y, 0.0f);
    rlRotatef(rotationDeg, 0.0f, 0.0f, 1.0f);
    
    // Dibujamos primero el borde (una elipse ligeramente más grande)
    if (outlineThick > 0.0f) {
        DrawEllipse(0, 0, radiusH + outlineThick, radiusV + outlineThick, outlineColor);
    }
    // Dibujamos el relleno encima
    DrawEllipse(0, 0, radiusH, radiusV, fillColor);
    
    rlPopMatrix();
}