#include "../include/fish.hpp"

Fish::Fish(Vector2 origin)
: spine(origin, 12, 32, PI/8){}

void Fish::resolve(){
    Vector2 head_pos = spine.joints[0];
    Vector2 mouse_pos = GetMousePosition();
    Vector2 diff = Vector2Subtract(mouse_pos, head_pos);

    float distance = Vector2Length(diff);
    float speed = 6.0f;

    Vector2 target;
    if (distance > speed) {
        // Si el ratón está lejos, avanzar hacia él a la velocidad máxima
        target = Vector2Add(head_pos, setVectorMagnitude(diff, speed));
    } else {
        // Si estamos a punto de tocar el ratón, ir exactamente a él sin pasarnos
        target = mouse_pos; 
    }

    spine.resolve(target);
}


void Fish::display() {
    // ==========================================
    // ⚙️ PANEL DE PERSONALIZACIÓN
    // ==========================================
    
    // Colores
    Color bodyFillColor = main_color;
    Color bodyOutColor  = BLACK;
    Color finFillColor  = accent_color;
    Color finOutColor   = BLACK;
    Color eyeColor      = WHITE;
    Color pupilColor    = BLACK;

    // Grosores
    float outlineThick  = 4.0f;

    // Aletas Pectorales (Lados - Articulación 3)
    float pecRadiusX    = 80.0f;
    float pecRadiusY    = 32.0f;
    float pecOffsetAng  = PI/3;  // Apertura
    float pecRotOffset  = PI/4;  // Rotación sobre sí mismas

    // Aletas Ventrales (Abajo - Articulación 7)
    float venRadiusX    = 48.0f;
    float venRadiusY    = 16.0f;
    float venOffsetAng  = PI/2;
    float venRotOffset  = PI/4;

    // Ojos (Cabeza - Articulación 0)
    float eyeRadius     = 12.0f;
    float pupilRadius   = 5.0f;
    float eyeDistSide   = PI/2; // Distancia hacia los lados
    float eyeDistFront  = 5.0f; // Offset hacia el hocico
    
    // ==========================================
    // 🧮 CÁLCULOS PREVIOS
    // ==========================================
    std::vector<Vector2>& j = spine.joints;
    std::vector<float>& a = spine.angles;
    
    float headToMid1 = relativeAngleDifference(a[6], a[0]); 
    float headToMid2 = relativeAngleDifference(a[7], a[0]);
    float headToTail = headToMid1 + relativeAngleDifference(a[11], a[6]);

    // OJO: El orden de dibujado importa (de atrás hacia adelante)

    // ==========================================
    // 1. ALETAS VENTRALES Y PECTORALES (Fondo)
    // ==========================================
    Vector2 rightVenPos = { getPosX(7, venOffsetAng, 0), getPosY(7, venOffsetAng, 0) };
    DrawEllipseCustom(rightVenPos, venRadiusX, venRadiusY, (a[6] - venRotOffset) * RAD2DEG, finFillColor, finOutColor, outlineThick); 

    Vector2 leftVenPos  = { getPosX(7, -venOffsetAng, 0), getPosY(7, -venOffsetAng, 0) };
    DrawEllipseCustom(leftVenPos, venRadiusX, venRadiusY, (a[6] + venRotOffset) * RAD2DEG, finFillColor, finOutColor, outlineThick);  

    Vector2 rightPecPos = { getPosX(3, pecOffsetAng, 0), getPosY(3, pecOffsetAng, 0) };
    DrawEllipseCustom(rightPecPos, pecRadiusX, pecRadiusY, (a[2] - pecRotOffset) * RAD2DEG, finFillColor, finOutColor, outlineThick); 

    Vector2 leftPecPos  = { getPosX(3, -pecOffsetAng, 0), getPosY(3, -pecOffsetAng, 0) };
    DrawEllipseCustom(leftPecPos, pecRadiusX, pecRadiusY, (a[2] + pecRotOffset) * RAD2DEG, finFillColor, finOutColor, outlineThick);  

    // ==========================================
    // 2. ALETA CAUDAL (COLA)
    // ==========================================
    std::vector<Vector2> tailPoints;

    // Generamos el perímetro de la cola
    for (int i = 8; i <= 11; i++) { // Abajo
        float tailWidth = 1.5f * headToTail * (i - 8) * (i - 8);
        tailPoints.push_back({ j[i].x + cos(a[i] - PI/2) * tailWidth, j[i].y + sin(a[i] - PI/2) * tailWidth });
    }
    for (int i = 11; i >= 8; i--) { // Arriba
        float tailWidth = std::clamp(headToTail * 6.0f, -13.0f, 13.0f);
        tailPoints.push_back({ j[i].x + cos(a[i] + PI/2) * tailWidth, j[i].y + sin(a[i] + PI/2) * tailWidth });
    }

    // --- RELLENO DE LA COLA ---
    // Creamos un abanico usando la base de la cola (j[8]) como centro.
    // Esto asegura que el relleno siga perfectamente los puntos del borde.
    std::vector<Vector2> tailFill = { j[8] }; 
    tailFill.insert(tailFill.end(), tailPoints.begin(), tailPoints.end());
    DrawTriangleFan(tailFill.data(), tailFill.size(), finFillColor);

    // --- BORDE DE LA COLA ---
    // TRUCO VITAL: Para que la curva Catmull-Rom se cierre perfectamente sobre sí misma
    // y no "vaya a su bola", tenemos que repetir los primeros 3 puntos al final.
    tailPoints.push_back(tailPoints[0]);
    tailPoints.push_back(tailPoints[1]);
    tailPoints.push_back(tailPoints[2]);
    
    DrawSplineCatmullRom(tailPoints.data(), tailPoints.size(), outlineThick, finOutColor);


    // ==========================================
    // 3. CUERPO (Cubre las bases de las aletas)
    // ==========================================
    std::vector<Vector2> bodyPoints;     
    std::vector<Vector2> bodyFillPoints; 

    // Puntos para el relleno central (ZigZag)
    for (int i = 0; i < 10; i++) {
        bodyFillPoints.push_back({getPosX(i, PI/2, 0), getPosY(i, PI/2, 0)});   // Derecha
        bodyFillPoints.push_back({getPosX(i, -PI/2, 0), getPosY(i, -PI/2, 0)}); // Izquierda
    }
    
    // --- NUEVO: RELLENO ESPECÍFICO DEL MORRO ---
    // En lugar de un círculo, hacemos un abanico desde el centro de la cabeza (j[0]) 
    // que siga exactamente la forma de la boca y el hocico que tiene tu outline.
    std::vector<Vector2> snoutFill;
    snoutFill.push_back(j[0]);                                         // Centro
    snoutFill.push_back({getPosX(0, PI/2, 0), getPosY(0, PI/2, 0)});   // Lado derecho
    snoutFill.push_back({getPosX(0, PI/6, 0), getPosY(0, PI/6, 0)});   // Diagonal derecha
    snoutFill.push_back({getPosX(0, 0, 4), getPosY(0, 0, 4)});         // Punta del hocico
    snoutFill.push_back({getPosX(0, -PI/6, 0), getPosY(0, -PI/6, 0)}); // Diagonal izquierda
    snoutFill.push_back({getPosX(0, -PI/2, 0), getPosY(0, -PI/2, 0)}); // Lado izquierdo

    // Puntos para el borde perimetral del cuerpo
    for (int i = 0; i < 10; i++) bodyPoints.push_back({getPosX(i, PI/2, 0), getPosY(i, PI/2, 0)});
    bodyPoints.push_back({getPosX(9, PI, 0), getPosY(9, PI, 0)});
    for (int i = 9; i >= 0; i--) bodyPoints.push_back({getPosX(i, -PI/2, 0), getPosY(i, -PI/2, 0)});

    // Puntos para cerrar la cabeza en el outline
    bodyPoints.push_back({getPosX(0, -PI/6, 0), getPosY(0, -PI/6, 0)});
    bodyPoints.push_back({getPosX(0, 0, 4), getPosY(0, 0, 4)});
    bodyPoints.push_back({getPosX(0, PI/6, 0), getPosY(0, PI/6, 0)});
    bodyPoints.push_back({getPosX(0, PI/2, 0), getPosY(0, PI/2, 0)});
    bodyPoints.push_back({getPosX(1, PI/2, 0), getPosY(1, PI/2, 0)});
    bodyPoints.push_back({getPosX(2, PI/2, 0), getPosY(2, PI/2, 0)});

    // 1. Dibujamos los rellenos primero (Centro, Hocico y Tapa de la cola)
    DrawTriangleStrip(bodyFillPoints.data(), bodyFillPoints.size(), bodyFillColor);
    DrawTriangleFan(snoutFill.data(), snoutFill.size(), bodyFillColor); // <--- Esto rellena el morro
    DrawCircleV(j[9], bodyWidth[9] / 2.0f, bodyFillColor); 
    
    // 2. Dibujamos el outline por encima de todo
    DrawSplineCatmullRom(bodyPoints.data(), bodyPoints.size(), outlineThick, bodyOutColor);

    // ==========================================
    // 4. ALETA DORSAL (Encima del cuerpo)
    // ==========================================
    Vector2 p1 = j[4];
    Vector2 p2 = j[5]; 
    Vector2 p3 = j[6]; 
    Vector2 p4 = j[7]; 
    Vector2 dorsalPoints[] = { p1, p2, p3, p4 };
    
    // Relleno rudimentario de la dorsal (Uniendo la curva con la espina)
    std::vector<Vector2> dorsalFill;
    dorsalFill.push_back(p1); // Base inicio
    dorsalFill.push_back(p4); // Base fin
    // Aproximamos la curva Bezier para rellenarla
    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;
        float u = 1.0f - t;
        // Fórmula matemática manual de Bezier Cúbica (compatible con cualquier versión de Raylib)
        Vector2 pt = Vector2Add(Vector2Add(Vector2Scale(p1, u*u*u), Vector2Scale(p2, 3*u*u*t)), 
                     Vector2Add(Vector2Scale(p3, 3*u*t*t), Vector2Scale(p4, t*t*t)));
        dorsalFill.push_back(pt);
    }
    DrawTriangleFan(dorsalFill.data(), dorsalFill.size(), finFillColor);
    DrawSplineBezierCubic(dorsalPoints, 4, outlineThick, finOutColor); // Borde

    // ==========================================
    // 5. OJOS Y DETALLES FINALES
    // ==========================================
    Vector2 rightEyePos = { getPosX(0, eyeDistSide, eyeDistFront), getPosY(0, eyeDistSide, eyeDistFront) };
    Vector2 leftEyePos  = { getPosX(0, -eyeDistSide, eyeDistFront), getPosY(0, -eyeDistSide, eyeDistFront) };

    // Globo ocular (Borde + Relleno)
    DrawCircleV(rightEyePos, eyeRadius + outlineThick/2, bodyOutColor);
    DrawCircleV(rightEyePos, eyeRadius, eyeColor);
    DrawCircleV(leftEyePos, eyeRadius + outlineThick/2, bodyOutColor);
    DrawCircleV(leftEyePos, eyeRadius, eyeColor);

    // Pupilas
    DrawCircleV(rightEyePos, pupilRadius, pupilColor);
    DrawCircleV(leftEyePos, pupilRadius, pupilColor);
}


void Fish::debugDisplay(){
    spine.display();
}

float Fish::getPosX(int i, float angleOffset, float lengthOffset){
    return spine.joints[i].x + cos(spine.angles[i] + angleOffset) * bodyWidth[i] + lengthOffset;
}

float Fish::getPosY(int i, float angleOffset, float lengthOffset){
    return spine.joints[i].y + sin(spine.angles[i] + angleOffset) * bodyWidth[i] + lengthOffset;
}
