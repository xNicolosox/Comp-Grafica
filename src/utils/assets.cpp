#include "utils/assets.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include <cstdio>

bool loadAssets(GameAssets &a)
{
    a.texChao = carregaTextura("assets/181.png");
    a.texParede = carregaTextura("assets/091.png");
    a.texSangue = carregaTextura("assets/016.png");
    a.texLava = carregaTextura("assets/179.png");
    a.texChaoInterno = carregaTextura("assets/100.png");
    a.texParedeInterna = carregaTextura("assets/060.png");
    a.texTeto = carregaTextura("assets/081.png");

    a.progSangue = criaShader("shaders/blood.vert", "shaders/blood.frag");
    a.progLava = criaShader("shaders/lava.vert", "shaders/lava.frag");

    // --- INIMIGO 0 (O Original 'E') ---
    a.texEnemies[0]       = carregaTextura("assets/enemy.png");
    a.texEnemiesRage[0]   = carregaTextura("assets/enemyRage.png");
    a.texEnemiesDamage[0] = carregaTextura("assets/enemyRageDamage.png");

    // --- INIMIGO 1 (O Novo 'F') ---
    a.texEnemies[1]       = carregaTextura("assets/enemy2.png");
    a.texEnemiesRage[1]   = carregaTextura("assets/enemyRage2.png");
    a.texEnemiesDamage[1] = carregaTextura("assets/enemyRageDamage2.png");

    // --- INIMIGO 2 (O Novo 'G') ---
    a.texEnemies[2]       = carregaTextura("assets/enemy3.png");
    a.texEnemiesRage[2]   = carregaTextura("assets/enemyRage3.png");
    a.texEnemiesDamage[2] = carregaTextura("assets/enemyRageDamage3.png");


    a.texHealthOverlay = carregaTextura("assets/heal.png");
    a.texGunDefault  = carregaTextura("assets/gun_default.png");
    a.texGunFire1 = carregaTextura("assets/gun_fire1.png");
    a.texGunFire2 = carregaTextura("assets/gun_fire2.png");
    a.texGunReload1 = carregaTextura("assets/gun_reload1.png");
    a.texGunReload2 = carregaTextura("assets/gun_reload2.png");
    a.texDamage = carregaTextura("assets/damage.png");

    a.texHealth = carregaTextura("assets/health.png");
    a.texAmmo = carregaTextura("assets/066.png");

    if (!a.texChao || !a.texParede || !a.texSangue || !a.texLava || !a.progSangue || !a.progLava  || !a.texHealth ||!a.texGunDefault || !a.texGunFire1 || !a.texGunFire2 ||!a.texGunReload1 || !a.texGunReload2 || !a.texDamage || !a.texAmmo || !a.texHealthOverlay ||
        !a.texEnemies[0] || !a.texEnemiesRage[0] || !a.texEnemiesDamage[0] ||
        !a.texEnemies[1] || !a.texEnemiesRage[1] || !a.texEnemiesDamage[1] ||
        !a.texEnemies[2] || !a.texEnemiesRage[2] || !a.texEnemiesDamage[2]) 
    {
        std::printf("ERRO: falha ao carregar algum asset (textura/shader).\n");
        return false;
    }
    return true;
}
