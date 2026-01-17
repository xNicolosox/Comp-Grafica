#pragma once
#include <GL/glew.h>

struct GameAssets
{
    // texturas
    GLuint texChao = 0;
    GLuint texParede = 0;
    GLuint texSangue = 0;
    GLuint texLava = 0;
    GLuint texChaoInterno = 0;
    GLuint texParedeInterna = 0;
    GLuint texTeto = 0;
    GLuint texEnemy = 0;
    GLuint texEnemyRage = 0;    // NOVO (Viu o player)
    GLuint texEnemyDamage = 0;  // NOVO (Leva dano)
    GLuint texHealth = 0;
    GLuint texAmmo = 0;
    GLuint texGunDefault = 0;
    GLuint texGunFire1 = 0;
    GLuint texGunFire2 = 0;
    GLuint texGunReload1 = 0;
    GLuint texGunReload2 = 0;
    GLuint texDamage = 0;

    // shaders
    GLuint progSangue = 0;
    GLuint progLava = 0;
};

bool loadAssets(GameAssets &a);
