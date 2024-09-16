Aqui está o README corrigido e atualizado de acordo com suas especificações:

---

# Gerador de Labirintos Aleatórios com DFS em C++

Este projeto é uma implementação de um gerador de labirintos aleatórios utilizando a busca em profundidade (DFS) em C++. O labirinto é visualizado graficamente utilizando a biblioteca SDL2.

## Descrição

O objetivo deste projeto é criar um labirinto aleatório a partir de uma matriz 2D, onde a geração do labirinto é feita através da técnica de busca em profundidade (DFS). A biblioteca SDL2 é utilizada para renderizar o labirinto graficamente, permitindo uma visualização interativa e responsiva.

## Funcionalidades

- Geração de labirintos aleatórios utilizando a técnica DFS.
- Visualização gráfica do labirinto com SDL2.
- Ajuste de parâmetros como tamanho do labirinto e velocidade de geração.
- Teclas de funcionalidade:
  - **R**: Gera um novo labirinto.
  - **Barra de espaço**: Remove o delay da geração atual.
  - **S**: Mostra o menor caminho de uma ponta a outra do labirinto.

## Requisitos

- **Compilador C++ para Windows 64-bit**: Certifique-se de ter um compilador compatível, como o MinGW. Instruções para instalação abaixo.
- **Biblioteca SDL2**: A biblioteca SDL2 utilizada esta inclusa nesse projeto
- **CMake** (opcional): Para facilitar a configuração e compilação do projeto.

## Instalação

1. **Clone o repositório**:

   ```bash
   git clone https://github.com/DanielAlvesFonsecaNeto/labirintoAleatorio
   ```

2. **Instale o MinGW (w64devkit)**:

   Baixe a versão adequada do compilador MinGW para Windows 64-bit [aqui](https://github.com/skeeto/w64devkit/releases). Neste projeto foi usada a versão 2.0.0:

   - Baixe o arquivo `w64devkit-x64-2.0.0.exe` e siga as instruções de instalação.

3. **Instale a biblioteca SDL2**:

   Baixe a versão adequada do SDL2 para MinGW [aqui](https://github.com/libsdl-org/SDL/releases). Neste projeto foi usada a versão 2.30.7:

   - Baixe o arquivo `SDL2-devel-2.30.7-mingw.zip`. caso utilize windows64
   - Extraia o conteúdo do arquivo e configure as variáveis de ambiente ou inclua os diretórios de cabeçalhos e bibliotecas no seu ambiente de desenvolvimento.
   - nesse projeto ela ja ta inclusa 

## Compilação


```bash
g++ -I src/include -L src/lib -o labirinto main.cpp -lmingw32 -lSDL2main -lSDL2
```



## Uso

Após a compilação, execute o programa para gerar e visualizar o labirinto:

```bash
./labirinto
```

Você poderá interagir com o labirinto utilizando as teclas descritas anteriormente para gerar novos labirintos, remover o delay ou visualizar o menor caminho.
