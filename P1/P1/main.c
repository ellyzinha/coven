#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
 
#define LARGURA_TELA 640
#define ALTURA_TELA 480
#define FPS 60.0
 
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_FONT *fonte = NULL;

struct sprite {
    ALLEGRO_BITMAP *sprite;
    int largura_sprite, altura_sprite;
    int regiao_x, regiao_y;
    int frames_sprite, cont_frames;
    int colunas_sprite, coluna_atual;
    int linhas_sprite, linha_atual;
    float pos_x, pos_y;
    float velocidade;
    int sprite_inicial, sprite_final;
};

struct sprite bruxa;
 
void error_msg(char *text){
	al_show_native_message_box(janela,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}
 
int inicializar(){
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon()){
        error_msg("Falha ao inicializar add-on allegro_ttf");
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    al_init_image_addon();
 
    if (!al_init_image_addon()){
        error_msg("Falha ao inicializar add-on allegro_image");
        return 0;
    }

 
    //inicializa addon do teclado
    if (!al_install_keyboard()){
        error_msg("Falha ao inicializar o teclado");
        return 0;
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        error_msg("Falha ao criar janela");
        return 0;
    }
 
    al_set_window_title(janela, "Coven");

    al_init_primitives_addon();
 
    fonte = al_load_font("C:/Users/elly/Desktop/jogo-p1/assets/arial.ttf", 72, 0);
    if (!fonte){
        error_msg("Falha ao carregar fonte");
        al_destroy_display(janela);
        return 0;
    }
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        error_msg("Falha ao criar fila de eventos");
        al_destroy_display(janela);
        return 0;
    }
 
    //registra duas fontes de eventos na fila. o da janela, e do teclado
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_start_timer(timer);

    
    return 1;
}

void init_bruxa() {
    bruxa.sprite = al_load_bitmap("C:/Users/elly/Desktop/jogo-p1/assets/Sprites/stand.png");
    bruxa.largura_sprite = 35;
    bruxa.altura_sprite = 35;
    bruxa.colunas_sprite = 3;
    bruxa.linhas_sprite = 4;
    bruxa.coluna_atual = 0;
    bruxa.linha_atual = 0;
    bruxa.frames_sprite = 6;
    bruxa.cont_frames = 0;
    bruxa.pos_x = 200;
    bruxa.pos_y = 300;
    bruxa.velocidade = 4;
    bruxa.sprite_inicial = 0;
    bruxa.sprite_final = 0;
}

void atualiza_bruxa() {
    int sprite_atual;
    bruxa.cont_frames++;
    if (bruxa.cont_frames >= bruxa.frames_sprite) {
        bruxa.cont_frames = 0;

        bruxa.coluna_atual++;
        sprite_atual = bruxa.coluna_atual + bruxa.colunas_sprite * bruxa.linha_atual;

        if (sprite_atual > bruxa.sprite_final || sprite_atual < bruxa.sprite_inicial) {
            bruxa.linha_atual = bruxa.sprite_inicial / bruxa.colunas_sprite;
            bruxa.coluna_atual = bruxa.sprite_inicial % bruxa.colunas_sprite;
        }
        else if (bruxa.coluna_atual >= bruxa.colunas_sprite) {
            bruxa.coluna_atual = 0;
            bruxa.linha_atual++;
            if (bruxa.linha_atual >= bruxa.linhas_sprite) {
                bruxa.linha_atual = 0;
            }
        }
        bruxa.regiao_x = bruxa.largura_sprite * bruxa.coluna_atual;
        bruxa.regiao_y = bruxa.altura_sprite * bruxa.linha_atual;
    }
}

 
int main(void)
{
    int sair = 0;
    int tecla = 0;
 
    if (!inicializar()){
        return -1;
    }
 
    al_clear_to_color(al_map_rgb(255, 155, 255));
 
    while (!sair){
        while(!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            //espera ate que algum evento esteja na fila
            al_wait_for_event(fila_eventos, &evento);
 
            init_bruxa();
            al_draw_bitmap(bruxa.sprite, bruxa.pos_x, bruxa.pos_y, 0);
            //se o evento for pressionar uma tecla
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                //verifica qual tecla foi pressionada
                switch(evento.keyboard.keycode){
                //seta para cima
                case ALLEGRO_KEY_UP:
                    tecla = 1;
                    break;
                //seta para baixo
                case ALLEGRO_KEY_DOWN:
                    tecla = 2;
                    break;
                //seta para esquerda
                case ALLEGRO_KEY_LEFT:
                    tecla = 3;
                    break;
                //seta para direita.
                case ALLEGRO_KEY_RIGHT:
                    tecla = 4;
                    break;
                //esc. sair=1 faz com que o programa saia do loop principal
                case ALLEGRO_KEY_ESCAPE:
                    sair = 1;
                }
            }
            //se o evento foi o soltar de uma tecla
            //tecla=5 significa que alguma foi pressionada mas nao e nenhuma das setas
            else if (evento.type == ALLEGRO_EVENT_KEY_UP){
                tecla = 5;
            }
            //se clicou para fechar a janela
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = 1;
            }
        }
 
        //tecla!=0 significa que algo foi pressionado
        if (tecla){
            al_clear_to_color(al_map_rgb(255, 255, 255));
 
            //desenha na tela as palavras relativas a seta preesionada
            switch (tecla){
            case 1:
                //cima
                break;
            case 2:
                //baixo
                break;
            case 3:
                //esquerda
                break;
            case 4:
                //direita
                break;
            }
 
            //zera a tecla para a proxima vez nao entrar aqui de novo
            tecla = 0;
        }
 
        al_flip_display();
    }
 
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 0;
}