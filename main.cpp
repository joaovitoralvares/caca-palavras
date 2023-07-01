#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro5.h>           //biblioteca padrao do allegro
#include <allegro5/allegro_primitives.h> //biblioteca para desenhar formas na tela
#include <allegro5/allegro_font.h>       //biblioteca pra escrever na tela
#define DISP_W 1200                      // largura da tela
#define DISP_H 700                       // altura da tela

using namespace std;

/**
 Comandos para compilar no VS Code

 g++ -I C:\allegro\include -c main.cpp
 g++.exe -I C:\allegro\include main.o -o Jogo.exe liballegro_monolith.dll.a
 .\Jogo.exe

 cls



*/

struct Quadrado_letra
{
    float posX1;
    float posY1;
    float posX2;
    float posY2;
};

int main()
{
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

    al_init(); // inicializa o allegro

    al_init_font_addon();       // inicializa o font addon do allegro (necessario pra escrever na tela)
    al_init_primitives_addon(); // inicilaiza o primitives addon do allegro (necessario pra desenhar na tela)

    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

    al_install_mouse(); // installa o mouse pro allegro

    ALLEGRO_DISPLAY *display = al_create_display(DISP_W, DISP_H); // cria a tela
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();         // cria a fila de eventos
    ALLEGRO_FONT *font = al_create_builtin_font();                // cria o font
    ALLEGRO_FONT *letras[20][20];
    ALLEGRO_MOUSE_STATE estado_mouse;

    al_get_mouse_state(&estado_mouse);

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            letras[i][j] = al_create_builtin_font();
        }
    }
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // cria o timer

    al_register_event_source(queue, al_get_display_event_source(display)); // registra os eventos do display na fila
    al_register_event_source(queue, al_get_timer_event_source(timer));     // registra os eventos do timer na fila
    al_register_event_source(queue, al_get_mouse_event_source());          // registra os eventos do mouse na fila

    //  =================================================================================
    //                             Criação Espaço das letras
    //  =================================================================================

    // struct Circle circle = {DISP_W/2, DISP_H/2, 20};//inicializa um circle (essa variavel representa o circulo desenhado na tela)
    Quadrado_letra quadrado[20][20];
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
    string palavras[] = {"arroz", "feijao", "batata", "macarrao", "peixe", "passas", "couve", "strgonof", "salada", "fricasse"};

    char matriz[20][20];

    int load = 0, tam_total = 0;
    for (int i = 0; i < sizeof(palavras) / sizeof(string); i++)
    {
        tam_total += palavras[i].length();
    }
    int posicoes_palavras_verticais[5][3];
    int posicoes_palavras_horizontais[5][3];
    int iteracao_palavras_escolhidas = tam_total - 1;

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
        cout << "Carregando " << load << "% ... \n";
        // al_draw_textf(font, al_map_rgb(0, 0, 0), DISP_W * 0.45, DISP_H * 0.5, 0 , "Carregando %d %...", load);

        string escolhida = palavras[i];
        bool teste_coluna = true;
        int tam_max = 20 - palavras[i].length(), pos_palavra = rand() % tam_max, iteracao_escolhida = 0, pos_coluna;
        do
        {
            srand((time(NULL)));
            teste_coluna = true;
            pos_coluna = rand() % 20;
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
        posicoes_palavras_verticais[i][0] = pos_palavra;
        posicoes_palavras_verticais[i][1] = pos_coluna;
        posicoes_palavras_verticais[i][2] = pos_palavra + escolhida.length() - 1;
    }

    for (int i = 5; i < 10; i++)
    {
        load += 10;
        system("cls");
        cout << "Carregando " << load << "% ... \n";
        // al_draw_textf(font, al_map_rgb(0, 0, 0), DISP_W * 0.45, DISP_H * 0.5, 0 , "Carregando %d %...", load);
        string escolhida = palavras[i];
        bool teste_linha = true;
        int tam_max = 20 - palavras[i].length(), pos_palavra = rand() % tam_max, iteracao_escolhida = 0, pos_linha;
        do
        {
            srand((time(NULL)));
            teste_linha = true;
            pos_linha = rand() % 20;
            for (int j = pos_palavra; j < pos_palavra + escolhida.length() && teste_linha == true; j++)
            {
                if (matriz[pos_linha][j] != '0')
                {
                    teste_linha = false;
                }
            }
        } while (!teste_linha && i > 0);

        for (int j = pos_palavra; j < pos_palavra + escolhida.length(); j++, iteracao_escolhida++)
        {
            matriz[pos_linha][j] = escolhida[iteracao_escolhida];
        }
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
                // matriz[i][j] = rand() % 26 + 97;
            }
            else
            {
                // posicoes_palavras_verticais[iteracao_palavras_escolhidas][0] = i;
                // posicoes_palavras_verticais[iteracao_palavras_escolhidas][1] = j;
                // iteracao_palavras_escolhidas--;
            }
            // printf("%c ", matriz[i][j]);
        }
    }

    //  =================================================================================
    //                                 Fila de Eventos
    //  =================================================================================
    int acertos = 0;
    bool click_button_tes = false;
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
        }
        //  =================================================================================
        //                                 Eventos na tela
        //  =================================================================================
        if (logic)
        {

            // DRAW

            //  =================================================================================
            //                                    JOGO
            //  =================================================================================

            // al_draw_filled_circle(circle.posX, circle.posY, circle.raio, al_map_rgb(0,0,0));//desenha o circulo na tela
            // al_draw_textf(font, al_map_rgb(0,0,0), DISP_W/100, 30, 0, "Voce clicou na posicao x: %d y: %d", mouseClickPositionX, mouseClickPositionY);//escreve na tela a posicao do mouse quando clicar na tela
            al_clear_to_color(al_map_rgb(0, 0, 0)); // limpa a tela a recolorindo toda de branco
            al_draw_textf(font, branco, DISP_W * 0.35, DISP_H * 0.01, 0, "CACA PALAVRAS - ACERTOS: %d", acertos);

            if (click_button_tes)
            {
                al_draw_line(mouseClickPositionX_init, mouseClickPositionY_init, mouseClickPositionX_end, mouseClickPositionY_end, branco, 5);
            }
            // for (int i = 0; i < 20; i++)
            // {
            //     for (int j = 0; j < 20; j++)
            //     {
            //     }
            // }



            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    float posX_letra = (quadrado[i][j].posX2 - quadrado[i][j].posX1) / 2, posY_letra = (quadrado[i][j].posY2 - quadrado[i][j].posY1) / 2;
                    al_draw_rectangle(quadrado[i][j].posX1, quadrado[i][j].posY1, quadrado[i][j].posX2, quadrado[i][j].posY2, al_map_rgb(255, 255, 255), 1);
                    al_draw_textf(letras[i][j], al_map_rgb(255, 0, 0), quadrado[i][j].posX1 + DISP_W * 0.01, quadrado[i][j].posY1 + DISP_H * 0.01, 0, "%c", matriz[i][j]);
                }
            }

            al_flip_display(); // atualiza a tela
            // posicoes_palavras_verticais[i][0] = pos_palavra;
            // posicoes_palavras_verticais[i][1] = pos_coluna;
            // posicoes_palavras_verticais[i][2] = pos_palavra + escolhida.length();

            for (int i = 0; i < 5; i++)
            {
                if (mouseClickPositionX_init > quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posX1 && mouseClickPositionX_init < quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posX2 && mouseClickPositionY_init > quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posY1 && mouseClickPositionY_init < quadrado[posicoes_palavras_verticais[i][0]][posicoes_palavras_verticais[i][1]].posY2)
                {
                    if (mouseClickPositionX_end > quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posX1 && mouseClickPositionX_end < quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posX2 && mouseClickPositionY_end > quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posY1 && mouseClickPositionY_end < quadrado[posicoes_palavras_verticais[i][2]][posicoes_palavras_verticais[i][1]].posY2)
                    {
                        acertos++;
                        mouseClickPositionX_init = NULL;
                        mouseClickPositionY_init = NULL;
                        mouseClickPositionX_end = NULL;
                        mouseClickPositionY_end = NULL;
                    }
                }
            }

            // posicoes_palavras_verticais[i][0] = pos_palavra;
            // posicoes_palavras_verticais[i][1] = pos_linha;
            // posicoes_palavras_verticais[i][2] = pos_palavra + escolhida.length();

            for (int i = 0; i < 5; i++)
            {
                if (mouseClickPositionX_init > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posX1 && mouseClickPositionX_init < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posX2 && mouseClickPositionY_init > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posY1 && mouseClickPositionY_init < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][0]].posY2)
                {
                    if (mouseClickPositionX_end > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posX1 && mouseClickPositionX_end < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posX2 && mouseClickPositionY_end > quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posY1 && mouseClickPositionY_end < quadrado[posicoes_palavras_horizontais[i][1]][posicoes_palavras_horizontais[i][2]].posY2)
                    {
                        acertos++;
                        mouseClickPositionX_init = NULL;
                        mouseClickPositionY_init = NULL;
                        mouseClickPositionX_end = NULL;
                        mouseClickPositionY_end = NULL;
                    }
                }
            }


            if(acertos >=10){
                done =true;
            }

            logic = false;
        }
    }

    //  =================================================================================
    //                         Encerramento do que foi criado
    //  =================================================================================

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
