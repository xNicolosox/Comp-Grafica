#include "scene.h"
#include "input.h"
#include <math.h>
#include <cstdio>
// --- NOVOS INCLUDES NECESSÁRIOS PARA LER O ARQUIVO ---
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// -----------------------------------------------------

float anguloPiramide = 0.0f;
float anguloEsfera = 0.0f;
int fps = 0;
int frameCount = 0;
int previousTime = 0;

// ==========================================================
// IMPLEMENTAÇÃO DA FUNÇÃO DE CARREGAR MAPA
// (Tem que vir ANTES da main)
// ==========================================================
void carregarMapaDoArquivo(const char* caminho) {
    std::ifstream arquivo(caminho); // Tenta abrir o arquivo

    if (!arquivo.is_open()) {
        printf("ERRO CRITICO: Nao foi possivel abrir o arquivo '%s'.\n", caminho);
        printf("Verifique se o arquivo mapa.txt esta na mesma pasta do executavel.\n");
        return;
    }

    mapa.clear(); // Limpa qualquer lixo anterior
    std::string linha;
    
    // Lê linha por linha e guarda no vetor
    while (std::getline(arquivo, linha)) {
        mapa.push_back(linha);
    }
    
    arquivo.close();
    printf("Mapa carregado com sucesso! Total de linhas: %d\n", (int)mapa.size());
}
// ==========================================================


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
    desenhaChao(); // O chao infinito
    
    desenhaCenario(); // <--- AQUI ELE DESENHA O MAPA TXT
    
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
    
    // Visão aumentada para ver o céu e o mapa grande
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
    glutCreateWindow("Projeto Shader - DOOM"); 

    // --- 2. Inicializa GLEW ---
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("Erro ao iniciar GLEW: %s\n", glewGetErrorString(err));
        return 1;
    }

    // --- 3. Carrega os Shaders e Luz ---
    initShaders(); 
    setupIluminacao();

    // --- 4. Configurações Globais ---
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); 

    // --- 5. Callbacks ---
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);

    // --- 6. Outras configs ---
    glutSetCursor(GLUT_CURSOR_NONE); 
    glutTimerFunc(0, timer, 0);      

    // --- 7. CARREGA O MAPA ANTES DE ENTRAR NO LOOP ---
    carregarMapaDoArquivo("mapa.txt");

    // --- 8. Loop Principal ---
    glutMainLoop();

    return 0;
}