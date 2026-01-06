#include "scene.h"
#include <math.h>

#define NUM_TORRES 5
#define RAIO 10.0f // raio das torres ao redor do centro

static void desenhaLosango(float altura)
{
    // 1. LIGA O SHADER DE BRILHO (Se ele foi criado com sucesso)
    if (shaderGlow != 0) {
        glUseProgram(shaderGlow);
    }

    float h = altura / 2.0f;
    float s = altura / 3.0f;

    float claro[3] = {1.0f, 0.843f, 0.0f};
    float escuro[3] = {1.0f, 0.766f, 0.0f};

    glBegin(GL_TRIANGLES);

    // --- METADE DE CIMA (Normais apontam para cima Y+) ---

    // Face 1 (Esquerda-Frente) -> Normal aponta para Esquerda/Cima/Frente
    glNormal3f(-1.0f, 1.0f, 1.0f);
    glColor3fv(claro);  glVertex3f(0.0f, h, 0.0f); glVertex3f(-s, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, s);

    // Face 2 (Direita-Frente) -> Normal aponta para Direita/Cima/Frente
    glNormal3f(1.0f, 1.0f, 1.0f);
    glColor3fv(escuro); glVertex3f(0.0f, h, 0.0f); glVertex3f(0.0f, 0.0f, s); glVertex3f(s, 0.0f, 0.0f);

    // Face 3 (Direita-Trás) -> Normal aponta para Direita/Cima/Trás
    glNormal3f(1.0f, 1.0f, -1.0f);
    glColor3fv(claro);  glVertex3f(0.0f, h, 0.0f); glVertex3f(s, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, -s);

    // Face 4 (Esquerda-Trás) -> Normal aponta para Esquerda/Cima/Trás
    glNormal3f(-1.0f, 1.0f, -1.0f);
    glColor3fv(escuro); glVertex3f(0.0f, h, 0.0f); glVertex3f(0.0f, 0.0f, -s); glVertex3f(-s, 0.0f, 0.0f);

    // --- METADE DE BAIXO (Normais apontam para baixo Y-) ---

    // Face 5 (Esquerda-Frente Baixo)
    glNormal3f(-1.0f, -1.0f, 1.0f);
    glColor3fv(claro);  glVertex3f(0.0f, -h, 0.0f); glVertex3f(0.0f, 0.0f, s); glVertex3f(-s, 0.0f, 0.0f);

    // Face 6 (Direita-Frente Baixo)
    glNormal3f(1.0f, -1.0f, 1.0f);
    glColor3fv(escuro); glVertex3f(0.0f, -h, 0.0f); glVertex3f(s, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, s);

    // Face 7 (Direita-Trás Baixo)
    glNormal3f(1.0f, -1.0f, -1.0f);
    glColor3fv(claro);  glVertex3f(0.0f, -h, 0.0f); glVertex3f(0.0f, 0.0f, -s); glVertex3f(s, 0.0f, 0.0f);

    // Face 8 (Esquerda-Trás Baixo)
    glNormal3f(-1.0f, -1.0f, -1.0f);
    glColor3fv(escuro); glVertex3f(0.0f, -h, 0.0f); glVertex3f(-s, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, -s);

    glEnd();

    // 2. DESLIGA O SHADER (Para não aplicar glow no chão, na bike, etc)
    glUseProgram(0);
}

static void desenhaCilindro(float raio, float altura, int slices)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
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
    glRotatef(angulo, 0.0f, 0.0f, 1.0f); 
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    desenhaCilindro(espessura, comprimento, 10);
    glPopMatrix();
}

