#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <stack>
#include <utility>   // para pair
#include <algorithm> // para shuffle
#include <random>    // para random_device e mt19937

/* ---------------------------------CORES----------------------------------- */

// cor do fundo
//SDL_Color cor_fundo = {120, 120, 180, 255}; // meio azul
SDL_Color cor_fundo = {20, 20, 20, 255};
// cor da parede
SDL_Color cor_parede = {20, 20, 20, 255}; // quase preto
// cor do caminho
SDL_Color cor_caminho = {130, 130, 160, 255}; // quase branco cinza azulado

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^CORES^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ---------------------------------VARIAVEIS E ESTRTURAS GLOBAIS----------------------------------- */

// tamanho da janela
const int Janela_L = 1280;
const int Janela_A = 720;

// quantas celulas ^2 tem a matriz, nxn  // padrao = 23
const int dim_matriz = 23;

// quantos porcentos a parede em relação ao tamanho da celula
const float espessura_parede = 0.05;

// gerando sementes aleatorias para o suffle
std::random_device randow_device;
std::mt19937 gerador(randow_device());

// celula de uma matriz , tem informações como direção de caminho etc..
struct labirinto_celula
{
    // verificação se ja foi explorado, apenas usada para algumas animação futura talvez mas nao afeta a DFS
    bool explorado = false;
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
// std::vector<std::vector<labirinto_celula>> matriz_labirinto(dim_matriz, std::vector<labirinto_celula>(dim_matriz));

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^VARIAVEIS E ESTRTURAS GLOBAIS^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ---------------------------------FUNÇÕES PARA DESENHAR COISAS NA TELA----------------------------------- */

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

