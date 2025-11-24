#include <GL/glut.h>
#include <math.h>
#include "scene.h"

#define NUM_TORRES 5
#define RAIO 10.0f // raio das torres ao redor do centro

static void desenhaLosango(float altura)
{
    float h = altura / 2.0f;
    float s = altura / 3.0f;

    float claro[3] = {0.40f, 0.00f, 0.00f};
    float escuro[3] = {0.80f, 0.00f, 0.00f};

    glBegin(GL_TRIANGLES);
    // metade de cima
    glColor3fv(claro);
    glVertex3f(0.0f, h, 0.0f);
    glVertex3f(-s, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, s);

    glColor3fv(escuro);
    glVertex3f(0.0f, h, 0.0f);
    glVertex3f(0.0f, 0.0f, s);
    glVertex3f(s, 0.0f, 0.0f);

    glColor3fv(claro);
    glVertex3f(0.0f, h, 0.0f);
    glVertex3f(s, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -s);

    glColor3fv(escuro);
    glVertex3f(0.0f, h, 0.0f);
    glVertex3f(0.0f, 0.0f, -s);
    glVertex3f(-s, 0.0f, 0.0f);

    // metade de baixo
    glColor3fv(claro);
    glVertex3f(0.0f, -h, 0.0f);
    glVertex3f(0.0f, 0.0f, s);
    glVertex3f(-s, 0.0f, 0.0f);

    glColor3fv(escuro);
    glVertex3f(0.0f, -h, 0.0f);
    glVertex3f(s, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, s);

    glColor3fv(claro);
    glVertex3f(0.0f, -h, 0.0f);
    glVertex3f(0.0f, 0.0f, -s);
    glVertex3f(s, 0.0f, 0.0f);

    glColor3fv(escuro);
    glVertex3f(0.0f, -h, 0.0f);
    glVertex3f(-s, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -s);
    glEnd();
}

void desenhaChao()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-80.0f, 0.0f, -80.0f);
    glVertex3f(80.0f, 0.0f, -80.0f);
    glVertex3f(80.0f, 0.0f, 80.0f);
    glVertex3f(-80.0f, 0.0f, 80.0f);
    glEnd();
}

// Adicione esta função nova para substituir o glutSolidCylinder ---
static void desenhaCilindro(float raio, float altura, int slices)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    // Cria um cilindro no eixo Z (base, topo, altura, fatias, pilhas)
    gluCylinder(quad, raio, raio, altura, slices, 1);
    gluDeleteQuadric(quad);
}

static void desenhaTubo(float x1, float y1, float x2, float y2, float espessura)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float comprimento = sqrt(dx * dx + dy * dy);
    
    float angulo = atan2(dy, dx) * 180.0f / M_PI;

    glPushMatrix();
    glTranslatef(x1, y1, 0.0f); 
    glRotatef(angulo, 0.0f, 0.0f, 1.0f); // Gira no plano XY
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Gira para o eixo X
    
    desenhaCilindro(espessura, comprimento, 10);
    
    glPopMatrix();
}