static void desenhaBicicleta()
{
    float angulo = -glutGet(GLUT_ELAPSED_TIME) * 0.5f; 
    float grossuraQuadro = 0.04f; 
    float grossuraGuidao = 0.03f;

    // --- Rodas ---
    glColor3f(0.0f, 0.0f, 0.0f); 

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
    glColor3f(0.0f, 0.0f, 0.0f); 
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
    desenhaTubo(-0.8f, 0.35f, -0.2f, 0.75f, grossuraQuadro); 
    desenhaTubo(-0.2f, 0.75f, 0.0f, 0.2f, grossuraQuadro);   
    desenhaTubo(0.0f, 0.2f, -0.8f, 0.35f, grossuraQuadro);   
    desenhaTubo(0.0f, 0.2f, 0.5f, 0.7f, grossuraQuadro);     
    desenhaTubo(0.5f, 0.7f, -0.2f, 0.75f, grossuraQuadro);   
    desenhaTubo(0.5f, 0.7f, 0.8f, 0.35f, grossuraQuadro);

    // --- Guidão (Preto) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    desenhaTubo(0.5f, 0.7f, 0.45f, 0.9f, grossuraGuidao);

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
        
        glPushMatrix(); // Haste
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f); 
            glScalef(0.04f, 0.4f, 0.04f); 
            glutSolidCube(1.0f);
        glPopMatrix();
         
        glPushMatrix(); // Eixo
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glScalef(0.04f, 0.25f, 0.04f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Placas
        glPushMatrix(); glTranslatef(0.0f, 0.2f, 0.1f); glScalef(0.1f, 0.02f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, -0.2f, -0.1f); glScalef(0.1f, 0.02f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPopMatrix();
}

// --- Funções Principais de Desenho ---

void desenhaCeu()
{
    if (texturaCeuBackground == 0) {
        texturaCeuBackground = carregarBMP("ceu.bmp");
    }

    if (texturaCeuBackground != 0) {
        
        glDisable(GL_LIGHTING);   
        glDisable(GL_DEPTH_TEST); 
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, texturaCeuBackground);
        glColor3f(1.0f, 1.0f, 1.0f); // Branco puro

        glPushMatrix();
        
        // Giramos para a textura ficar de pé
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); 

        GLUquadric *quadCeu = gluNewQuadric();
        gluQuadricTexture(quadCeu, GL_TRUE);
        gluQuadricOrientation(quadCeu, GLU_INSIDE); // Textura por dentro
        
        // AUMENTAdo O RAIO PARA 1000.0f (Para cobrir tudo)
        gluSphere(quadCeu, 1000.0f, 32, 32); 
        
        gluDeleteQuadric(quadCeu);

        glPopMatrix();

        // --- RELIGA TUDO PARA O RESTO DO JOGO ---
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_LIGHTING);   
    }
}

void desenhaChao()
{
    if (texturaChao == 0) {
        texturaChao = carregarBMP("chao.bmp");
    }

    if (texturaChao != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaChao);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.2f, 0.2f, 0.2f);
    }

    float repeticao = 20.0f;
    float tamanho = 80.0f;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-tamanho, 0.0f, -tamanho);
        glTexCoord2f(repeticao, 0.0f); glVertex3f(tamanho, 0.0f, -tamanho);
        glTexCoord2f(repeticao, repeticao); glVertex3f(tamanho, 0.0f, tamanho);
        glTexCoord2f(0.0f, repeticao); glVertex3f(-tamanho, 0.0f, tamanho);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Função genérica para desenhar um cubo completo (Parede) com uma textura específica
