#include "../include/chain.hpp"
#include "chain.hpp"

Chain::Chain(Vector2 origin, int join_count, float link_size) : Chain(origin, join_count, link_size, PI*2) {};

Chain::Chain(Vector2 origin, int joint_count, float link_size, float max_angle){
    this->link_size = link_size;
    this->max_angle = max_angle;
    joints.push_back(origin);
    angles.push_back(0.0f);
    for (int i = 1; i < joint_count; i++){
        joints.push_back(Vector2Add(joints[i-1], {0, link_size}));
        angles.push_back(0.0f);
    }
}

void Chain::resolve(Vector2 pos){
    Vector2 diff = Vector2Subtract(pos, joints[0]);

    if (Vector2Length(diff) > 0.1f){
        angles[0] = angleFromVector(diff);
    }
    joints[0] = pos;

    for (int i = 1; i < joints.size(); i++){
        float curAngle = angleFromVector(Vector2Subtract(joints[i-1], joints[i]));
        angles[i] = clampAngle(curAngle, angles[i-1], max_angle);
        joints[i] = Vector2Subtract(joints[i-1], Vector2Scale(vectorFromAngle(angles[i]), link_size));
    }
}

void Chain::fabrikResolve(Vector2 pos, Vector2 anchor){
    // Forward pass
    joints[0] = pos;
    for (int i = 1; i < joints.size(); i++){
        joints[i] = clampDistance(joints[i], joints[i-1], link_size);
    }
    
    // Backwards pass
    joints[joints.size() - 1] = anchor;
    for (int i = joints.size() - 2; i >= 0; i--){
        joints[i] = clampDistance(joints[i], joints[i+1], link_size);
    }
}

void Chain::display(){
    for (int i = 0; i < joints.size() - 1; i++){
        DrawLineV(joints[i], joints[i+1], BLACK);
    }
    for (auto&j : joints){
        DrawCircleV(j, 2, WHITE);
    }
}