#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro5.h>           //biblioteca padrao do allegro
#include <allegro5/allegro_primitives.h> //biblioteca para desenhar formas na tela
#include <allegro5/allegro_font.h>       //biblioteca pra escrever na tela
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/keyboard.h>
#define DISP_W 1200 // largura da tela
#define DISP_H 700  // altura da tela

using namespace std;

/**
 Comandos para compilar no VS Code

 g++ -I C:\allegro\include -c main.cpp
 g++.exe -I C:\allegro\include main.o -o Jogo.exe liballegro_monolith.dll.a
 .\Jogo.exe

 cls



*/

struct Retangulo
{
    float posX1;
    float posY1;
    float posX2;
    float posY2;
};

struct Triangulo
{
    float posX1;
    float posY1;
    float posX2;
    float posY2;
    float posX3;
    float posY3;
};

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Portuguese");
    // system("cls");
    //   =================================================================================
    //                                    Eventos Allegro
    //   =================================================================================

    ALLEGRO_EVENT event;              // representa o evento atual
    bool done = false;                // true quando a aplicacao for encerrada
    bool logic = false;               // true quando evento timer for acionado
    int mouseClickPositionX_init = 0; // posicao x inicial do mouse quando clicar
    int mouseClickPositionY_init = 0; // posicao y inicial do mouse quando clicar
    int mouseClickPositionX_end = 0;  // posicao x final do mouse quando clicar
    int mouseClickPositionY_end = 0;  // posicao y final do mouse quando clicar
    int tecla_pressionada = 0;

    al_init(); // inicializa o allegro

    al_init_font_addon();       // inicializa o font addon do allegro (necessario pra escrever na tela)
    al_init_primitives_addon(); // inicilaiza o primitives addon do allegro (necessario pra desenhar na tela)
    al_init_ttf_addon();

    ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

    al_install_mouse();    // installa o mouse pro allegro
    al_install_keyboard(); // instala o teclado para o allegro

    ALLEGRO_DISPLAY *display = al_create_display(DISP_W, DISP_H);      // cria a tela
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();              // cria a fila de eventos
    ALLEGRO_FONT *font = al_load_ttf_font("./Roboto-Bold.ttf", 14, 0); // cria o font
    ALLEGRO_FONT *letras[20][20];

    ALLEGRO_MOUSE_STATE estado_mouse;

    al_get_mouse_state(&estado_mouse);

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            letras[i][j] = al_load_ttf_font("./Roboto-Bold.ttf", 12, 0);
        }
    }
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // cria o timer

    al_register_event_source(queue, al_get_display_event_source(display)); // registra os eventos do display na fila
    al_register_event_source(queue, al_get_timer_event_source(timer));     // registra os eventos do timer na fila
    al_register_event_source(queue, al_get_mouse_event_source());          // registra os eventos do mouse na fila
    al_register_event_source(queue, al_get_keyboard_event_source());       // registra os eventos do teclado na fila

    //  =================================================================================
    //                             Criação Espaço das letras
    //  =================================================================================

    // struct Circle circle = {DISP_W/2, DISP_H/2, 20};//inicializa um circle (essa variavel representa o circulo desenhado na tela)
    Retangulo quadrado[20][20];
    if (quadrado)
    {
        float x = DISP_W * .3, y = DISP_H * .08;
        for (int i = 0; i < 20; i++, y += DISP_H * 0.036)
        {
            x = DISP_W * .3;
            for (int j = 0; j < 20; j++, x += DISP_W * 0.02)
            {
                quadrado[i][j] = {x, y, x + DISP_H * 37 / 1000, y + DISP_W * 1 / 50};
            }
        }
    }
    //  =================================================================================
    //                             Organização das palavras
    //  =================================================================================
    string palavras[][10] = {
        {"ARROZ", "FEIJAO", "BATATA", "MACARRAO", "PEIXE", "PASSAS", "COUVE", "STRGONOF", "SALADA", "FRICASSE"},
        {"ADICAO", "SUBTRACAO", "MULTIPLICACAO", "DIVISAO", "POTENCIACAO", "RADICIACAO", "IGUALDADE", "MAIOR", "MENOR", "DIFERENTE"},
        {"VERMELHO", "AZUL", "AMARELO", "VERDE", "CIANO", "MAGENTA", "PRETO", "BRANCO", "CINZA", "MARROM"},
        {"BRASIL", "ARGENTINA", "CHILE", "COLOMBIA", "PERU", "EQUADOR", "VENEZUELA", "BOLIVIA", "PARAGUAI", "URUGUAI"},
        {"VIOLAO", "GUITARRA", "VIOLINO", "VIOLONCELO", "CONTRABAIXO", "HARPA", "BANDOLIM", "CAVAQUINHO", "UKULELE", "BANJO"},
        {"LEAO", "TIGRE", "ELEFANTE", "GIRAFA", "RINOCERONTE", "HIPOPOTAMO", "CROCODILO", "GORILA", "LOBO", "URSO"},
        {"MEDICO", "PROFESSOR", "ENGENHEIRO", "ADVOGADO", "ARQUITETO", "CONTADOR", "DENTISTA", "PSICOLOGO", "JORNALISTA", "ESTILISTA"},
        {"ROCK", "POP", "JAZZ", "SAMBA", "RAP", "REGGAE", "ELETRONICA", "CLASSICA", "FUNK", "COUNTRY"},
        {"FUTEBOL", "BASQUETE", "TENIS", "NATACAO", "VOLEI", "ATLETISMO", "GOLFE", "HANDEBOL", "BOXE", "JUDO"},
        {"HIDROGENIO", "OXIGENIO", "CARBONO", "NITROGENIO", "SODIO", "FERRO", "CALCIO", "POTASSIO", "OURO", "PRATA"}};

    char matriz[20][20];

    int load = 0, tam_total = 0;
    char tema[40];

    for (int i = 0; i < sizeof(palavras) / sizeof(string) * 2; i++)
    {
        //  tam_total += palavras[banco_sorteado][i].length();
    }
    int posicoes_palavras_verticais[5][3];
    int posicoes_palavras_horizontais[5][3];
    int iteracao_palavras_escolhidas = tam_total - 1;

    //  =================================================================================
    //                                 Fila de Eventos
    //  =================================================================================

    int acertos = 0, palavras_acertadas[10][3];
    Retangulo linhas_palavras_acertadas[10];
    Retangulo recomecar = {DISP_W * 0.87, DISP_H * 0.01, DISP_W * 0.98, DISP_H * 0.04};
    Triangulo retroceder;
    retroceder = {
        DISP_W * 0.03,
        DISP_H * 0.02,
        DISP_W * 0.03,
        DISP_H * 0.04,
        DISP_W * 0.02,
        DISP_H * 0.03,
    };
    Retangulo botoes_menu[3];
    botoes_menu[0] = {DISP_W * 0.3, DISP_H * 0.3, DISP_W * 0.7, DISP_H * 0.35};
    botoes_menu[1] = {DISP_W * 0.3, DISP_H * 0.4, DISP_W * 0.7, DISP_H * 0.45};
    botoes_menu[2] = {DISP_W * 0.3, DISP_H * 0.5, DISP_W * 0.7, DISP_H * 0.55};
    int tela_atual = 0;
    /**
     * TELA ATUAL = Indica a tela que aparece
     * 0 - Tela inicial (menu)
     * 1 - Tela do caça palavras
     * 2 - Como jogar
     */

    int opcao_selecionada = 1;

    bool click_button_tes = false, pronto_para_jogar = false;
    al_start_timer(timer); // inicializa o timer
    // enquanto a aplicacao nao fechar faca
    while (!done)
    {

        al_wait_for_event(queue, &event); // espera o proximo evento

        // verifica o proximo evento
        switch (event.type)
        {
        // Aciona quando a tela e fechada
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;

        // Aciona qunado bate o timer
        case ALLEGRO_EVENT_TIMER:
            logic = true;
            break;

        // Aciona quando precionado algum botao do mouse
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (event.mouse.button == 1) // Quando o botao precionado for o esquerdo
            {
                mouseClickPositionX_init = event.mouse.x;
                mouseClickPositionY_init = event.mouse.y;
                // mouseClickPositionX = estado_mouse.x;
                // mouseClickPositionY = estado_mouse.y;
                click_button_tes = false;
            }
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            if (event.mouse.button == 1) // Quando o botao precionado for o esquerdo
            {
                mouseClickPositionX_end = event.mouse.x;
                mouseClickPositionY_end = event.mouse.y;
                // mouseClickPositionX = estado_mouse.x;
                // mouseClickPositionY = estado_mouse.y;
                click_button_tes = true;
            }

            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            tecla_pressionada = event.keyboard.keycode;
            break;
        }
        //  =================================================================================
        //                                 Eventos na tela
        //  =================================================================================

        if (logic)
        {
            srand((time(NULL)));
            // DRAW
            al_clear_to_color(branco);
            //  ========================================================================================================================================================================
            //                                                                                   JOGO
            //  ========================================================================================================================================================================

            if (tecla_pressionada == ALLEGRO_KEY_ESCAPE)
            {
                tela_atual = 0;
            }

            switch (tela_atual)
            {

            case 0:
                //  =================================================================================
                //                                      MENU
                //  =================================================================================
                {
                    char opcoes_menu[3][20] = {{"Jogar"}, {"Como jogar"}, {"Sair"}};
                    al_draw_text(font, preto, DISP_W * 0.45, DISP_H * 0.2, 0, "CAÇA PALAVRAS");
                    for (int i = 0; i < 3; i++)
                    {
                        if (i == opcao_selecionada - 1)
                        {
                            cout << "SELECIONADO" << opcao_selecionada << endl;
                            al_draw_filled_rectangle(botoes_menu[i].posX1, botoes_menu[i].posY1, botoes_menu[i].posX2, botoes_menu[i].posY2, vermelho);
                        }
                        else
                        {
                            al_draw_filled_rectangle(botoes_menu[i].posX1, botoes_menu[i].posY1, botoes_menu[i].posX2, botoes_menu[i].posY2, preto);
                        }
                    }
                    al_draw_textf(font, branco, botoes_menu[0].posX1 * 1.6, botoes_menu[0].posY1 * 1.06, 0, "%s", opcoes_menu[0]);
                    al_draw_textf(font, branco, botoes_menu[1].posX1 * 1.55, botoes_menu[1].posY1 * 1.05, 0, "%s", opcoes_menu[1]);
                    al_draw_textf(font, branco, botoes_menu[2].posX1 * 1.61, botoes_menu[2].posY1 * 1.04, 0, "%s", opcoes_menu[2]);

                    if (mouseClickPositionX_init > botoes_menu[0].posX1 && mouseClickPositionX_init < botoes_menu[0].posX2 && mouseClickPositionY_init > botoes_menu[0].posY1 && mouseClickPositionY_init < botoes_menu[0].posY2)
                    {
                        tela_atual = 1;
                    }
                    else if (mouseClickPositionX_init > botoes_menu[1].posX1 && mouseClickPositionX_init < botoes_menu[1].posX2 && mouseClickPositionY_init > botoes_menu[1].posY1 && mouseClickPositionY_init < botoes_menu[1].posY2)
                    {
                        tela_atual = 2;
                    }
                    else if (mouseClickPositionX_init > botoes_menu[2].posX1 && mouseClickPositionX_init < botoes_menu[2].posX2 && mouseClickPositionY_init > botoes_menu[2].posY1 && mouseClickPositionY_init < botoes_menu[2].posY2)
                    {
                        done = true;
                    }

                    if (tecla_pressionada == ALLEGRO_KEY_UP)
                    {
                        opcao_selecionada--; // navegar para a opção acima
                        if (opcao_selecionada < 1)
                        {
                            opcao_selecionada = 3;
                        }
                        cout << "CIMA " << opcao_selecionada << endl;
                    }
                    else if (tecla_pressionada == ALLEGRO_KEY_DOWN)
                    {

                        opcao_selecionada++; // navegar para a opção abaixo
                        if (opcao_selecionada > 3)
                        {
                            opcao_selecionada = 1;
                        }
                        cout << "BAIXO " << opcao_selecionada << endl;
                    }
                    else if (tecla_pressionada == ALLEGRO_KEY_ENTER && tela_atual == 0)
                    {
                        cout << "ENTER" << opcao_selecionada << endl;
                        // Executar a ação correspondente à opção selecionada
                        if (opcao_selecionada == 1)
                        {
                            tela_atual = 1;
                        }
                        else if (opcao_selecionada == 2)
                        {
                            tela_atual = 2;
                        }
                        else if (opcao_selecionada == 3)
                        {
                            done = true;
                        }
                    }
                }
                break;
            case 1:

                //  =================================================================================
                //                                 Tela do Jogo
                //  =================================================================================

                {
                    al_clear_to_color(branco); // limpa a tela a recolorindo toda de branco

                    if (!pronto_para_jogar)
                    {

                        int banco_sorteado = rand() % 10;
                        switch (banco_sorteado)
                        {
                        case 0:
                        {
                            char temp[] = "Comidas";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 1:
                        {
                            char temp[] = "Operações Matematicas";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 2:
                        {
                            char temp[] = "Cores";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 3:
                        {
                            char temp[] = "Países da América do Sul";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 4:
                        {
                            char temp[] = "Intrumentos de Corda";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 5:
                        {
                            char temp[] = "Animais";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 6:
                        {
                            char temp[] = "Profissões";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 7:
                        {
                            char temp[] = "Ritmos Musicais";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 8:
                        {
                            char temp[] = "Esportes";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        case 9:
                        {
                            char temp[] = "Elementos Químicos";
                            for (int i = 0; i < sizeof(temp); i++)
                            {
                                tema[i] = temp[i];
                            }
                        }
                        break;
                        }

                        //  =================================================================================================================================================================================

                        for (int i = 0; i < 20; i++)
                        {
                            for (int j = 0; j < 20; j++)
                            {
                                matriz[i][j] = '0';
                            }
                        }

                        //  =================================================================================================================================================================================

                        for (int i = 0; i < 5; i++)
                        {
                            load += 10;
                            system("cls");
                            al_flip_display();
                            al_clear_to_color(branco);
                            cout << "Carregando " << load << "% ... \n";
                            al_draw_textf(font, preto, DISP_W * 0.45, DISP_H * 0.5, 0, "Carregando %d % ...", load);
                            string escolhida = palavras[banco_sorteado][i];
                            bool teste_coluna = true;
                            int tam_max = 20 - palavras[banco_sorteado][i].length(), pos_palavra = rand() % tam_max, iteracao_escolhida = 0, pos_coluna;
                            do
                            {
                                teste_coluna = true;
                                pos_coluna = rand() % 19;
                                for (int j = pos_palavra; j < pos_palavra + escolhida.length() && teste_coluna == true; j++)
                                {
                                    if (matriz[j][pos_coluna] != '0')
                                    {
                                        teste_coluna = false;
                                    }
                                }
                            } while (!teste_coluna && i > 0);

                            for (int j = pos_palavra; j < pos_palavra + escolhida.length(); j++, iteracao_escolhida++)
                            {
                                matriz[j][pos_coluna] = escolhida[iteracao_escolhida];
                            }

                            cout << "vertical " << i << ". Posicao: " << pos_palavra << "," << pos_coluna << " Tamanho: " << escolhida.length() << " " << escolhida << endl;
                            posicoes_palavras_verticais[i][0] = pos_palavra;
                            posicoes_palavras_verticais[i][1] = pos_coluna;
                            posicoes_palavras_verticais[i][2] = pos_palavra + escolhida.length() - 1;
                        }

                        for (int i = 5; i < 10; i++)
                        {
                            load += 10;
                            al_flip_display();
                            al_clear_to_color(branco);
                            system("cls");
                            cout << "Carregando " << load << "% ... \n";
                            al_draw_textf(font, preto, DISP_W * 0.45, DISP_H * 0.5, 0, "Carregando %d  % ...", load);
                            string escolhida = palavras[banco_sorteado][i];
                            bool teste_linha = true;
                            int tam_max = 20 - palavras[banco_sorteado][i].length(), pos_palavra = rand() % tam_max, iteracao_escolhida = 0, pos_linha, cont_randomizacao = 0;
                            do
                            {
                                teste_linha = true;
                                pos_linha = rand() % 19;
                                for (int j = pos_palavra; j < pos_palavra + escolhida.length() && teste_linha == true; j++)
                                {
                                    if (matriz[pos_linha][j] != '0')
                                    {
                                        teste_linha = false;
                                        cont_randomizacao++;
                                    }
                                    if (cont_randomizacao >= 10)
                                    {
                                        srand(time(NULL));
                                        cont_randomizacao = 0;
                                        pos_palavra++;
                                    }
                                }
                            } while (!teste_linha && i > 0);

                            for (int j = pos_palavra; j < pos_palavra + escolhida.length(); j++, iteracao_escolhida++)
                            {
                                matriz[pos_linha][j] = escolhida[iteracao_escolhida];
                            }
                            cout << "horizontal " << i - 5 << ". Posicao: " << pos_palavra << "," << pos_linha << " Tamanho: " << escolhida.length() << " " << escolhida << endl;
                            posicoes_palavras_horizontais[i - 5][0] = pos_palavra;
                            posicoes_palavras_horizontais[i - 5][1] = pos_linha;
                            posicoes_palavras_horizontais[i - 5][2] = pos_palavra + escolhida.length() - 1;
                        }

                        //  =================================================================================================================================================================================

                        for (int i = 0; i < 20; i++)
                        {
                            for (int j = 0; j < 20; j++)
                            {
                                if (matriz[i][j] == '0')
                                {
                                    matriz[i][j] = rand() % 26 + 65;
                                }
                            }
                        }
                        pronto_para_jogar = true;
                    }
                    al_clear_to_color(branco); // limpa a tela a recolorindo toda de branco
                    bool palavra_repetida = false;

                    al_draw_textf(font, preto, DISP_W * 0.35, DISP_H * 0.01, 0, "CAÇA PALAVRAS - TEMA: %s - ACERTOS: %d", tema, acertos);
                    if (click_button_tes)
                    {
                        al_draw_pixel(mouseClickPositionX_init, mouseClickPositionY_init, preto);
                        if (mouseClickPositionX_init != NULL && mouseClickPositionX_end != NULL)
                        {
                            al_draw_line(mouseClickPositionX_init, mouseClickPositionY_init, mouseClickPositionX_end, mouseClickPositionY_end, preto, 5);
                        }
                    }

                    for (int i = 0; i < 20; i++)
                    {
                        for (int j = 0; j < 20; j++)
                        {
                            float posX_letra = (quadrado[i][j].posX2 - quadrado[i][j].posX1) / 2, posY_letra = (quadrado[i][j].posY2 - quadrado[i][j].posY1) / 2;
                            al_draw_rectangle(quadrado[i][j].posX1, quadrado[i][j].posY1, quadrado[i][j].posX2, quadrado[i][j].posY2, al_map_rgb(0, 0, 0), 1);
                            al_draw_textf(letras[i][j], preto, quadrado[i][j].posX1 + DISP_W * 0.01, quadrado[i][j].posY1 + DISP_H * 0.01, 0, "%c", matriz[i][j]);
                        }
                    }

                    for (int index = 0; index < acertos; index++)
                    {
                        al_draw_pixel(linhas_palavras_acertadas[index].posX1, linhas_palavras_acertadas[index].posY1, preto);
                        al_draw_line(linhas_palavras_acertadas[index].posX1, linhas_palavras_acertadas[index].posY1, linhas_palavras_acertadas[index].posX2, linhas_palavras_acertadas[index].posY2, preto, 5);
                    }

                    for (int i = 0; i < 5; i++)
                    {

                        if (mouseClickPositionX_init > quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posX1 && mouseClickPositionX_init < quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posX2 && mouseClickPositionY_init > quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posY1 && mouseClickPositionY_init < quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posY2)
                        {
                            if (mouseClickPositionX_end > quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posX1 && mouseClickPositionX_end < quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posX2 && mouseClickPositionY_end > quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posY1 && mouseClickPositionY_end < quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posY2)
                            {
                                for (int j = 0; j < acertos; j++)
                                {
                                    if (
                                        palavras_acertadas[j][0] == posicoes_palavras_verticais[i][0] && palavras_acertadas[j][1] == posicoes_palavras_verticais[i][1] && palavras_acertadas[j][2] == posicoes_palavras_verticais[i][2])
                                    {
                                        cout << "palavra repetida" << endl;
                                        palavra_repetida = true;
                                    }
                                }

                                if (!palavra_repetida)
                                {

                                    linhas_palavras_acertadas[acertos].posX1 = mouseClickPositionX_init;
                                    linhas_palavras_acertadas[acertos].posY1 = mouseClickPositionY_init;
                                    linhas_palavras_acertadas[acertos].posX2 = mouseClickPositionX_end;
                                    linhas_palavras_acertadas[acertos].posY2 = mouseClickPositionY_end;

                                    palavras_acertadas[acertos][0] = posicoes_palavras_verticais[i][0];
                                    palavras_acertadas[acertos][1] = posicoes_palavras_verticais[i][1];
                                    palavras_acertadas[acertos][2] = posicoes_palavras_verticais[i][2];

                                    acertos++;
                                    mouseClickPositionX_init = NULL;
                                    mouseClickPositionY_init = NULL;
                                    mouseClickPositionX_end = NULL;
                                    mouseClickPositionY_end = NULL;
                                }
                            }
                        }
                    }
                    for (int i = 0; i < 5; i++)
                    {

                        if (mouseClickPositionX_init > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posX1 && mouseClickPositionX_init < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posX2 && mouseClickPositionY_init > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posY1 && mouseClickPositionY_init < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posY2)
                        {
                            if (mouseClickPositionX_end > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posX1 && mouseClickPositionX_end < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posX2 && mouseClickPositionY_end > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posY1 && mouseClickPositionY_end < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posY2)
                            {
                                for (int j = 0; j < acertos; j++)
                                {
                                    if (
                                        palavras_acertadas[j][0] == posicoes_palavras_horizontais[i][0] && palavras_acertadas[j][1] == posicoes_palavras_horizontais[i][1] && palavras_acertadas[j][2] == posicoes_palavras_horizontais[i][2])
                                    {
                                        cout << "palavras repetidas" << endl;
                                        palavra_repetida = true;
                                    }
                                }

                                if (!palavra_repetida)
                                {

                                    linhas_palavras_acertadas[acertos].posX1 = mouseClickPositionX_init;
                                    linhas_palavras_acertadas[acertos].posY1 = mouseClickPositionY_init;
                                    linhas_palavras_acertadas[acertos].posX2 = mouseClickPositionX_end;
                                    linhas_palavras_acertadas[acertos].posY2 = mouseClickPositionY_end;

                                    palavras_acertadas[acertos][0] = posicoes_palavras_horizontais[i][0];
                                    palavras_acertadas[acertos][1] = posicoes_palavras_horizontais[i][1];
                                    palavras_acertadas[acertos][2] = posicoes_palavras_horizontais[i][2];

                                    acertos++;
                                    mouseClickPositionX_init = NULL;
                                    mouseClickPositionY_init = NULL;
                                    mouseClickPositionX_end = NULL;
                                    mouseClickPositionY_end = NULL;
                                }
                            }
                        }
                    }
                    al_draw_filled_triangle(retroceder.posX1, retroceder.posY1, retroceder.posX2, retroceder.posY2, retroceder.posX3, retroceder.posY3, preto);
                    if (mouseClickPositionX_init > retroceder.posX3 && mouseClickPositionX_init < retroceder.posX1 && mouseClickPositionY_init > retroceder.posY1 && mouseClickPositionY_init < retroceder.posY2)
                    {
                        tela_atual = 0;
                    }
                    al_draw_filled_rounded_rectangle(recomecar.posX1, recomecar.posY1, recomecar.posX2, recomecar.posY2, 1, 1, preto);
                    al_draw_textf(font, branco, recomecar.posX1 + recomecar.posX1 * 0.03, recomecar.posY1 + recomecar.posY1 * 0.7, 0, "Recomeçar");
                    if (mouseClickPositionX_init > recomecar.posX1 && mouseClickPositionX_init < recomecar.posX2 && mouseClickPositionY_init > recomecar.posY1 && mouseClickPositionY_init < recomecar.posY2)
                    {

                        pronto_para_jogar = false;
                        load = 0;
                        mouseClickPositionX_end = NULL;
                        mouseClickPositionY_end = NULL;
                        mouseClickPositionX_init = NULL;
                        mouseClickPositionY_init = NULL;

                        acertos = 0;

                        for (int i = 0; i < 10; i++)
                        {
                            linhas_palavras_acertadas[i] = Retangulo();
                        }
                    }
                    if (acertos >= 10)
                    {
                        done = true;
                    }
                }

                break;
            case 2:
                //  =================================================================================
                //                                  Instruções
                //  =================================================================================
                {
                    al_draw_text(font, preto, DISP_W * 0.4, DISP_H * 0.1, -1, "COMO JOGAR");
                    al_draw_multiline_text(font, preto, DISP_W * 0.2, DISP_H * 0.2, DISP_W * 0.6, al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT, "O caça-palavras é um jogo em que o objetivo é encontrar palavras escondidas em uma matriz de letras. As palavras podem estar dispostas na horizontal e vertical, apenas da esquerda para a direita. Elas não podem se sobrepor e compartilhar letras com outras palavras.\n\nPara jogar, você precisa observar a matriz de letras e procurar por palavras que vão em acordo com o tema na parte superior da tela. Use o mouse para selecionar as palavras que encontrar. É importante avisar que as palavras só serão validadas se selecionadas na ordem correta, ou seja, da esquerda para a direta ou de cima para baixo.\n\nExistem 10 opções de temas, com 10 palavras diferentes em cada, sendo eles:\n\n- COMIDAS\n- OPERAÇÕES MATEMÁTICAS\n- CORES\n- PAÍSES DA AMÉRICA DO SUL\n- INSTRUMENTOS DE CORDA\n- ANIMAIS\n- PROFISSÕES\n- RITMOS MUSICAIS\n- ESPORTES\n- ELEMENTOS QUÍMICOS.\n\nAs posições das palavras nunca se repetem, então não faltará opções para jogar.\n\nDIVIRTA-SE!");
                    al_draw_filled_triangle(retroceder.posX1, retroceder.posY1, retroceder.posX2, retroceder.posY2, retroceder.posX3, retroceder.posY3, preto);

                    if (mouseClickPositionX_init > retroceder.posX3 && mouseClickPositionX_init < retroceder.posX1 && mouseClickPositionY_init > retroceder.posY1 && mouseClickPositionY_init < retroceder.posY2)
                    {
                        tela_atual = 0;
                        mouseClickPositionX_init = NULL;
                        mouseClickPositionY_init = NULL;
                        mouseClickPositionX_end = NULL;
                        mouseClickPositionY_end = NULL;
                    }
                }
                break;
            default:
                done = true;
                break;
            }
            al_flip_display();
            logic = false;
            tecla_pressionada = 0;
        }
    }

    //  ===========================================================================================================================================================================
    //                                                                  Encerramento do que foi criado
    //  ===========================================================================================================================================================================

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(font);
    al_destroy_timer(timer);
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            al_destroy_font(letras[i][j]);
        }
    }

    return 0;
}