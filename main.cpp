#include "scene.h"
#include "input.h"
#include <math.h>
#include <cstdio>


float anguloPiramide = 0.0f;
float anguloEsfera = 0.0f;
int fps = 0;
int frameCount = 0;
int previousTime = 0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;

    float dirX = cosf(radPitch) * sinf(radYaw);
    float dirY = sinf(radPitch);
    float dirZ = -cosf(radPitch) * cosf(radYaw);

    gluLookAt(
        camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0.0f, 1.0f, 0.0f);

    desenhaCeu();
    desenhaChao();
    desenhaTorresELosangos();
    desenhaPiramideDegraus();

    glutSwapBuffers();

    frameCount++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - previousTime > 1000) // passou 1 segundo
    {
        fps = frameCount;
        frameCount = 0;
        previousTime = currentTime;

        char titulo[64];
        sprintf(titulo, "Cena FPS - %d FPS", fps);
        glutSetWindowTitle(titulo);
    }
}

void reshape(int w, int h)
{
if (h == 0) h = 1;
    float a = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // --- MUDANÇA AQUI: De 100.0f para 2000.0f ---
    gluPerspective(60.0f, a, 1.0f, 2000.0f); 

    glMatrixMode(GL_MODELVIEW);
    atualizaCentroJanela(w, h);
}

void timer(int v)
{
    anguloPiramide += 1.5f;
    if (anguloPiramide >= 360.0f)
        anguloPiramide -= 360.0f;

    anguloEsfera += 1.0f;
    if (anguloEsfera >= 360.0f)
        anguloEsfera -= 360.0f;

    atualizaMovimento();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

int main(int argc, char **argv)
{
    // --- 1. Inicializa GLUT e Cria a Janela ---
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(janelaW, janelaH);
    glutCreateWindow("Projeto Shader - DOOM"); // Escolhi um nome legal ;)

    // --- 2. Inicializa GLEW (TEM QUE SER LOGO APÓS CRIAR A JANELA) ---
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("Erro ao iniciar GLEW: %s\n", glewGetErrorString(err));
        return 1;
    }

    // --- 3. Carrega os Shaders ---
    initShaders(); 
    setupIluminacao();

    // --- 4. Configuraçõesa Globais do OpenGL ---
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Cor do fundo (azul escuro)

    // --- 5. Define as Funções de Callback (Teclado, Mouse, Desenho) ---
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);

    // --- 6. Outras configs ---
    glutSetCursor(GLUT_CURSOR_NONE); // Esconde o cursor
    glutTimerFunc(0, timer, 0);      // Inicia o loop de animação

    // --- 7. Loop Principal ---
    glutMainLoop();

    return 0;
}