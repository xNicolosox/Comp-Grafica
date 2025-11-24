#ifndef SCENE_H // (se tiver guardas de inclusão)
#define SCENE_H

#include <GL/glut.h>

extern GLuint texturaCorinthians; 

GLuint carregarBMP(const char *imagepath);

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

void desenhaChao(void);
void desenhaTorresELosangos(void);
void desenhaPiramideDegraus(void);

#endif
