#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include "graphics/drawlevel.h"
#include "core/camera.h"
#include "core/entities.h"
#include "level/levelmetrics.h"
#include <cstdio>


extern GLuint texParede;
extern GLuint texParedeInterna;
extern GLuint texLava;
extern GLuint texSangue;
extern GLuint texChao;
extern GLuint texChaoInterno;
extern GLuint texTeto;
extern GLuint texEnemy;
extern GLuint texEnemyRage;
extern GLuint texEnemyDamage;
extern GLuint texHealth;
extern GLuint texAmmo;

extern GLuint progLava;
extern GLuint progSangue;

// Controle de tempo
extern float tempo;

// Config do grid
static const float TILE = 4.0f;      // tamanho do tile no mundo (ajuste)
static const float CEILING_H = 4.0f; // altura do teto
static const float WALL_H = 4.0f;    // altura da parede
static const float EPS_Y = 0.001f;   // evita z-fighting

static const GLfloat kAmbientOutdoor[] = {0.45f, 0.30f, 0.25f, 1.0f}; // quente (seu atual)
static const GLfloat kAmbientIndoor[] = {0.12f, 0.12f, 0.18f, 1.0f};  // frio/azulado

static void bindTexture0(GLuint tex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
}

static float hash01(float x)
{
    float s = sinf(x * 12.9898f) * 43758.5453f;
    return s - floorf(s);
}

static float flickerFluorescente(float t)
{
    const float rate = 4.0f; // era 9.0
    float block = floorf(t * rate);
    float r = hash01(block);

    if (r < 0.22f) // era 0.12
    {
        float phase = t * rate - block;

        // apagão mais longo
        if (phase > 0.35f && phase < 0.55f)
            return 0.12f; // quase apaga

        // as vezes um segundo tranco
        if (r < 0.06f && phase > 0.65f && phase < 0.78f)
            return 0.40f;
    }

    return 0.96f + 0.04f * sinf(t * 5.0f);
}

static void setIndoorLampAt(float x, float z, float intensity)
{
    // posição da lâmpada (pontual)
    GLfloat pos[] = {x, CEILING_H - 0.05f, z, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, pos);

    GLfloat diff[] = {
        1.20f * intensity,
        1.22f * intensity,
        1.28f * intensity,
        1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);

    GLfloat amb[] = {
        1.10f * intensity,
        1.10f * intensity,
        1.12f * intensity,
        1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
}

static void beginIndoor(float wx, float wz)
{
    // sol NÃO entra
    glDisable(GL_LIGHT0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, kAmbientIndoor); // esfria o ambiente

    // lâmpada fria entra
    glEnable(GL_LIGHT1);

    float f = flickerFluorescente(tempo);
    float intensity = 1.2f * f;

    setIndoorLampAt(wx, wz, intensity);
}

static void endIndoor()
{
    glDisable(GL_LIGHT1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, kAmbientOutdoor);

    glEnable(GL_LIGHT0);
}

static void desenhaQuadTeto(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); // NORMAL DO TETO

    // note a ordem invertida
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, CEILING_H, z - half);
    glTexCoord2f(tilesUV, 0.0f);
    glVertex3f(x + half, CEILING_H, z - half);
    glTexCoord2f(tilesUV, tilesUV);
    glVertex3f(x + half, CEILING_H, z + half);
    glTexCoord2f(0.0f, tilesUV);
    glVertex3f(x - half, CEILING_H, z + half);
    glEnd();
}

static void desenhaQuadChao(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f); // NORMAL DO CHÃO

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, 0.0f);
    glVertex3f(x + half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, tilesUV);
    glVertex3f(x + half, EPS_Y, z - half);
    glTexCoord2f(0.0f, tilesUV);
    glVertex3f(x - half, EPS_Y, z - half);
    glEnd();
}

static void desenhaTileChao(float x, float z, GLuint texChaoX, bool temTeto)
{
    glUseProgram(0); // sem shader
    glColor3f(1, 1, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texChaoX);

    // chão
    desenhaQuadChao(x, z, TILE, 2.0f);

    // teto
    if (temTeto)
    {
        glBindTexture(GL_TEXTURE_2D, texTeto);
        desenhaQuadTeto(x, z, TILE, 2.0f);
    }
}

static void desenhaParede(float x, float z, GLuint texParedeX)
{
    float half = TILE * 0.5f;

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texParedeX);

    float tilesX = 1.0f;
    float tilesY = 2.0f;

    glBegin(GL_QUADS);

    // Frente (z+)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x + half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x - half, WALL_H, z + half);

    // Trás (z-)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x - half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x + half, WALL_H, z - half);

    // Direita (x+)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x + half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x + half, WALL_H, z + half);

    // Esquerda (x-)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x - half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x - half, WALL_H, z - half);

    // Topo
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, WALL_H, z + half);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + half, WALL_H, z + half);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + half, WALL_H, z - half);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x - half, WALL_H, z - half);

    glEnd();
}

static void desenhaTileLava(float x, float z)
{
    glUseProgram(progLava);

    GLint locTime = glGetUniformLocation(progLava, "uTime");
    GLint locStr = glGetUniformLocation(progLava, "uStrength");
    GLint locScr = glGetUniformLocation(progLava, "uScroll");
    GLint locHeat = glGetUniformLocation(progLava, "uHeat");
    GLint locTex = glGetUniformLocation(progLava, "uTexture");

    glUniform1f(locTime, tempo);
    glUniform1f(locStr, 1.0f);
    glUniform2f(locScr, 0.1f, 0.0f);
    glUniform1f(locHeat, 0.6f);

    bindTexture0(texLava);
    glUniform1i(locTex, 0);

    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);

    glUseProgram(0);
}

