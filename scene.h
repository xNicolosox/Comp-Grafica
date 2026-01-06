#ifndef SCENE_H 
#define SCENE_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
extern std::vector<std::string> mapa; // "Aviso: existe um mapa em algum lugar"

extern GLuint texturaCorinthians; 
extern GLuint texturaChao;
extern GLuint texturaBola;       
extern GLuint shaderGlow;
extern GLuint texturaTorre;
extern GLuint texturaPiramide;
extern GLuint texturaCeuBackground;
extern GLuint texParede1;
extern GLuint texParede2;
extern GLuint texPiso1;
extern GLuint texPiso2;
extern GLuint texTeto;

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
void desenhaCenario();

#endif