static void desenhaBloco(GLuint texturaID)
{
    if (texturaID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaID);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.6f, 0.6f, 0.6f); // Cinza se falhar
    }

    float d = 0.5f;

    // Desenha o cubo (lados)
    glBegin(GL_QUADS);
        // Frente
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -d,  d);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( d, -d,  d);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( d,  d,  d);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-d,  d,  d);
        // Trás
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -d, -d);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-d,  d, -d);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( d,  d, -d);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( d, -d, -d);
        // Direita
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( d, -d, -d);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( d,  d, -d);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( d,  d,  d);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( d, -d,  d);
        // Esquerda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -d, -d);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -d,  d);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-d,  d,  d);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-d,  d, -d);
        // Topo e Base também podem ser desenhados aqui se a parede for flutuante,
        // mas para paredes de labirinto, geralmente só os lados importam.
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Função genérica para desenhar Chão ou Teto (um quadrado plano)
// isCeiling: se true, desenha em cima e aponta a normal para baixo
static void desenhaPlanoHorizontal(GLuint texturaID, bool isCeiling)
{
    if (texturaID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaID);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.4f, 0.4f, 0.4f);
    }

    float d = 0.5f;
    float yPos = isCeiling ? d : -d; // Se for teto é +0.5, se for chão é -0.5
    float normalY = isCeiling ? -1.0f : 1.0f; // Teto aponta pra baixo, chão pra cima

    glBegin(GL_QUADS);
    glNormal3f(0.0f, normalY, 0.0f);
    
    // Mapeamento UV simples
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, yPos, -d);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, yPos,  d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( d, yPos,  d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( d, yPos, -d);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void desenhaTorresELosangos()
{
    if (texturaTorre == 0) {
        texturaTorre = carregarBMP("torre.bmp");
    }

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

        // Desenha a Bike na torre específica
        if (i == 3) 
        {
            glPushMatrix();
            glTranslatef(0.10f, 0.0f, 0.9f); 
            glRotatef(23.0f, 0.0f, 4.0f, 0.0f); 
            glRotatef(-15.0f, 8.0f, 0.0f, 0.0f);
            desenhaBicicleta();
            glPopMatrix();
        }

        // --- TORRE COM TEXTURA ---
        glPushMatrix();
        
        // Configura Textura
        if (texturaTorre != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texturaTorre);
            glColor3f(1.0f, 1.0f, 1.0f); // Branco para não tingir a textura
        } else {
            glColor3f(1.0f, 1.0f, 1.0f); // Se falhar, desenha branco sólido
        }

        glTranslatef(0.0f, alturaTorre / 2.0f, 0.0f);
        glScalef(w, alturaTorre, w);
        
        // SUBSTITUI O GLUTSOLIDCUBE POR NOSSA FUNÇÃO
        desenhaBloco(texturaTorre); 

        glDisable(GL_TEXTURE_2D); // Desliga textura para não afetar o losango
        glPopMatrix();

        // --- Losango girando (com Shader Glow) ---
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

    // --- 1. Carrega Textura ---
    if (texturaPiramide == 0) {
        texturaPiramide = carregarBMP("piramide.bmp");
    }

    glPushMatrix(); 

    // --- 2. Habilita Textura ---
    if (texturaPiramide != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaPiramide);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        // Se falhar, usa cinza/preto como fallback
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5f, 0.5f, 0.5f);
    }

    // Degrau 1 (Base)
    glPushMatrix();
    glTranslatef(0.0f, alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase, alturaDegrau, tamanhoBase);
    desenhaBloco(texturaPiramide); 
    glPopMatrix();

    // Degrau 2 (Meio)
    glPushMatrix();
    glTranslatef(0.0f, alturaDegrau + alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase * reducao, alturaDegrau, tamanhoBase * reducao);
    desenhaBloco(texturaPiramide); 
    glPopMatrix();

    // Degrau 3 (Topo)
    glPushMatrix();
    glTranslatef(0.0f, 2 * alturaDegrau + alturaDegrau / 2.0f, 0.0f);
    glScalef(tamanhoBase * reducao * reducao, alturaDegrau, tamanhoBase * reducao * reducao);
    desenhaBloco(texturaPiramide); 
    glPopMatrix();

    // Desliga a textura para não afetar a bola e o escudo
    glDisable(GL_TEXTURE_2D);

    // --- ESFERA (BOLA DE FUTEBOL) ---
    float topoDegrausY = 5.0f * alturaDegrau;
    float raioEsfera = 3.0f;
    float yEsfera = topoDegrausY + raioEsfera + 0.2f; // variavel usada no escudo tbm

    if (texturaBola == 0) {
        texturaBola = carregarBMP("bola.bmp");
    }

    glPushMatrix();
    glTranslatef(0.0f, yEsfera, 0.0f);
    glRotatef(anguloEsfera, 1.0f, 1.5f, 0.0f); 

    if (texturaBola != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaBola);
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUquadric *quadBola = gluNewQuadric();
        gluQuadricTexture(quadBola, GL_TRUE);
        gluSphere(quadBola, raioEsfera, 40, 40);
        gluDeleteQuadric(quadBola);
        glDisable(GL_TEXTURE_2D);
    } 
    else {
        glColor3f(1.0f, 1.0f, 1.0f); glutSolidSphere(raioEsfera, 40, 40);
        glColor3f(0.0f, 0.0f, 0.0f); glutWireSphere(raioEsfera + 0.05f, 15, 15);
    }
    glPopMatrix();

    // --- ESCUDO DO CORINTHIANS ---
    if (texturaCorinthians == 0) {
        texturaCorinthians = carregarBMP("logo.bmp");
    }

    if (texturaCorinthians != 0) 
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaCorinthians);
        glColor3f(1.0f, 1.0f, 1.0f);

        glPushMatrix();
        glTranslatef(0.0f, yEsfera + raioEsfera + 2.5f, 0.0f);
        glRotatef(anguloEsfera, 0.0f, 1.0f, 0.0f); // Gira junto com a bola

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

    // --- CORREÇÃO FINAL AQUI ---
    glPopMatrix(); // <--- FALTAVA ESSE CARA FECHANDO A FUNÇÃO!
}