static void desenhaBicicleta()
{
    float angulo = -glutGet(GLUT_ELAPSED_TIME) * 0.5f; 
    
    float grossuraQuadro = 0.04f; 
    float grossuraGuidao = 0.03f;

    // --- Rodas ---
    glColor3f(0.3f, 0.3f, 0.3f); 

    // Roda Traseira
    glPushMatrix();
        glTranslatef(-0.8f, 0.35f, 0.0f);
        glRotatef(angulo, 0.0f, 0.0f, 1.0f);
        glutSolidTorus(0.05, 0.35, 10, 30);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            for(int i = 0; i < 8; i++) {
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.35f * cos(i * 3.14 / 4.0), 0.35f * sin(i * 3.14 / 4.0), 0.0f);
            }
        glEnd();
    glPopMatrix();

    // Roda Dianteira
    glColor3f(0.3f, 0.3f, 0.3f); 
    glPushMatrix();
        glTranslatef(0.8f, 0.35f, 0.0f);
        glRotatef(angulo, 0.0f, 0.0f, 1.0f);
        glutSolidTorus(0.05, 0.35, 10, 30);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            for(int i = 0; i < 8; i++) {
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.35f * cos(i * 3.14 / 4.0), 0.35f * sin(i * 3.14 / 4.0), 0.0f);
            }
        glEnd();
    glPopMatrix();


    // --- Quadro (Vermelho) ---
    glColor3f(1.0f, 0.0f, 0.0f); 
    
    // Triângulo Traseiro
    desenhaTubo(-0.8f, 0.35f, -0.2f, 0.75f, grossuraQuadro); 
    desenhaTubo(-0.2f, 0.75f, 0.0f, 0.2f, grossuraQuadro);   
    desenhaTubo(0.0f, 0.2f, -0.8f, 0.35f, grossuraQuadro);   

    // Triângulo Principal
    desenhaTubo(0.0f, 0.2f, 0.5f, 0.7f, grossuraQuadro);     
    desenhaTubo(0.5f, 0.7f, -0.2f, 0.75f, grossuraQuadro);   

    // Garfo da frente
    desenhaTubo(0.5f, 0.7f, 0.8f, 0.35f, grossuraQuadro);

    // --- Guidão (Preto) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    
    // Haste vertical
    desenhaTubo(0.5f, 0.7f, 0.45f, 0.9f, grossuraGuidao);

    // Barra horizontal do guidão
    glPushMatrix();
        glTranslatef(0.45f, 0.9f, -0.3f); 
        desenhaCilindro(grossuraGuidao, 0.6f, 10); 
    glPopMatrix();

    // Selim
    glPushMatrix();
        glTranslatef(-0.2f, 0.8f, 0.0f); 
        glScalef(0.25f, 0.05f, 0.15f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // --- Pedais ---
    glPushMatrix();
        glTranslatef(0.0f, 0.2f, 0.0f); 
        glRotatef(angulo, 0.0f, 0.0f, 1.0f); 

        glColor3f(0.0f, 0.0f, 0.0f); 
        
        // Haste central dos pedais
         glPushMatrix();
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f); 
            glScalef(0.04f, 0.4f, 0.04f); 
            glutSolidCube(1.0f);
         glPopMatrix();
         
         // Eixo horizontal
         glPushMatrix();
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.04f, 0.25f, 0.04f);
            glutSolidCube(1.0f);
         glPopMatrix();

        // Placas dos pedais
        glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.1f);
            glScalef(0.1f, 0.02f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, -0.2f, -0.1f);
            glScalef(0.1f, 0.02f, 0.1f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();
}
void desenhaTorresELosangos()
{
    float alturaTorre = 2.5f;
    float w = 0.7f;
    float ang0 = -M_PI / 2.0f;
    float passo = 2.0f * M_PI / NUM_TORRES;

    for (int i = 0; i < NUM_TORRES; i++)
    {
        float ang = ang0 + passo * i;
        float x = RAIO * cosf(ang);
        float z = RAIO * sinf(ang);

        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        if (i == 3) 
        {
            glPushMatrix();
            // Ajusta posição para encostar na base da torre
            glTranslatef(0.10f, 0.0f, 0.9f); 
            
            // Gira a bike
            glRotatef(23.0f, 0.0f, 4.0f, 0.0f); 
            
            // Inclina ela como se estivesse apoiada)
            glRotatef(-15.0f, 8.0f, 0.0f, 0.0f);

            desenhaBicicleta();
            glPopMatrix();
        }
        

        // Torre roxa
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glTranslatef(0.0f, alturaTorre / 2.0f, 0.0f);
        glScalef(w, alturaTorre, w);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Losango branco girando em cima 
        glPushMatrix();
        glTranslatef(0.0f, alturaTorre + 1.2f, 0.0f);
        glRotatef(anguloPiramide, 0.0f, 1.0f, 0.0f);
        desenhaLosango(1.5f);
        glPopMatrix();

        glPopMatrix();
    }
}

void desenhaPiramideDegraus()
{
    float alturaDegrau = 0.5f;
    float tamanhoBase = 6.0f;
    float reducao = 0.65f;

    glPushMatrix();


    // Degrau 1
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase, alturaDegrau, tamanhoBase);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Degrau 2
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, alturaDegrau + alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase * reducao, alturaDegrau, tamanhoBase * reducao);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Degrau 3
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 2 * alturaDegrau + alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase * reducao * reducao,
             alturaDegrau,
             tamanhoBase * reducao * reducao);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Esfera flutuando acima
    float topoDegrausY = 5.0f * alturaDegrau;
    float raioEsfera = 3.0f;

    glPushMatrix();
    glTranslatef(0.0f, topoDegrausY + raioEsfera + 0.2f, 0.0f);
    
    // coordenadas da esfera (para o escudo)
    float xEsfera = 0.0f;
    float yEsfera = topoDegrausY + raioEsfera + 0.2f;
    float zEsfera = 0.0f;

    glRotatef(anguloEsfera, 1.0f, 1.5f, 0.0f);

    double eq[4];

    // metade de cima
    glColor3f(1.0f, 1.0f, 1.0f);
    eq[0] = 0;
    eq[1] = -1;
    eq[2] = 0;
    eq[3] = 0;
    glClipPlane(GL_CLIP_PLANE0, eq);
    glEnable(GL_CLIP_PLANE0);
    glutSolidSphere(raioEsfera, 40, 40);
    glDisable(GL_CLIP_PLANE0);

    // metade de baixo
    glColor3f(0.00f, 0.00f, 0.00f);
    eq[0] = 0;
    eq[1] = 1;
    eq[2] = 0;
    eq[3] = 0;
    glClipPlane(GL_CLIP_PLANE1, eq);
    glEnable(GL_CLIP_PLANE1);
    glutSolidSphere(raioEsfera, 40, 40);
    glDisable(GL_CLIP_PLANE1);

    glPopMatrix();

    glPopMatrix();

    //             ESCUDO DO CORINTHIANS
    if (texturaCorinthians == 0) {
        texturaCorinthians = carregarBMP("logo.bmp");
    }

    if (texturaCorinthians != 0) 
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaCorinthians);
        
        glColor3f(1.0f, 1.0f, 1.0f); // Cor branca

        glPushMatrix();
        
        glTranslatef(0.0f, yEsfera + raioEsfera + 2.5f, 0.0f);
        
        // Rotação
        glRotatef(anguloEsfera, 0.0f, 1.0f, 0.0f);

        // Desenha
        float tamanhoLogo = 2.0f;
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-tamanhoLogo, -tamanhoLogo, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( tamanhoLogo, -tamanhoLogo, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( tamanhoLogo,  tamanhoLogo, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamanhoLogo,  tamanhoLogo, 0.0f);
        glEnd();

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}
