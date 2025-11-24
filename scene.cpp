#include "scene.h"
#include <math.h>
#include <stdio.h>  
#include <stdlib.h> 


GLuint texturaCorinthians = 0; 

GLuint carregarBMP(const char *imagepath) {
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char *data;

    FILE *file = fopen(imagepath, "rb");
    if (!file) { printf("Imagem não encontrada!\n"); return 0; }

    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        printf("Não é um arquivo BMP válido\n");
        fclose(file);
        return 0;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] data;
    return textureID;
}

int janelaW = 1920;
int janelaH = 1080;
int centerX = janelaW / 2;
int centerY = janelaH / 2;

float camX = 0.0f;
float camY = 1.5f;
float camZ = 25.0f;
float yaw = 0.0f;
float pitch = 0.0f;

bool ignoreWarp = false;
bool firstMouse = true;

bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;

bool fullScreen = false;

void atualizaCentroJanela(int w, int h)
{
    centerX = w / 2;
    centerY = h / 2;
}

void altFullScreen()
{
    firstMouse = true;

    if (!fullScreen)
    {
        glutFullScreen(); // entra no fullscreen
        fullScreen = true;
    }
    else
    {
        glutReshapeWindow(janelaW, janelaH);
        fullScreen = false;
    }
}
