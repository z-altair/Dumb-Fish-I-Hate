#pragma once
#include "chain.hpp"
#include <algorithm>

class Fish{
    public:
    Chain spine;

    Color main_color = Color(234, 67, 99, 255);
    Color accent_color = Color(87, 49, 145, 255);

    float bodyWidth[10] = {68, 81, 84, 83, 77, 64, 51, 38, 32, 19};

    Fish(Vector2 origin);

    void resolve();

    void display();

    void debugDisplay();

    float getPosX(int i, float angleOffset, float lengthOffset);
    float getPosY(int i, float angleOffset, float lengthOffset);

};
