# Gerador de Labirintos Aleatórios com DFS em C++

Este projeto é uma implementação de um gerador de labirintos aleatórios utilizando a busca em profundidade (DFS) em C++. O labirinto é visualizado graficamente usando a biblioteca GLUT (OpenGL Utility Toolkit).

## Descrição

O objetivo deste projeto é criar um labirinto aleatório a partir de uma matriz 2D, onde a geração do labirinto é feita através da técnica de busca em profundidade. A biblioteca GLUT é utilizada para renderizar o labirinto graficamente, permitindo uma visualização interativa.

## Funcionalidades

- Geração de labirintos aleatórios utilizando a técnica DFS.
- Visualização gráfica do labirinto com GLUT.
- Ajuste de parâmetros como tamanho do labirinto e velocidade de geração.

## Requisitos

- **Compilador C++**: Certifique-se de ter um compilador C++ instalado. Por exemplo, GCC ou Clang.
- **Biblioteca GLUT**: A biblioteca GLUT deve estar instalada no seu sistema. Você pode encontrar instruções de instalação [aqui](http://www.opengl.org/resources/libraries/glut/).
- **CMake** (opcional): Para facilitar a configuração e compilação do projeto.

## Instalação

1. **Clone o repositório**:

   ```bash
   git clone hhttps://github.com/DanielAlvesFonsecaNeto/labirintoAleatorio
   ```

2. **Instale a biblioteca GLUT**:

   - No **Ubuntu/Debian**:

     ```bash
     sudo apt-get install freeglut3-dev
     ```

   - No **macOS** (usando Homebrew):

     ```bash
     brew install freeglut
     ```

   - No **Windows**: Baixe e instale o pacote do GLUT e configure as variáveis de ambiente conforme necessário.

## Uso

Execute o programa compilado para gerar e visualizar o labirinto:

```bash
./labirinto
```

```