    // Calcula o tamanho do quadrado menor
    int tamanhoMenor = tamanho * (1 - (espessura_parede * 2));

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

// Não me pergunte como isso ta funcionando !!!!! , odioooo , fiquei 3 horas nessa porcaria >:(   )
//  essa função desenha denovo o quadrado
void desenhar_deslocamento(SDL_Renderer *renderizador, int offsetX, int offsetY, int x, int y, int tamanho, int direcao)
{

    /*
        direcao = 0 ==> norte
        direcao = 1 ==> sul
        direcao = 2 ==> leste
        direcao = 3 ==> oeste
    */

    // Calcula o tamanho do quadrado menor
    int tamanhoMenor = tamanho * (1 - (espessura_parede * 2));

    // Calcula a posição para centralizar o quadrado menor no quadrado maior
    int xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2;
    int yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2;

    // Desenhar o quadrado menor
    SDL_Rect quadradoMenor = {xMenor, yMenor, tamanhoMenor, tamanhoMenor};
    SDL_SetRenderDrawColor(renderizador, cor_caminho.r, cor_caminho.g, cor_caminho.b, cor_caminho.a); // azul
    SDL_RenderFillRect(renderizador, &quadradoMenor);


    if (direcao == 0)
    {
        // deslocamento pro norte
        xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2;
        yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2 + tamanhoMenor / -2 - tamanho * espessura_parede;
    }
    if (direcao == 1)
    {
        // deslocamento pro sul
        xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2;
        yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2 + tamanhoMenor / 2 + tamanho * espessura_parede;
    }
    if (direcao == 2)
    {
        // deslocamento pro leste
        xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2 + tamanhoMenor / 2 + tamanho * espessura_parede;
        yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2;
    }
    if (direcao == 3)
    {
        // deslocamento pro oeste
        xMenor = offsetX + x + (tamanho - tamanhoMenor) / 2 + tamanhoMenor / -2 - tamanho * espessura_parede;
        yMenor = offsetY + y + (tamanho - tamanhoMenor) / 2;
    }

    // Desenhar o quadrado menor
    SDL_Rect quadradoDeslocado = {xMenor, yMenor, tamanhoMenor, tamanhoMenor};
    SDL_SetRenderDrawColor(renderizador, cor_caminho.r, cor_caminho.g, cor_caminho.b, cor_caminho.a); // azul
    SDL_RenderFillRect(renderizador, &quadradoDeslocado);
}

void desenharQuebraParede(SDL_Renderer *renderizador, int rect_size, int linha, int coluna, int offsetX, int offsetY, int direcao)
{

    int x = offsetX + coluna * rect_size;
    int y = offsetY + linha * rect_size;

    desenhar_deslocamento(renderizador, x, y, 0, 0, rect_size, direcao);
    SDL_RenderPresent(renderizador);
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^FUNÇÕES PARA DESENHAR COISAS NA TELA^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ---------------------------------CRIAR LABIRINTO----------------------------------- */

/* função que constroi o labirinto e tambem desenha */
void construir_Labirinto(std::vector<std::vector<labirinto_celula>> &matriz_labirinto, SDL_Renderer *renderizador, int rect_size, int offsetX, int offsetY)
{

    srand(time(nullptr));

    bool continuar_loop = true;

    std::stack<std::pair<int, int>> pilha;
    pilha.push({0, 0}); // começa a geração do labirinto a partir da celula 0,0 , a primeira
    matriz_labirinto[0][0].explorado = true;
    matriz_labirinto[0][0].norte = 1;                // seria o caminho pra saida do labirinto
    matriz_labirinto[0][0].pai_cordenadas = {-1, 0}; // ele mesmo

    while (!pilha.empty() && continuar_loop)
    {
        // desempilha da pilha, vamos trabalhar com o atual
        std::pair<int, int> atual = pilha.top();
        pilha.pop();

        // verifica se ja foi visitado
        if (matriz_labirinto[atual.first][atual.second].visitado == false)
        {

            //std::cout << atual.first << ' ' << atual.second << '\n';

            // marca esse atual como visitado para ele não poder ser visitado de novo
            matriz_labirinto[atual.first][atual.second].visitado = true;

            // marca cordenadas do atual como cordenadas do prorio atual
            matriz_labirinto[atual.first][atual.second].cordenadas = atual;

            // referencia do pai
            std::pair<int, int> pai_do_atual = matriz_labirinto[atual.first][atual.second].pai_cordenadas;
            // coloca o pai na lista de ligacoes
            if (atual.first != 0 || atual.second != 0)
            { // caso não seja o {0,0}
                matriz_labirinto[atual.first][atual.second].ligacoes.push_back(pai_do_atual);
                // coloca o atual na lista de ligacoes do pai
                matriz_labirinto[pai_do_atual.first][pai_do_atual.second].ligacoes.push_back(atual);
            }

            // desenhar AQUI caminho do pai com o atual !!! if(atual = 0,0 , nao desenha)

            // definindo direções possiveis para celulas visinhas
            // norte
            std::pair<int, int> norte_do_atual = {atual.first - 1, atual.second};
            // sul
            std::pair<int, int> sul_do_atual = {atual.first + 1, atual.second};
            // leste
            std::pair<int, int> leste_do_atual = {atual.first, atual.second + 1};
            // oeste
            std::pair<int, int> oeste_do_atual = {atual.first, atual.second - 1};

            // vetor de celulas visinhas que vão poder entrar na pilha
            std::vector<std::pair<int, int>> vetor_explorados;

            // validando movimentos com IF
            /*
                verifico se os movimentos são possiveis,
                se forem, verifico se a celula do movimenta ainda não foi visitada,

                marco a celula como explorada, lembrando que explorada não significa visitada

                se não foi visitada, e marco o pai dela como atual para uma possivel próxima interação
                esse pai pode mudar conforme o loop


                verifico tbm se o pai do atual é igual umas das direções , mesmo que ele não entre na pilha
                essa é a hora que caso o pai do atual seja de uma das direções, ele vai marcar um caminho para
                norte ou sul ou leste ou oeste do atual
                tbm faz isso com o pai só que pra direção oposta
                ex : se atual tem caminho para o pai no norte, o pai do atual tem caaminho para o atual pelo sul

            */
            if (norte_do_atual.first >= 0 && norte_do_atual.second >= 0 && norte_do_atual.first < dim_matriz && norte_do_atual.second < dim_matriz)
            {
                if (matriz_labirinto[norte_do_atual.first][norte_do_atual.second].visitado == false)
                {
                    matriz_labirinto[norte_do_atual.first][norte_do_atual.second].explorado = true;
                    matriz_labirinto[norte_do_atual.first][norte_do_atual.second].pai_cordenadas = atual;
                    vetor_explorados.push_back(norte_do_atual);
                }
                if (matriz_labirinto[atual.first][atual.second].pai_cordenadas == norte_do_atual)
                {
                    matriz_labirinto[atual.first][atual.second].norte = 1;
                    matriz_labirinto[pai_do_atual.first][pai_do_atual.second].sul = 1;
                    desenharQuebraParede(renderizador,rect_size,atual.first,atual.second,offsetX,offsetY,0);
                }
            }
            if (sul_do_atual.first >= 0 && sul_do_atual.second >= 0 && sul_do_atual.first < dim_matriz && sul_do_atual.second < dim_matriz)
            {
                if (matriz_labirinto[sul_do_atual.first][sul_do_atual.second].visitado == false)
                {
                    matriz_labirinto[sul_do_atual.first][sul_do_atual.second].explorado = true;
                    matriz_labirinto[sul_do_atual.first][sul_do_atual.second].pai_cordenadas = atual;
                    vetor_explorados.push_back(sul_do_atual);
                }
                if (matriz_labirinto[atual.first][atual.second].pai_cordenadas == sul_do_atual)
                {
                    matriz_labirinto[atual.first][atual.second].sul = 1;
                    matriz_labirinto[pai_do_atual.first][pai_do_atual.second].norte = 1;
                    desenharQuebraParede(renderizador,rect_size,atual.first,atual.second,offsetX,offsetY,1);
                }
            }
            if (leste_do_atual.first >= 0 && leste_do_atual.second >= 0 && leste_do_atual.first < dim_matriz && leste_do_atual.second < dim_matriz)
            {
                if (matriz_labirinto[leste_do_atual.first][leste_do_atual.second].visitado == false)
                {
                    matriz_labirinto[leste_do_atual.first][leste_do_atual.second].explorado = true;
                    matriz_labirinto[leste_do_atual.first][leste_do_atual.second].pai_cordenadas = atual;
                    vetor_explorados.push_back(leste_do_atual);
                }
                if (matriz_labirinto[atual.first][atual.second].pai_cordenadas == leste_do_atual)
                {
                    matriz_labirinto[atual.first][atual.second].leste = 1;
                    matriz_labirinto[pai_do_atual.first][pai_do_atual.second].oeste = 1;
                    desenharQuebraParede(renderizador,rect_size,atual.first,atual.second,offsetX,offsetY,2);
                }
            }
            if (oeste_do_atual.first >= 0 && oeste_do_atual.second >= 0 && oeste_do_atual.first < dim_matriz && oeste_do_atual.second < dim_matriz)
            {
                if (matriz_labirinto[oeste_do_atual.first][oeste_do_atual.second].visitado == false)
                {
                    matriz_labirinto[oeste_do_atual.first][oeste_do_atual.second].explorado = true;
                    matriz_labirinto[oeste_do_atual.first][oeste_do_atual.second].pai_cordenadas = atual;
                    vetor_explorados.push_back(oeste_do_atual);
                }
                if (matriz_labirinto[atual.first][atual.second].pai_cordenadas == oeste_do_atual)
                {
                    matriz_labirinto[atual.first][atual.second].oeste = 1;
                    matriz_labirinto[pai_do_atual.first][pai_do_atual.second].leste = 1;
                    desenharQuebraParede(renderizador,rect_size,atual.first,atual.second,offsetX,offsetY,3);
                }
            }

            // Embaralha o vetor de explorados
            std::shuffle(vetor_explorados.begin(), vetor_explorados.end(), gerador);

            // Transfere os elementos do vetor para a pilha
            for (auto it = vetor_explorados.rbegin(); it != vetor_explorados.rend(); ++it)
            {
                pilha.push(*it); // Insere os elementos na pilha
            }
        }
    }
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^CRIAR LABIRINTO^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

int main(int argc, char *argv[])
{
    std::vector<std::vector<labirinto_celula>> matriz_labirinto(dim_matriz, std::vector<labirinto_celula>(dim_matriz));

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

    desenharGrids(renderizador_1, rect_size, offsetX, offsetY);

    construir_Labirinto(matriz_labirinto, renderizador_1, rect_size, offsetX, offsetY);

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

        // desenharGrids(renderizador_1, rect_size, offsetX, offsetY);

        // desenharQuebraParede(renderizador_1,rect_size,3,2,offsetX,offsetY,rect_size);
    }

    // destroi janela e libera recursos alocados para ela
    SDL_DestroyWindow(janela);

    // encerra a SDL e libera todos os recursos que ela usou
    SDL_Quit();

    return 0;
}
