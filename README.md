# Odeio o Xulio!!!
#  Ambiente C/C++ + OpenGL (FreeGLUT + GLEW) no VSCode usando MSYS2

Este guia explica como configurar um ambiente completo para desenvolvimento em C/C++ com OpenGL, utilizando **MSYS2**, **FreeGLUT**, **GLEW** e **Visual Studio Code** no Windows.

---

## 📦 1. Instalação do MSYS2

### 🔹 Baixar e instalar
1. Acesse [https://www.msys2.org](https://www.msys2.org).
2. Baixe a versão para **Windows 64 bits** e conclua a instalação padrão.

### 🔹 Atualizar o sistema
1. Abra o terminal **MSYS2 MSYS** (ícone roxo).
2. Execute o comando abaixo e aceite as atualizações:

```bash
pacman -Syu
```

---

## 🔧 2. Instalar GCC, GDB, FreeGLUT e GLEW

Feche o terminal anterior e abra o terminal **MSYS2 MinGW 64-bit** (ícone azul). Execute os comandos abaixo para instalar o compilador e as bibliotecas gráficas:

```bash
# Compilador C/C++
pacman -S mingw-w64-x86_64-gcc

# Debugger
pacman -S mingw-w64-x86_64-gdb

# FreeGLUT (Janelas e Input)
pacman -S mingw-w64-x86_64-freeglut

# GLEW (Extensões modernas do OpenGL)
pacman -S mingw-w64-x86_64-glew
```

---

## 🛠️ 3. Configurar variáveis de ambiente

Para que o Windows reconheça os comandos `gcc` e `g++`, adicione o binário ao Path:

1. Abra a busca do Windows e digite: **"Editar as variáveis de ambiente do sistema"**.
2. Clique em **Variáveis de Ambiente**.
3. Na seção **Variáveis de usuário** (parte de cima), selecione a linha `Path` e clique em **Editar**.
4. Clique em **Novo** e adicione o caminho:
   ```
   C:\msys64\mingw64\bin
   ```
5. Clique em OK em todas as janelas.

> ⚠️ **Importante:** Reinicie o computador (ou faça logoff) para aplicar as mudanças.

### Testar instalação
Abra o Prompt de Comando (CMD) ou PowerShell e digite:
```bash
gcc --version
gdb --version
```

---

## 🎨 4. Configuração do VSCode

### 🔹 Instalar extensão C/C++
1. Abra o VSCode.
2. Vá em **Extensões** (`Ctrl+Shift+X`).
3. Pesquise por `C/C++` (Microsoft) e instale.

---

## 📁 5. Criar o projeto

1. Crie uma pasta para seu projeto no Windows.
2. Abra essa pasta no VSCode (`File > Open Folder`).
3. Crie um arquivo chamado `main.cpp` e insira seu código OpenGL.

---

## ⚙️ 6. Configurar build no VSCode (tasks.json)

Este passo é crucial para linkar as bibliotecas corretamente.

1. No VSCode, pressione `Ctrl+Shift+P` e digite `Tasks: Configure Default Build Task`.
2. Selecione **g++ (MSYS2 / MinGW64)** se aparecer, ou "Create tasks.json file from template".
3. Substitua todo o conteúdo do arquivo `.vscode/tasks.json` pelo código abaixo:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe",
                "-lfreeglut",
                "-lopengl32",
                "-lglu32",
                "-lglew32"
            ],
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

> **Nota:** As flags `-lfreeglut -lopengl32 -lglu32 -lglew32` são responsáveis por incluir as bibliotecas no executável final.

---

## ▶️ 7. Compilar e Executar

1. Abra o arquivo `main.cpp`.
2. Pressione `Ctrl+Shift+B` para compilar (Build).
3. Se não houver erros, o executável (`.exe`) será gerado na mesma pasta.
4. Execute o programa pelo terminal integrado ou clicando duas vezes no `.exe`.

---

## 🎉 Resumo da Configuração

Seu ambiente está pronto com:

- ✔️ **MSYS2** (Gerenciador de pacotes)
- ✔️ **GCC / G++** (Compilador)
- ✔️ **GDB** (Debugger)
- ✔️ **FreeGLUT** (Gerenciamento de janelas)
- ✔️ **GLEW** (Funções OpenGL modernas)
- ✔️ **VSCode** (IDE configurada)