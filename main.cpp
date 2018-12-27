/*

* Programa: Jogo da Mem�ria 2.0
* Data de Cria��o: 28/10/2017
* Criado por: Luiz Gustavo Nunes

*/

#include <stdio.h> // Biblioteca para o printf e scanf.
#include <stdlib.h> // Biblioteca para o rand().
#include <allegro.h> // Biblioteca para o Allegro.
#include <time.h> // Biblioteca para o srand().
#include <winalleg.h> // Biblioteca para usar o SetConsoleTitle.

// Configura��o de Resolu��o e Cores:
#define MAX_X		800 // Resolu��o Horizontal.
#define MAX_Y		600 // Resolu��o Vertical.
#define V_MAX_X     0
#define V_MAX_Y     0
#define COLOR_BITS	8

// Prot�tipos do Allegro:
int inicia(void);
void principal(void);
void finaliza(void);

// Main:
int main (void)
{
    if (!inicia())
	{
		finaliza();
		return -1;
	}
	
	principal();
	finaliza();
	
	SetConsoleTitle("Hearthstone's Memory Game");
	
    return 0;
}
END_OF_MAIN();

// Inicializa��o do Allegro:
int inicia (void)
{
	allegro_init(); // Fun��o de Inicializa��o do Allegro.
	install_keyboard(); // Fun��o para instalar o teclado.
	install_mouse(); // Fun��o para instalar o mouse.
	install_timer(); // Fun��o para instalar o timer.
	set_color_depth(8); // Fun��o de instala��o do modo gr�fico.
	
	// Teste de funcionalidade do modo gr�fico:
	if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		printf("Erro ao inicializar o modo grafico.\n");
		return (FALSE);
	}
	
	// Teste de funcionalidade do som:
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
		printf("Erro ao inicializar o som.\n");
		return (FALSE);
	}
	set_volume(150, 200); // Configura��o de Volume.
	return (TRUE);
}

