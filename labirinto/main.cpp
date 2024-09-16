#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <utility>
#include <stack>

/* ---------------------------------CORES----------------------------------- */

// cor do fundo
SDL_Color cor_fundo = {120, 120, 180, 255}; // meio azul
// cor da parede
SDL_Color cor_parede = {20, 20, 20, 255}; // quase preto
// cor do caminho
SDL_Color cor_caminho = {130, 130, 160, 255}; // quase branco cinza azulado

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^CORES^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ---------------------------------VARIAVEIS E ESTRTURAS GLOBAIS----------------------------------- */

// tamanho da janela
const int Janela_L = 720;
const int Janela_A = 720;

// quantas celulas ^2 tem a matriz, nxn
const int dim_matriz = 23;

// celula de uma matriz , tem informações como direção de caminho etc..
struct labirinto_celula
{
    // verificação se ja foi visitado para poder entrar na pilha ou não
    bool visitado = false;
    // cordenadas caso precise
    std::pair<int, int> cordenadas;
    // quem era o pai, util para achar menor caminho
    std::pair<int, int> pai_cordenadas;
    // tem caminho ? 0 = não, 1 = sim
    int norte = 0;
    int sul = 0;
    int leste = 0;
    int oeste = 0;
    // caso precise para manipulações melhores do grafo
    std::vector<std::pair<int, int>> ligacoes;
};

// matriz do labirinto, essa é a estrura que vai ser processada e armazenar o labirinto
std::vector<std::vector<labirinto_celula>> matriz_labirinto;

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^VARIAVEIS E ESTRTURAS GLOBAIS^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/*
Função que desenha malha de quadrados na tela, seria o labirinto sem caminho, apenas paredes
e desenhar cada quadrado da grid
*/
void desenharQuadradoComOutroDentro(SDL_Renderer *renderizador, int offsetX, int offsetY, int x, int y, int tamanho)
{
    // Desenhar o quadrado maior, levando em consideração o offset
    SDL_Rect quadradoMaior = {offsetX, offsetY, tamanho, tamanho};
    SDL_SetRenderDrawColor(renderizador, cor_parede.r, cor_parede.g, cor_parede.b, cor_parede.a); // verde
    SDL_RenderFillRect(renderizador, &quadradoMaior);

    // Calcula o tamanho do quadrado menor (80% do tamanho do quadrado maior)
    int tamanhoMenor = tamanho * 0.8;

    // Calcula a posição para centralizar o quadrado menor no quadrado maior
    int xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2;
    int yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2;

    // Desenhar o quadrado menor
    SDL_Rect quadradoMenor = {xMenor, yMenor, tamanhoMenor, tamanhoMenor};
    SDL_SetRenderDrawColor(renderizador, cor_caminho.r, cor_caminho.g, cor_caminho.b, cor_caminho.a); // azul
    SDL_RenderFillRect(renderizador, &quadradoMenor);
}

void desenharGrids(SDL_Renderer *renderizador, int rect_size, int offsetX, int offsetY)
{
    // Itera sobre as células da matriz
    for (int i = 0; i < dim_matriz; i++)
    {
        for (int j = 0; j < dim_matriz; j++)
        {
            // Calcula a posição do quadrado na tela
            int x = offsetX + j * rect_size;
            int y = offsetY + i * rect_size;

            desenharQuadradoComOutroDentro(renderizador, x, y, 0, 0, rect_size);
        }
    }

    SDL_RenderPresent(renderizador);
}

/* função que constroi o labirinto e tambem desenha */
void construir_Labirinto()
{

    srand(time(nullptr));

    bool continuar_loop = true;

    std::stack<std::pair<int, int>> pilha;
    pilha.push({0, 0}); // começa a geração do labirinto a partir da celula 0,0 , a primeira
    matriz_labirinto[0][0].visitado = true;
    matriz_labirinto[0][0].cordenadas = {0, 0};
    matriz_labirinto[0][0].norte = 1;               // seria o caminho pra saida do labirinto
    matriz_labirinto[0][0].pai_cordenadas = {0, 0}; // ele mesmo

    while (!pilha.empty() && continuar_loop)
    {

        // desempilha da pilha, vamos trabalhar com o atual
        std::pair<int, int> atual = pilha.top();
        pilha.pop();

        // verifica se ja foi visitado
        if (matriz_labirinto[atual.first][atual.second].visitado == false)
        {

            // definindo direções possiveis
            // norte
            std::pair<int, int> norte_do_atual = {atual.first - 1, atual.second};
            // sul
            std::pair<int, int> sul_do_atual = {atual.first + 1, atual.second};
            // leste
            std::pair<int, int> leste_do_atual = {atual.first, atual.second + 1};
            // oeste
            std::pair<int, int> oeste_do_atual = {atual.first, atual.second - 1};

            /*
                para facilitar as operações com rng e listas vou definir que
                :
                    0 = norte
                    1 = sul
                    2 = leste
                    3 = oeste
            */

            // validando movimentos com IF
        }
    }
}

int main(int argc, char *argv[])
{

    // Criando a janela
    SDL_Window *janela = SDL_CreateWindow(
        "Labirinto",             // nome da janela
        SDL_WINDOWPOS_UNDEFINED, // posi X janela
        SDL_WINDOWPOS_UNDEFINED, // posi Y janela
        Janela_L, Janela_A,      // largura e altura da janela
        SDL_WINDOW_ALLOW_HIGHDPI // flag pra permitir alta DPI
    );

    // verificando se a janela foi criada corretamente
    if (janela == NULL)
    {
        std::cout << "nao foi possível criar a janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // criando um renderizador para desenhar na janela
    SDL_Renderer *renderizador_1 = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (renderizador_1 == NULL)
    {
        std::cout << "nao foi possível criar o renderizador: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return 1;
    }
    // definindo a cor de desenho do renderizador
    SDL_SetRenderDrawColor(renderizador_1, cor_fundo.r, cor_fundo.g, cor_fundo.b, cor_fundo.a);
    // limpa o renderizador aplicando a cor definida em cima
    SDL_RenderClear(renderizador_1);
    // mostra o renderizador na tela
    SDL_RenderPresent(renderizador_1);

    /*
        Aqui vou definir a area central, tudo vai ser deshando nela!!
    */

    // Calcula o tamanho do quadrado e o offset para centralizar
    int rect_size = std::min(Janela_L / dim_matriz, Janela_A / dim_matriz); // Menor valor entre largura/n e altura/n
    int offsetX = (Janela_L - (rect_size * dim_matriz)) / 2;                // Offset X para centralizar
    int offsetY = (Janela_A - (rect_size * dim_matriz)) / 2;                // Offset Y para centralizar

    // estrutura que armazena todos os eventos capturados (mouse, teclado, fechar janela, etc..)
    SDL_Event evento;

    // controle do loop
    bool continuar_loop = true;

    // continua o loop inifitamente até que um evento faça parar
    while (continuar_loop)
    {

        // verifica se teve evento
        if (SDL_PollEvent(&evento))
        {

            // se o evento foi de fechar a janela
            if (SDL_QUIT == evento.type)
            {
                continuar_loop = false;
                continue;
            }
        }

        desenharGrids(renderizador_1, rect_size, offsetX, offsetY);
    }

    // destroi janela e libera recursos alocados para ela
    SDL_DestroyWindow(janela);

    // encerra a SDL e libera todos os recursos que ela usou
    SDL_Quit();

    return 0;
}