void desenhaCenario() // Ou o nome que você usa para desenhar o grid

{

    // --- 1. Carregamento Preguiçoso das Texturas ---

    if (texParede1 == 0) texParede1 = carregarBMP("parede.bmp");

    if (texParede2 == 0) texParede2 = carregarBMP("paredeAlt.bmp");

    if (texPiso1 == 0)   texPiso1   = carregarBMP("chao.bmp");

    if (texPiso2 == 0)   texPiso2   = carregarBMP("chaoAlt.bmp");

    if (texTeto == 0)    texTeto    = carregarBMP("teto.bmp");

    // --- CONFIGURAÇÕES DO MAPA ---
    float tamanhoBloco = 1.0f;
    float alturaParede = 4.0f; // <--- MUDE AQUI PARA AUMENTAR A PAREDE!
    
    // Move o mapa inteiro para longe da pirâmide
    glPushMatrix();
    glTranslatef(15.0f, 0.0f, 0.0f); 

    for (int z = 0; z < mapa.size(); z++) 
    {
        for (int x = 0; x < mapa[z].size(); x++) 
        {
            char tile = mapa[z][x];
            
            if (tile == ' ') continue; // Pula espaços vazios

            float posX = x * tamanhoBloco;
            float posZ = z * tamanhoBloco;

            glPushMatrix();

            // --- SE FOR PAREDE (# ou W) ---
            if (tile == '#' || tile == 'W') 
            {
                // 1. Posiciona no centro da altura (pra base ficar no chão 0.0)
                glTranslatef(posX, alturaParede / 2.0f, posZ);
                
                // 2. AQUI ESTÁ A MÁGICA: Estica o bloco no eixo Y
                glScalef(1.0f, alturaParede, 1.0f); 

                // 3. Desenha
                if (tile == '#') desenhaBloco(texParede1);
                else desenhaBloco(texParede2);
            }
            // --- SE FOR PISO OU TETO (., , - +) ---
            else 
            {
                // Mantém o chão na altura padrão (0.5 para ficar nível 0)
                glTranslatef(posX, 0.51f, posZ);

                switch (tile) 
                {
                    case '.': // Grama
                        desenhaPlanoHorizontal(texPiso1, false); 
                        break;

                    case ',': // Terra
                        desenhaPlanoHorizontal(texPiso2, false); 
                        break;

                    case '-': // Grama + Teto Alto
                        desenhaPlanoHorizontal(texPiso1, false); // Chão
                        
                        // Sobe para desenhar o teto lá no topo da parede
                        glPushMatrix();
                        glTranslatef(0.0f, alturaParede - 1.0f, 0.0f);
                        desenhaPlanoHorizontal(texTeto, true);   // Teto
                        glPopMatrix();
                        break;

                    case '+': // Terra + Teto Alto
                        desenhaPlanoHorizontal(texPiso2, false); // Chão
                        
                        // Sobe para desenhar o teto lá no topo
                        glPushMatrix();
                        glTranslatef(0.0f, alturaParede - 1.0f, 0.0f);
                        desenhaPlanoHorizontal(texTeto, true);   // Teto
                        glPopMatrix();
                        break;
                }
            }

            glPopMatrix();
        }
    }

    glPopMatrix(); // Fecha o bloco do mapa inteiro
}