// Fun��o principal do jogo:
void principal (void)
{    
	int	x=165, y=165; // Posi��o da carta inicial/refer�ncia.
    int	i, ai; // �ndice.
	int cima[20]; // Vetor de cartas viradas pra cima.
	int vturn[2]; // Vetor de cartas escolhidas na rodada.
	int num[20], auxnum; // Identificador de carta.
    int turn=0; // Escolhas de carta.
    int score1=0, score2=0; // Pontua��o bruta.
	float sfinal1, sfinal2; // Pontua��o l�quida.
    int jogada1=0, jogada2=0; // Jogadas.
    int round=0; // Total de rodadas.   
    int esc=0; // Bot�o ESC.
	int	botEsq; // Bot�o esquerdo do mouse.
	int	botDir; // Bot�o direito do mouse.

	// Ponteiros de bitmap:
    BITMAP	*tela; // Tabuleiro.
	BITMAP  *rcard[20]; // Cartas Gabarito.
	BITMAP  *card[20]; // Cartas Jogo.
	BITMAP  *aux; // Auxiliar.
	BITMAP  *back; // Verso.
	BITMAP  *logo; // Logo do jogo.
    
    PALETTE	pal; // Declara��o da paleta de cores.
    
	// Declara��o dos �udios:
    MIDI	*musica;
	SAMPLE	*flip;
	SAMPLE  *vitoria;
	SAMPLE  *saida;
	SAMPLE  *welcome;
	SAMPLE  *errou;
	SAMPLE  *acertou;

	tela = create_bitmap(MAX_X, MAX_Y); // Cria o tabuleiro.

	// Carregar o vetor gabarito com os bitmaps das cartas e o logo:
    logo = load_bitmap("figuras\\logo.bmp", pal);
	back = load_bitmap("figuras\\verso.bmp", pal);
    rcard[0] = load_bitmap("figuras\\carta1.bmp", pal);
    num[0]=1;
    rcard[1] = load_bitmap("figuras\\carta2.bmp", pal);
    num[1]=2;
    rcard[2] = load_bitmap("figuras\\carta3.bmp", pal);
    num[2]=3;
    rcard[3] = load_bitmap("figuras\\carta4.bmp", pal);
    num[3]=4;
    rcard[4] = load_bitmap("figuras\\carta5.bmp", pal);
    num[4]=5;
    rcard[5] = load_bitmap("figuras\\carta6.bmp", pal);
    num[5]=6;
    rcard[6] = load_bitmap("figuras\\carta7.bmp", pal);
    num[6]=7;
    rcard[7] = load_bitmap("figuras\\carta8.bmp", pal);
	num[7]=8;
    rcard[8] = load_bitmap("figuras\\carta9.bmp", pal);
	num[8]=9;
    rcard[9] = load_bitmap("figuras\\carta10.bmp", pal);
	num[9]=10;
    rcard[10] = load_bitmap("figuras\\carta1.bmp", pal);
	num[10]=1;
    rcard[11] = load_bitmap("figuras\\carta2.bmp", pal);
	num[11]=2;
    rcard[12] = load_bitmap("figuras\\carta3.bmp", pal);
	num[12]=3;
    rcard[13] = load_bitmap("figuras\\carta4.bmp", pal);
	num[13]=4;
    rcard[14] = load_bitmap("figuras\\carta5.bmp", pal);
	num[14]=5;
    rcard[15] = load_bitmap("figuras\\carta6.bmp", pal);
	num[15]=6;
    rcard[16] = load_bitmap("figuras\\carta7.bmp", pal);
	num[16]=7;
    rcard[17] = load_bitmap("figuras\\carta8.bmp", pal);
	num[17]=8;
    rcard[18] = load_bitmap("figuras\\carta9.bmp", pal);
	num[18]=9;
    rcard[19] = load_bitmap("figuras\\carta10.bmp", pal);
	num[19]=10;
	
    // Carregamento dos �udios:
	musica = load_midi("sons\\musica.mid");
	flip = load_sample("sons\\flip.wav");
	vitoria = load_sample("sons\\vitoria.wav");
	saida = load_sample("sons\\saida.wav");
	welcome = load_sample("sons\\welcome.wav");
	errou = load_sample("sons\\errou.wav");
	acertou = load_sample("sons\\acertou.wav");
	
    // Configura��o da paleta de cores:
	set_palette(black_palette);
	pal[4].r=0;
	pal[4].g=0;
	pal[4].b=50;
	
    clear(tela); // Limpa o tabuleiro.
	
    // Associar o tabuleiro � tela do jogo:
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	
	play_midi(musica, TRUE); // Inicia a m�sica de fundo.
	fade_in(pal, 2); // Fade in de entrada para a tela do jogo.
	
	// Mostra o texto de boas-vindas:
	text_mode(-1);
	draw_sprite(screen, logo, x+70, y-110);
	textout(screen, font, "Bem-Vindo ao Hearthstone's Memory Game!", 3 * MAX_X / 10, 200, 8);
	textout(screen, font, " Pressione qualquer tecla para iniciar.", 3 * MAX_X / 10, 2 * MAX_Y / 5, 8);
	textout(screen, font, "Feito por: Luiz Gustavo Nunes - 1o Eng. de Software", 2.3 * MAX_X / 10, 4 * MAX_Y / 5, 8);
	play_sample(welcome, 255, 128, 1000, FALSE);
	clear_keybuf();
	readkey();
	
	// Condi��o de sa�da do jogo:
	if (key[KEY_ESC])
	   esc=1;
	
    // Carregar o vetor de amostra com o bitmap de verso:
	for (i=0; i<20; i++){
        card[i]=back;
        cima[i]=0;
	}
	
    // Zerar o vetor de cartas escolhidas:
	for (i=0; i<2; i++){
        vturn[i]=0;
	}
	
	// Embaralhar as cartas:
	srand (time(NULL));
	for (i=0; i<20; i++){
	    ai=rand()%20;
	    aux=rcard[i];
	    rcard[i]=rcard[ai];
	    rcard[ai]=aux;
	    auxnum=num[i];
	    num[i]=num[ai];
	    num[ai]=auxnum;
    }
    
    // Desenhar as cartas no tabuleiro com anima��o:
	if (esc!=1){
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[0], x, y);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	rest(150);
	play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[1], x+100, y);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[2], x+200, y);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[3], x+300, y);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[4], x+400, y);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[5], x, y+100);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[6], x+100, y+100);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[7], x+200, y+100);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[8], x+300, y+100);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[9], x+400, y+100);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[10], x, y+200);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[11], x+100, y+200);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[12], x+200, y+200);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[13], x+300, y+200);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[14], x+400, y+200);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[15], x, y+300);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[16], x+100, y+300);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[17], x+200, y+300);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[18], x+300, y+300);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    play_sample(flip, 255, 128, 1000, FALSE);
    draw_sprite(tela, card[19], x+400, y+300);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    rest(150);
    }
      
	// Loop do jogo com condi��o de fim:
	while (esc!=1 && (score1+score2)<10)
	{
          
		// Mostra o mouse:
		show_mouse(NULL);
		clear(tela);
		
		// Associar as vari�veis de mouse com o mouse:
		botEsq = (mouse_b & 1) ? (1) : (0);
		botDir = (mouse_b & 2) ? (1) : (0);
		
		// Texto de pontua��o e informa��es de sa�da:
        textout(tela, font, "Presione ESC para sair.", 165, 10, 8);		
		textprintf_centre(tela, font, MAX_X / 2, 65, 8, "JOGADOR 1 - PONTUACAO: %d TENTATIVAS: %d", score1, jogada1);		
		textprintf_centre(tela, font, MAX_X / 2, 90, 8, "JOGADOR 2 - PONTUACAO: %d TENTATIVAS: %d", score2, jogada2);
		
        // Imprime as cartas no tabuleiro:
		draw_sprite(tela, card[0], x, y);
		draw_sprite(tela, card[1], x+100, y);
		draw_sprite(tela, card[2], x+200, y);
		draw_sprite(tela, card[3], x+300, y);
		draw_sprite(tela, card[4], x+400, y);
		draw_sprite(tela, card[5], x, y+100);
		draw_sprite(tela, card[6], x+100, y+100);
		draw_sprite(tela, card[7], x+200, y+100);
		draw_sprite(tela, card[8], x+300, y+100);
		draw_sprite(tela, card[9], x+400, y+100);
		draw_sprite(tela, card[10], x, y+200);
		draw_sprite(tela, card[11], x+100, y+200);
		draw_sprite(tela, card[12], x+200, y+200);
		draw_sprite(tela, card[13], x+300, y+200);
		draw_sprite(tela, card[14], x+400, y+200);
		draw_sprite(tela, card[15], x, y+300);
		draw_sprite(tela, card[16], x+100, y+300);
		draw_sprite(tela, card[17], x+200, y+300);
		draw_sprite(tela, card[18], x+300, y+300);
		draw_sprite(tela, card[19], x+400, y+300);
		
		show_mouse(tela); // Mostra o mouse no tabuleiro por cima das cartas.
		
        // Mostrar o tabuleiro:
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);  
		
		// Primeira escolha:
        while (turn<1){            
              if (key[KEY_ESC]){
                 esc=1;
                 break;
              }
              
              // Rotina de visualiza��o:
              show_mouse(tela);
		      clear(tela);
              botEsq = (mouse_b & 1) ? (1) : (0);
              botDir = (mouse_b & 2) ? (1) : (0);
              textprintf_centre(tela, font, MAX_X / 2, 125, 8, "ESCOLHA A PRIMEIRA CARTA");
              textout(tela, font, "Presione ESC para sair.", 165, 10, 8);				
              textprintf_centre(tela, font, MAX_X / 2, 65, 8, "JOGADOR 1 - PONTUACAO: %d TENTATIVAS: %d", score1, jogada1);
              textprintf_centre(tela, font, MAX_X / 2, 90, 8, "JOGADOR 2 - PONTUACAO: %d TENTATIVAS: %d", score2, jogada2);
              draw_sprite(tela, card[0], x, y);
              draw_sprite(tela, card[1], x+100, y);
              draw_sprite(tela, card[2], x+200, y);
              draw_sprite(tela, card[3], x+300, y);
              draw_sprite(tela, card[4], x+400, y);
              draw_sprite(tela, card[5], x, y+100);
              draw_sprite(tela, card[6], x+100, y+100);
              draw_sprite(tela, card[7], x+200, y+100);
              draw_sprite(tela, card[8], x+300, y+100);
              draw_sprite(tela, card[9], x+400, y+100);
              draw_sprite(tela, card[10], x, y+200);
              draw_sprite(tela, card[11], x+100, y+200);
              draw_sprite(tela, card[12], x+200, y+200);
              draw_sprite(tela, card[13], x+300, y+200);
              draw_sprite(tela, card[14], x+400, y+200);
              draw_sprite(tela, card[15], x, y+300);
              draw_sprite(tela, card[16], x+100, y+300);
              draw_sprite(tela, card[17], x+200, y+300);
              draw_sprite(tela, card[18], x+300, y+300);
              draw_sprite(tela, card[19], x+400, y+300);
              show_mouse(tela);
              blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
              
              // Sensor de carta escolhida:
              if (botEsq==1){
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+96 && mouse_y>=y && cima[0]!=1){
                                card[0]=rcard[0];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[0]=1;
                                vturn[0]=0;
                                turn++;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+96 && mouse_y>=y && cima[1]!=1){
                                card[1]=rcard[1];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[1]=1;
                                turn++;
                                vturn[0]=1;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+96 && mouse_y>=y && cima[2]!=1){
                                card[2]=rcard[2];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[2]=1;
                                turn++;
                                vturn[0]=2;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+96 && mouse_y>=y && cima[3]!=1){
                                card[3]=rcard[3];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[3]=1;
                                turn++;
                                vturn[0]=3;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+96 && mouse_y>=y && cima[4]!=1){
                                card[4]=rcard[4];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[4]=1;
                                turn++;
                                vturn[0]=4;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[5]!=1){
                                card[5]=rcard[5];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[5]=1;
                                turn++;
                                vturn[0]=5;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[6]!=1){
                                card[6]=rcard[6];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[6]=1;
                                turn++;
                                vturn[0]=6;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[7]!=1){
                                card[7]=rcard[7];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[7]=1;
                                turn++;
                                vturn[0]=7;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[8]!=1){
                                card[8]=rcard[8];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[8]=1;
                                turn++;
                                vturn[0]=8;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[9]!=1){
                                card[9]=rcard[9];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[9]=1;
                                turn++;
                                vturn[0]=9;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[10]!=1){
                                card[10]=rcard[10];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[10]=1;
                                turn++;
                                vturn[0]=10;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[11]!=1){
                                card[11]=rcard[11];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[11]=1;
                                turn++;
                                vturn[0]=11;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[12]!=1){
                                card[12]=rcard[12];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[12]=1;
                                turn++;
                                vturn[0]=12;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[13]!=1){
                                card[13]=rcard[13];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[13]=1;
                                turn++;
                                vturn[0]=13;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[14]!=1){
                                card[14]=rcard[14];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[14]=1;
                                turn++;
                                vturn[0]=14;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[15]!=1){
                                card[15]=rcard[15];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[15]=1;
                                turn++;
                                vturn[0]=15;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[16]!=1){
                                card[16]=rcard[16];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[16]=1;
                                turn++;
                                vturn[0]=16;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[17]!=1){
                                card[17]=rcard[17];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[17]=1;
                                turn++;
                                vturn[0]=17;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[18]!=1){
                                card[18]=rcard[18];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[18]=1;
                                turn++;
                                vturn[0]=18;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[19]!=1){
                                card[19]=rcard[19];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[19]=1;
                                turn++;
                                vturn[0]=19;
                             }
              }      
       	}
       	
        if (key[KEY_ESC]){
                 esc=1;
                 break;
        }
        
       	// Segunda escolha:
       	while (turn<2){
              if (key[KEY_ESC]){
                 esc=1;
                 break;
              }
              show_mouse(tela);
              clear(tela);
              botEsq = (mouse_b & 1) ? (1) : (0);
              botDir = (mouse_b & 2) ? (1) : (0);
              textprintf_centre(tela, font, MAX_X / 2, 125, 8, "ESCOLHA A SEGUNDA CARTA");
              textout(tela, font, "Pressione ESC para sair.", 165, 10, 8);				
              textprintf_centre(tela, font, MAX_X / 2, 65, 8, "JOGADOR 1 - PONTUACAO: %d TENTATIVAS: %d", score1, jogada1);
              textprintf_centre(tela, font, MAX_X / 2, 90, 8, "JOGADOR 2 - PONTUACAO: %d TENTATIVAS: %d", score2, jogada2);
              draw_sprite(tela, card[0], x, y);
              draw_sprite(tela, card[1], x+100, y);
              draw_sprite(tela, card[2], x+200, y);
              draw_sprite(tela, card[3], x+300, y);
              draw_sprite(tela, card[4], x+400, y);
              draw_sprite(tela, card[5], x, y+100);
              draw_sprite(tela, card[6], x+100, y+100);
              draw_sprite(tela, card[7], x+200, y+100);
              draw_sprite(tela, card[8], x+300, y+100);
              draw_sprite(tela, card[9], x+400, y+100);
              draw_sprite(tela, card[10], x, y+200);
              draw_sprite(tela, card[11], x+100, y+200);
              draw_sprite(tela, card[12], x+200, y+200);
              draw_sprite(tela, card[13], x+300, y+200);
              draw_sprite(tela, card[14], x+400, y+200);
              draw_sprite(tela, card[15], x, y+300);
              draw_sprite(tela, card[16], x+100, y+300);
              draw_sprite(tela, card[17], x+200, y+300);
              draw_sprite(tela, card[18], x+300, y+300);
              draw_sprite(tela, card[19], x+400, y+300);
              show_mouse(tela);
              blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
              if (botEsq==1){
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+96 && mouse_y>=y && cima[0]!=1){
                                card[0]=rcard[0];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[0]=1;
                                vturn[1]=0;
                                turn++;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+96 && mouse_y>=y && cima[1]!=1){
                                card[1]=rcard[1];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[1]=1;
                                turn++;
                                vturn[1]=1;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+96 && mouse_y>=y && cima[2]!=1){
                                card[2]=rcard[2];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[2]=1;
                                turn++;
                                vturn[1]=2;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+96 && mouse_y>=y && cima[3]!=1){
                                card[3]=rcard[3];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[3]=1;
                                turn++;
                                vturn[1]=3;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+96 && mouse_y>=y && cima[4]!=1){
                                card[4]=rcard[4];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[4]=1;
                                turn++;
                                vturn[1]=4;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[5]!=1){
                                card[5]=rcard[5];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[5]=1;
                                turn++;
                                vturn[1]=5;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[6]!=1){
                                card[6]=rcard[6];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[6]=1;
                                turn++;
                                vturn[1]=6;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[7]!=1){
                                card[7]=rcard[7];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[7]=1;
                                turn++;
                                vturn[1]=7;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[8]!=1){
                                card[8]=rcard[8];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[8]=1;
                                turn++;
                                vturn[1]=8;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+100+96 && mouse_y>=y+100 && cima[9]!=1){
                                card[9]=rcard[9];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[9]=1;
                                turn++;
                                vturn[1]=9;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[10]!=1){
                                card[10]=rcard[10];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[10]=1;
                                turn++;
                                vturn[1]=10;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[11]!=1){
                                card[11]=rcard[11];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[11]=1;
                                turn++;
                                vturn[1]=11;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[12]!=1){
                                card[12]=rcard[12];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[12]=1;
                                turn++;
                                vturn[1]=12;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[13]!=1){
                                card[13]=rcard[13];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[13]=1;
                                turn++;
                                vturn[1]=13;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+200+96 && mouse_y>=y+200 && cima[14]!=1){
                                card[14]=rcard[14];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[14]=1;
                                turn++;
                                vturn[1]=14;
                             }
                             if (mouse_x>=x && mouse_x<=x+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[15]!=1){
                                card[15]=rcard[15];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[15]=1;
                                turn++;
                                vturn[1]=15;
                             }
                             if (mouse_x>=x+100 && mouse_x<=x+100+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[16]!=1){
                                card[16]=rcard[16];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[16]=1;
                                turn++;
                                vturn[1]=16;
                             }
                             if (mouse_x>=x+200 && mouse_x<=x+200+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[17]!=1){
                                card[17]=rcard[17];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[17]=1;
                                turn++;
                                vturn[1]=17;
                             }
                             if (mouse_x>=x+300 && mouse_x<=x+300+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[18]!=1){
                                card[18]=rcard[18];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[18]=1;
                                turn++;
                                vturn[1]=18;
                             }
                             if (mouse_x>=x+400 && mouse_x<=x+400+71 && mouse_y<=y+300+96 && mouse_y>=y+300 && cima[19]!=1){
                                card[19]=rcard[19];
                                play_sample(flip, 255, 128, 1000, FALSE);
                                cima[19]=1;
                                turn++;
                                vturn[1]=19;
                             }
              }      
       	}
       	
       	if (key[KEY_ESC]){
           esc=1;
           break;
        }
       	// Rotina de visualiza��o:
        show_mouse(tela);
		clear(tela);
        botEsq = (mouse_b & 1) ? (1) : (0);
        botDir = (mouse_b & 2) ? (1) : (0);
        textout(tela, font, "Presione ESC para sair.", 165, 10, 8);						
        textprintf_centre(tela, font, MAX_X / 2, 65, 8, "JOGADOR 1 - PONTUACAO: %d TENTATIVAS: %d", score1, jogada1);		
		textprintf_centre(tela, font, MAX_X / 2, 90, 8, "JOGADOR 2 - PONTUACAO: %d TENTATIVAS: %d", score2, jogada2);
        draw_sprite(tela, card[0], x, y);
        draw_sprite(tela, card[1], x+100, y);
        draw_sprite(tela, card[2], x+200, y);
        draw_sprite(tela, card[3], x+300, y);
        draw_sprite(tela, card[4], x+400, y);
        draw_sprite(tela, card[5], x, y+100);
        draw_sprite(tela, card[6], x+100, y+100);
        draw_sprite(tela, card[7], x+200, y+100);
        draw_sprite(tela, card[8], x+300, y+100);
        draw_sprite(tela, card[9], x+400, y+100);
        draw_sprite(tela, card[10], x, y+200);
        draw_sprite(tela, card[11], x+100, y+200);
        draw_sprite(tela, card[12], x+200, y+200);
        draw_sprite(tela, card[13], x+300, y+200);
        draw_sprite(tela, card[14], x+400, y+200);
        draw_sprite(tela, card[15], x, y+300);
        draw_sprite(tela, card[16], x+100, y+300);
        draw_sprite(tela, card[17], x+200, y+300);
        draw_sprite(tela, card[18], x+300, y+300);
        draw_sprite(tela, card[19], x+400, y+300);
        blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
        
       	// Decis�o de pontua��o:
       	if (num[vturn[0]]==num[vturn[1]]){
       	   turn=0;
       	   rest(300);
       	   play_sample(acertou, 120, 128, 1000, FALSE);
           textout_centre(screen, font, "UM PAR!", MAX_X / 2, 1 * MAX_Y / 5, 8);
       	   if (round%2==0)
              score1++;
              else
                  score2++;
           rest(1000);
        }
           else{
                rest(300);
                play_sample(errou, 255, 128, 1000, FALSE);
                textout_centre(screen, font, "ERROU!", MAX_X / 2, 1 * MAX_Y / 5, 8);
                rest(1000);
                
                // Desvirar cartas:
                card[vturn[0]]=back;
                cima[vturn[0]]=0;
                card[vturn[1]]=back;
                cima[vturn[1]]=0;
                play_sample(flip, 255, 128, 1000, FALSE);
                turn=0;
           }
           
        // Zerar vetor de cartas escolhidas:
        for (i=0; i<2; i++){
            vturn[i]=0;
        }
        
        // Contador de rodadas:
        if ((round%2)==0)
			jogada1++;
			else 	
				jogada2++;
		round++;
    }
    clear(tela);
    blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    
    // Pontua��o final:
    if (jogada1==0)
       sfinal1=0;
       else
	       sfinal1=(((float)score1/jogada1)*100)+score1*10;
	if (jogada2==0)
	   sfinal2=0;
       else
           sfinal2=(((float)score2/jogada2)*100)+score2*10;
    // Vencedor:
    if (score1==0 && score2==0){
		textout_centre(tela, font, "Jogo encerrado.", MAX_X / 2, 200, 8);
		play_sample(saida, 255, 128, 1000, FALSE);
    }
		else {
             if (sfinal1>sfinal2){
				textout_centre(tela, font, "PARABENS JOGADOR 1, VOCE VENCEU!", MAX_X / 2, 200, 8);
				rest(200);
                play_sample(vitoria, 255, 128, 1000, FALSE);
             }
				else if (sfinal2>sfinal1) {
					    textout_centre(tela, font, "PARABENS JOGADOR 2, VOCE VENCEU!", MAX_X / 2, 200, 8);
					    rest(200);
                        play_sample(vitoria, 255, 128, 1000, FALSE);
                     }
					    else if (sfinal1==sfinal2)
					         textout_centre(tela, font, "EMPATOU!", MAX_X / 2, 200, 8);	
                             rest(200);
                             play_sample(vitoria, 255, 128, 1000, FALSE);				         
        // Placar:
	    textprintf_centre(tela, font, MAX_X / 2, 150, 8, "JOGADOR 1 - PONTUACAO FINAL: %.0f", sfinal1);
        textprintf_centre(tela, font, MAX_X / 2, 165, 8, "JOGADOR 2 - PONTUACAO FINAL: %.0f", sfinal2);
       }    
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	stop_midi(); // Para a m�sica de fundo.
	// Tela de sa�da:
	textout_centre(screen, font, "Pressione qualquer tecla para sair.", MAX_X / 2, 2 * MAX_Y / 5, 8);
	clear_keybuf();
	readkey();
	fade_out(4); // Efeito de fade out para sa�da do jogo.
}
// Encerramento do Allegro:
void finaliza (void)
{
	allegro_exit();
}
