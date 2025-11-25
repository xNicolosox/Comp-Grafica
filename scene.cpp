#include "scene.h"
#include <math.h>
#include <stdio.h>  
#include <stdlib.h> 


GLuint texturaCorinthians = 0; 
GLuint texturaChao = 0;
GLuint texturaBola = 0;
GLuint shaderGlow = 0;
GLuint texturaTorre = 0;
GLuint texturaPiramide = 0;
GLuint texturaCeuBackground = 0;

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

void setupIluminacao()
{
    glEnable(GL_LIGHTING); // Liga o sistema de luz
    glEnable(GL_LIGHT0);   // Liga o sol
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Configuração da Luz
    float luzAmbiente[]  = { 0.2f, 0.2f, 0.2f, 1.0f }; // Luz de fundo fraca (pra nada ficar 100% preto)
    float luzDifusa[]    = { 1.0f, 1.0f, 1.0f, 1.0f }; // Cor do sol (Branco)
    float luzEspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Brilho reflexivo
    float posicaoLuz[]   = { 0.0f, 50.0f, 50.0f, 1.0f }; // Posição do sol (No alto)

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

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
// --- CÓDIGO DO VERTEX SHADER (Processa os vértices) ---
const char* vertexShaderSource = 
"#version 120\n" // Versão compatível com glBegin/glEnd
"varying vec3 normal;"
"varying vec3 viewDir;"
"void main() {"
"   gl_FrontColor = gl_Color;" // Repassa a cor definida no glColor3f
"   normal = normalize(gl_NormalMatrix * gl_Normal);" // Calcula normal visual
"   vec4 viewPos = gl_ModelViewMatrix * gl_Vertex;"
"   viewDir = normalize(-viewPos.xyz);" // Vetor apontando para a câmera
"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"}";

// --- CÓDIGO DO FRAGMENT SHADER (Processa as cores/pixels) ---
const char* fragmentShaderSource = 
"#version 120\n"
"varying vec3 normal;"
"varying vec3 viewDir;"
"void main() {"
    // Calcula o 'Rim' (Borda): 1.0 quando a normal é perpendicular à camera
"   float rim = 1.0 - max(dot(normal, viewDir), 0.0);"
"   rim = smoothstep(0.2, 1.0, rim);" // Suaviza e intensifica a borda
"   rim = pow(rim, 3.0);" // Deixa a borda mais fina e brilhante
    
    // Cor do Brilho (Um Ciano meio esverdeado, mude se quiser)
"   vec3 glowColor = vec3(0.0, 1.0, 0.8);" 
    
    // Mistura a cor original do objeto com o brilho
"   vec3 finalColor = gl_Color.rgb + (glowColor * rim * 2.0);"
"   gl_FragColor = vec4(finalColor, gl_Color.a);"
"}";

// Função auxiliar para compilar
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Verifica erros
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERRO NO SHADER: %s\n", infoLog);
    }
    return shader;
}

void initShaders() {
    // 1. Compila os códigos
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // 2. Cria o Programa e Linka
    shaderGlow = glCreateProgram();
    glAttachShader(shaderGlow, vs);
    glAttachShader(shaderGlow, fs);
    glLinkProgram(shaderGlow);

    // Limpeza (já estão no programa, não precisa dos individuais)
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    printf("Shaders inicializados com sucesso!\n");
}
