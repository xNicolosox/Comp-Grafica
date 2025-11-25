#ifndef SCENE_H 
#define SCENE_H

#include <GL/glew.h>
#include <GL/glut.h>

extern GLuint texturaCorinthians; 
extern GLuint texturaChao;
extern GLuint texturaBola;       
extern GLuint shaderGlow;
extern GLuint texturaTorre;
extern GLuint texturaPiramide;
extern GLuint texturaCeuBackground;

GLuint carregarBMP(const char *imagepath);
void initShaders();
extern float anguloPiramide;
extern float anguloEsfera;

extern float camX, camY, camZ;
extern float yaw, pitch;

extern int janelaW;
extern int janelaH;

extern int centerX;
extern int centerY;

extern bool fullScreen;

extern bool ignoreWarp;
extern bool firstMouse;

extern bool keyW;
extern bool keyA;
extern bool keyS;
extern bool keyD;

void altFullScreen();
void atualizaCentroJanela(int w, int h);

void setupIluminacao();
void desenhaCeu();
void desenhaChao(void);
void desenhaTorresELosangos(void);
void desenhaPiramideDegraus(void);

#endif
