#include "../include/fish.hpp"

int main(){
    InitWindow(900, 900, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa");
    SetTargetFPS(60);

    HideCursor();

    Fish lol({400,400});
    while(!WindowShouldClose()){
        lol.resolve();        

        BeginDrawing();
        ClearBackground(GRAY);
        lol.display();

        EndDrawing();
    }
}