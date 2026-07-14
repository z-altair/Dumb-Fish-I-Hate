#pragma once
#include <vector>
#include "util.hpp"
#include "../lib/raylib/include/raylib.h"
#include "../lib/raylib/include/raymath.h"
#include "../lib/raylib/include/rlgl.h"

class Chain{
    public:
    // Trozos de la cadena
    std::vector<Vector2> joints;
    // Distancia entre los trozos de cadena 
    float link_size;

    // no se
    std::vector<float> angles;
    float max_angle;

    Chain(Vector2 origin, int join_count, float link_size);

    Chain(Vector2 origin, int joint_count, float link_size, float max_angle);

    void resolve(Vector2 pos);
    void fabrikResolve(Vector2 pos, Vector2 anchor);

    void display();

};