static void desenhaTileSangue(float x, float z)
{
    glUseProgram(progSangue);

    GLint locTime = glGetUniformLocation(progSangue, "uTime");
    GLint locStr = glGetUniformLocation(progSangue, "uStrength");
    GLint locSpd = glGetUniformLocation(progSangue, "uSpeed");
    GLint locTex = glGetUniformLocation(progSangue, "uTexture");

    glUniform1f(locTime, tempo);
    glUniform1f(locStr, 1.0f);
    glUniform2f(locSpd, 2.0f, 1.3f);

    bindTexture0(texSangue);
    glUniform1i(locTex, 0);

    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);

    glUseProgram(0);
}

void drawLevel(const MapLoader &map)
{
    const auto &data = map.data();
    int H = map.getHeight();

    // centraliza o mapa no mundo
    LevelMetrics m = LevelMetrics::fromMap(map, TILE);

    for (int z = 0; z < H; z++)
    {
        for (int x = 0; x < (int)data[z].size(); x++)
        {
            float wx, wz;
            m.tileCenter(x, z, wx, wz); // centro do tile

            char c = data[z][x];

            // Se for entidade, desenha o chão embaixo dela
            if (c == 'E' || c == 'H' || c == 'A') {
                 desenhaTileChao(wx, wz, texChao, false);
                 // O inimigo/item em si será desenhado DEPOIS, em outra função
                 // que faremos na próxima etapa (Billboarding)
            }
            // TIRAR A RESPOSNABILIDADE DO TILE DAQUI
            if (c == '0') // chão A (outdoor)
                desenhaTileChao(wx, wz, texChao, false);
            else if (c == '3') // chão B (indoor, tem teto)
            {
                beginIndoor(wx, wz);
                desenhaTileChao(wx, wz, texChaoInterno, true);
                endIndoor();
            }
            else if (c == '1') // parede A (outdoor)
                desenhaParede(wx, wz, texParede);
            else if (c == '2') // parede B (indoor)
            {
                beginIndoor(wx, wz);
                desenhaParede(wx, wz, texParedeInterna);
                endIndoor();
            }
            else if (c == 'L')
            {
                // lava é shader: não pega GL_LIGHT0/1.
                // Se quiser “indoor” com lava escura/clara, a gente ajusta no shader depois.
                desenhaTileLava(wx, wz);
            }
            else if (c == 'B')
            {
                desenhaTileSangue(wx, wz);
            }
        }
    }
}

static void drawSprite(float x, float z, float w, float h, GLuint tex, float camX, float camZ)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Habilita teste alpha para descartar pixels transparentes (recorte)
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    glBindTexture(GL_TEXTURE_2D, tex);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, 0.0f, z); // Vai para a posição do objeto

    // MATEMÁTICA DO BILLBOARD (Olhar para a câmera)
    // Calcula o ângulo entre o objeto e a câmera
    float dx = camX - x;
    float dz = camZ - z;
    float angle = std::atan2(dx, dz) * 180.0f / 3.14159f;
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Gira no eixo Y

    // Desenha o quadrado centralizado
    float hw = w * 0.5f;
    
    glBegin(GL_QUADS);
    // Normal apontando pro jogador
    glNormal3f(0, 0, 1); 
    
    // --- CÓDIGO CORRIGIDO (INVERTIDO U e V) ---
    // Onde era 0.0 virou 1.0 e vice-versa, para desvirar e desespelhar.
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, 0.0f, 0.0f); // Pé esquerdo (no mundo)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hw, 0.0f, 0.0f);  // Pé direito
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hw, h, 0.0f);     // Cabeça direita
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-hw, h, 0.0f);    // Cabeça esquerda
    // ------------------------------------------
    glEnd();

    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

// Essa é a função principal que vamos chamar no gameRender
// Precisamos passar a lista de inimigos e itens
void drawEntities(const std::vector<Enemy>& enemies, const std::vector<Item>& items, float camX, float camZ)
{



    glDisable(GL_LIGHTING);

    // Desenha Itens
    for (const auto& item : items)
    {
        if (!item.active) continue;

        if (item.type == ITEM_HEALTH)
            drawSprite(item.x, item.z, 0.7f, 0.7f, texHealth, camX, camZ);
        else if (item.type == ITEM_AMMO)
            drawSprite(item.x, item.z, 0.7f, 0.7f, texAmmo, camX, camZ); 
    }

    // Desenha Inimigos
    for (const auto& en : enemies)
    {
        if (en.state == STATE_DEAD) continue;
        
        // 1. Define o padrão (Inimigo calmo)
        GLuint currentTex = texEnemy; 

        // 2. Se tomou dano (Prioridade Máxima)
        if (en.hurtTimer > 0.0f) {
            currentTex = texEnemyDamage;
        }
        // 3. Se está perseguindo ou atacando (Prioridade Média)
        else if (en.state == STATE_CHASE || en.state == STATE_ATTACK) {
            currentTex = texEnemyRage;
        }


        // 4. DESENHA USANDO A VARIÁVEL ESCOLHIDA (currentTex)
        drawSprite(en.x, en.z, 4.5f, 4.5f, currentTex, camX, camZ);
    }

    glEnable(GL_LIGHTING);
}