/**
Implementação controlada pelo GIt
Alunos:
    Fagner de oliveira Bernardo 14.2.4155
    Natália Monferrari Sol 14.2.4335
*/

/*
Comando para compilação no Liux
    Compilação:g++ main.cpp -o compilado -lGL -lGLU -lglut
    Execução: ./compilado
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
//#include <cstdlib>
#include <ctime>
#include<unistd.h>


using namespace std;

typedef struct ponto{
    GLfloat x;
    GLfloat y;
}TPonto;

#define INDICE 25
#define QUAD2X2 4
#define QUAD3X3 9
#define NIVEL_INICIAL 1
#define NUM_EMB_BOMB 10
#define EMBARCACOES 5
#define TAMANHO 40

/*======================DECLARAÇÃO DE VARIÁVEIS GLOBAIS======================*/

/*Especifica aposicao da janela*/
GLfloat posX = 700.0, posY = 150.0, rate = 0.05;

/*Especifica as dimensoes da janela*/
GLfloat windowWidth = 520.0, windowHeight = 440.0;

//turn: guarda as posicoes dos quadrados que vao mudar de cor
//rodada guarda a quantidade de quadrado a memorizar
//onGame é deve ser falsa quando o player perder
//guess: guarda os cliques do jogador na posicao dos objetos na tela
//verificacao: usada para chamar a funcao que avalia o resultado do jogo
//jogoAtual: guarda qual jogo está sendo jogado no momento
//indice: guarda a quantidade de cliques de mouse dados nas posicoes dos quadrados
//tempo: determina o tempo base da duração do quadrado preto

TPonto a, b, c, d;

int turn[INDICE], guess[INDICE], nivel = NIVEL_INICIAL, jogoAtual = 0, indice = 0,
    verificacao = 0, tempo = 500000, flagBlackColor = 0, posicoesEmb[NUM_EMB_BOMB],
    lin[NUM_EMB_BOMB], col[NUM_EMB_BOMB], tabuleiro[TAMANHO][TAMANHO] = {},
    campo[TAMANHO][TAMANHO], linha = 0, coluna = 0, vidas;

//===========================OPEN_GL===========================
void init(void);

void display(void);

/*Função calback chamada para gerenciar eventos do mouse*/
void GerenciaMouse(int button, int state, int x, int y);

//Menu que controla a interação com o programa
void GoMenu(int value);


//===========================GENIUS===========================
/*Gerador dos numeros aleatórios*/
void geraNum2x2();

/*Gerador dos numeros aleatórios*/
void geraNum3x3();

bool confereBomba(int, int);

/*Gerador dos numeros aleatórios*/
void geraNumBatalha();

//Função que desenha 4 quadrados
void quad2x2();

//Função que desenha 9 quadrad
void quad3x3();

//Verifica se a sequencia de cliques confere com a sequencia esperada
void verificaAcerto();

//Controla o jogo da memoria no nivel facil
void jogoMemoriaFacil();

//Controla o jogo da memoria no nivel dificil
void jogoMemoriaDificil();



//===========================BATALHA_NAVAL===========================

//Controla o jogo da batalha naval
void jogoBatalhaNaval();

//Desenha os quadrados do mapa
void desenhaQuadrados();

//Verifica se este elemento pode ser posicionado no local indicado
bool confereDestroyer(int, int);

//Verifica se este elemento pode ser posicionado no local indicado
bool confereSubmarino(int, int);

//Verifica se este elemento pode ser posicionado no local indicado
bool conferePortaAvioes(int, int);

//Verifica se este elemento pode ser posicionado no local indicado
bool confereCorveta(int, int);

//Verifica se este elemento pode ser posicionado no local indicado
bool confereLancaAtaque(int, int);

//Verifica se este elemento pode ser posicionado no local indicado
void posicionaEmbarcacoes();

//Inicializa a o campo de jogo com valores neutros
void inicializaCampo();

//Imprime o tabuleiro com as posições dos elementos reveladas
void imprimeTabuleiroVerdade();

//Imprime o campo de jogo com os valores padrão
void imprimeCampo();

//Converte as coordenadas clicadas da tela em referências para acessar as matrizes
void pixelToIndice(int, int);

//Verifica se o usuário ainda tem vidas
void verificaVidas();



int main (int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /*
    if(jogoAtual == 1 || jogoAtual == 2){
        windowWidth = 520;
        windowHeight = 440;
    }
    else if(jogoAtual == 3){
        windowWidth = 1200;
        windowHeight = 675;
        posX = 1.0;
        posY = 1.0;
    }.
    */
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(posX, posY);
    glutCreateWindow("Menu");

    int sub1 = glutCreateMenu(GoMenu);

    glutAddMenuEntry("Fácil",1);
    glutAddMenuEntry("Difícil",2);

    glutCreateMenu(GoMenu);
    glutAddSubMenu("Memória", sub1);

    glutAddMenuEntry("Batalha Naval",3);
    glutAddMenuEntry("Sair",4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMouseFunc(GerenciaMouse);

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

void GoMenu(int value) {
    switch(value){
    case 1:
        jogoAtual = 1;
        nivel = NIVEL_INICIAL;
        indice = 0;
        geraNum2x2();

        //Zera o vetor de chutes
        for (int i = 0; i < INDICE; i++){
                guess[i] = -1;
        }

        jogoMemoriaFacil();
        break;
    case 2:
        jogoAtual = 2;
        nivel = NIVEL_INICIAL;
        indice = 0;
        geraNum3x3();

        //Zera o vetor de chutes
        for (int i = 0; i < INDICE; i++){
                guess[i] = -1;
        }

        jogoMemoriaDificil();
        break;
    case 3:
        jogoAtual = 3;
        vidas = 3;
        nivel = NIVEL_INICIAL;
        geraNumBatalha();
        jogoBatalhaNaval();
        break;
    case 4:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void init(void) {
	cout << "Entrou init()" << endl;
	//Define a cor do plano de fundo da janela de renderização
    glClearColor(1.0, 1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //inicializa
	//windowWidth = 520;
	//windowHeight = 440;


    //Ponto a
    a.x = -1.0;
    a.y = 0.8;

	//Ponto b
    b.x = -0.8;
    b.y = 0.8;

    //Ponto c
    c.x = -0.8;
    c.y = 1.0;

    //Ponto d
    d.x = -1.0;
    d.y = 1.0;

    /*Determina as configurações do plano cartesiano de projeção*/
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0 ,0.0);
}

void geraNum2x2(){
    cout << "Entrou geraNum2x2()" << endl;

    //Gera uma semente para o tempo
    srand((unsigned) time (NULL));

    //Gera um vetor com 25 numeros aleatorios
    for(int i = 0 ; i < INDICE ; i++){
        turn[i] = rand()%QUAD2X2 +1;
        cout <<  turn[i] << " ";
    }
    cout << endl;
}

void geraNum3x3(){
    cout << "Entrou geraNum3x3()" << endl;

    //Gera uma semente para o tempo
    srand((unsigned) time (NULL));

    //Gera um vetor com 25 numeros aleatorios
    for(int i = 0 ; i < INDICE ; i++){
        turn[i] = rand()%QUAD3X3 +1;
        cout <<  turn[i] << " ";
    }
    cout << endl;
}

bool confereBomba(int linha, int coluna){
    //Confere se não tem nada na posicao
    if(tabuleiro[linha][coluna] == 0){
        tabuleiro[linha][coluna] = 2;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(linha < TAMANHO-1){
            linha++;
            confereBomba(linha, coluna);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if(coluna < TAMANHO-1){
            linha = 0;
            coluna++;
            confereBomba(linha, coluna);
        }
        //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
    //Coloquei esse return
    return false;
}

void geraNumBatalha(){
    cout << "Entrou geraNumBatalha()" << endl;

    //Gera uma semente para o tempo
    srand((unsigned) time (NULL));

    //Gera um vetor com 10 embarcaccoes aleatorias
    for(int i = 0 ; i < NUM_EMB_BOMB ; i++){
        posicoesEmb[i] = rand()%EMBARCACOES +1;
        cout << posicoesEmb[i] << " ";
    }
    //Gera 10 posicoes aleatorias para as bombas
    for(int j =0; j < NUM_EMB_BOMB; j++){
        linha = rand()%TAMANHO;
        coluna = rand()%TAMANHO;
        //Confere a posicao da bomba está vazia
        while(!confereBomba(linha, coluna)){
            linha = rand()%TAMANHO;
            coluna = rand()%TAMANHO;
        }
    }
    cout << endl;
}

void quad2x2(){

    if(flagBlackColor == 1){
        glColor3f(0.0, 0.0, 0.0);
        cout << "Black color: " << flagBlackColor << " direito superior" << endl;
    }
    else{
        glColor3f(1.0, 1.0, 0.0);
    }

    /*Gera um poligono direita superior*/
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.5,0.0,0.0);
        glVertex3f(0.5,0.5,0.0);
        glVertex3f(0.0,0.5,0.0);
    glEnd();

    if(flagBlackColor == 2){
        glColor3f(0.0, 0.0, 0.0);
        cout << "Black color: " << flagBlackColor << " esquerdo superior" << endl;
    }
    else{
        glColor3f(1.0, 0.0, 0.0);
    }
    /*Gera um poligono esquerda superior*/
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(-0.5,0.5,0.0);
        glVertex3f(-0.5,0.0,0.0);
    glEnd();

    if(flagBlackColor == 3){
        glColor3f(0.0, 0.0, 0.0);
        cout << "Black color: " << flagBlackColor << " esquerdo inferior" << endl;
    }
    else{
        glColor3f(0.0, 0.0, 1.0);

    }
    /*Gera um poligono esquerda inferior*/
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(-0.5,0.0,0.0);
        glVertex3f(-0.5,-0.5,0.0);
        glVertex3f(0.0,-0.5,0.0);
    glEnd();

    if(flagBlackColor == 4){
        glColor3f(0.0, 0.0, 0.0);
        cout << "Black color: " << flagBlackColor << " direito inferior" << endl;
    }
    else{
        glColor3f(0.0, 1.0, 0.0);
    }
    /*Gera um poligono direita inferior*/
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,-0.5,0.0);
        glVertex3f(0.5,-0.5,0.0);
        glVertex3f(0.5,0.0,0.0);
    glEnd();
    glFlush();
}

void quad3x3(){
    if(flagBlackColor == 1){
        cout << "Black color: " << flagBlackColor << " direito direita L1" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(1.0, 1.0, 0.0);
    }

    /*Gera um poligono direita L1*/
    glBegin(GL_POLYGON);
        glVertex3f(0.6,0.6,0.0);
        glVertex3f(0.2,0.6,0.0);
        glVertex3f(0.2,0.2,0.0);
        glVertex3f(0.6,0.2,0.0);
    glEnd();

    if(flagBlackColor == 2){
        cout << "Black color: " << flagBlackColor << " central L1" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(1.0, 0.0, 0.0);
    }

    /*Gera um poligono central L1*/
    glBegin(GL_POLYGON);
        glVertex3f(0.2,0.6,0.0);
        glVertex3f(-0.2,0.6,0.0);
        glVertex3f(-0.2,0.2,0.0);
        glVertex3f(0.2,0.2,0.0);
    glEnd();

    if(flagBlackColor == 3){
        cout << "Black color: " << flagBlackColor << " esquerda L1" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.0, 0.0, 1.0);
    }

    /*Gera um poligono esquerda L1*/
    glBegin(GL_POLYGON);
        glVertex3f(-0.2,0.6,0.0);
        glVertex3f(-0.6,0.6,0.0);
        glVertex3f(-0.6,0.2,0.0);
        glVertex3f(-0.2,0.2,0.0);
    glEnd();

    if(flagBlackColor == 4){
        cout << "Black color: " << flagBlackColor << " direita L2" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.5, 0.5, 0.0);
    }

    /*Gera um poligono direita L2*/
    glBegin(GL_POLYGON);
        glVertex3f(0.6,-0.2,0.0);
        glVertex3f(0.2,-0.2,0.0);
        glVertex3f(0.2,0.2,0.0);
        glVertex3f(0.6,0.2,0.0);
    glEnd();

    if(flagBlackColor == 5){
        cout << "Black color: " << flagBlackColor << " central L2" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.0, 0.5, 0.5);
    }

    /*Gera um poligono central L2*/
    glBegin(GL_POLYGON);
        glVertex3f(0.2,-0.2,0.0);
        glVertex3f(-0.2,-0.2,0.0);
        glVertex3f(-0.2,0.2,0.0);
        glVertex3f(0.2,0.2,0.0);
    glEnd();

    if(flagBlackColor == 6){
        cout << "Black color: " << flagBlackColor << " esquerda L2" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.5, 0.0, 0.5);
    }

    /*Gera um poligono esquerda L2*/
    glBegin(GL_POLYGON);
        glVertex3f(-0.2,-0.2,0.0);
        glVertex3f(-0.6,-0.2,0.0);
        glVertex3f(-0.6,0.2,0.0);
        glVertex3f(-0.2,0.2,0.0);
    glEnd();

    if(flagBlackColor == 7){
        cout << "Black color: " << flagBlackColor << " direita L3" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(1.0, 0.5, 5.0);
    }

    /*Gera um poligono direita L3*/
    glBegin(GL_POLYGON);
        glVertex3f(0.6,-0.6,0.0);
        glVertex3f(0.2,-0.6,0.0);
        glVertex3f(0.2,-0.2,0.0);
        glVertex3f(0.6,-0.2,0.0);
    glEnd();

    if(flagBlackColor == 8){
        cout << "Black color: " << flagBlackColor << " central L3" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.5, 0.5, 1.0);
    }

    /*Gera um poligono central L3*/
    glBegin(GL_POLYGON);
        glVertex3f(0.2,-0.6,0.0);
        glVertex3f(-0.2,-0.6,0.0);
        glVertex3f(-0.2,-0.2,0.0);
        glVertex3f(0.2,-0.2,0.0);
    glEnd();

    if(flagBlackColor == 9){
        cout << "Black color: " << flagBlackColor << " esquerda L3" << endl;
        glColor3f(0.0, 0.0, 0.0);
    }
    else{
        glColor3f(0.5, 1.0, 0.5);
    }

    /*Gera um poligono esquerda L3*/
    glBegin(GL_POLYGON);
        glVertex3f(-0.2,-0.6,0.0);
        glVertex3f(-0.6,-0.6,0.0);
        glVertex3f(-0.6,-0.2,0.0);
        glVertex3f(-0.2,-0.2,0.0);
    glEnd();
    glFlush();
}

void desenhaQuadrados(){
    float passo = 0.0;
    int opcao;
    cout << "Entrou desenhaCubos()" << endl;
    for(int i = 0; i < TAMANHO; i++){
        //cout << "Ponto A: ("<< a.x << "," << a.y << ")" << endl;
        //cout << "Ponto B: ("<< b.x << "," << b.y << ")" << endl;
        //cout << "Ponto C: ("<< c.x << "," << c.y << ")" << endl;
        //cout << "Ponto D: ("<< d.x << "," << d.y << ")" << endl;
        for(int j = 0; j < TAMANHO; j++){
            /*Gera um poligono*/
            glBegin(GL_POLYGON);
				//A regra q ve onde foi clicado fica na gerenciaMouse()
				//Aqui so renderiza os quadrados conforme as escolhas do jogador.

				opcao = campo[i][j];
                //cout << "opcao: " << opcao << endl;

                switch(opcao){
                	//Caso a região clicada seja um navio
                	case 1:{
						//Desenha um quadrado amarelo
						glColor3f(1.0, 1.0, 0.0);
						glVertex3f(a.x + passo, a.y, 0.0);
						glVertex3f(b.x + passo, b.y, 0.0);
						glVertex3f(c.x + passo, c.y, 0.0);
						glVertex3f(d.x + passo, d.y, 0.0);
						break;
                	}
                	case 2:{
                		//Caso a região clicada seja uma bomba
                		//Desenha um quadrado vermelho
						glColor3f(1.0, 0.0, 0.0);
						glVertex3f(a.x + passo, a.y, 0.0);
						glVertex3f(b.x + passo, b.y, 0.0);
						glVertex3f(c.x + passo, c.y, 0.0);
						glVertex3f(d.x + passo, d.y, 0.0);
                		break;
                	}
                	case 0:{
                		//Caso a região clicada seja o mar
						//Desenha um quadrado azulinho
						glColor3f(0.0, 0.0, 1.0);
						glVertex3f(a.x + passo, a.y, 0.0);
						glVertex3f(b.x + passo, b.y, 0.0);
						glVertex3f(c.x + passo, c.y, 0.0);
						glVertex3f(d.x + passo, d.y, 0.0);
                		break;
                	}
                	case -1:{
						//Alterna entre as cores dpadrão dos quadrados
						if(j%2 == 0){
								if (i%2 == 0){
									glColor3f(0.1, 0.1, 0.1);
								}
								else{
									glColor3f(0.0, 0.0, 0.0);
								}
						}
						else{
								if (i%2 == 0){
									glColor3f(0.0, 0.0, 0.0);

								}
								else{
									glColor3f(0.1, 0.1, 0.1);
								}
						}
						//Desenha um quadrado com as cores padrão
						glVertex3f(a.x + passo, a.y, 0.0);
						glVertex3f(b.x + passo, b.y, 0.0);
						glVertex3f(c.x + passo, c.y, 0.0);
						glVertex3f(d.x + passo, d.y, 0.0);
                	}
                }
            glEnd();
			//muda as coordenadas base para desenhar outro quadrado na prox iteracao
            passo += rate;
        }
        //Retorna ao inicio da linha
        passo = 0.0;

        // Desce p a proxima linha de renderização
        a.y -= rate;
        b.y -= rate;
        c.y -= rate;
        d.y -= rate;

    }
    glFlush();

    a.x = -1.0;
    a.y = 0.8;

    b.x = -0.8;
    b.y = 0.8;

    c.x = -0.8;
    c.y = 1.0;

    d.x = -1.0;
    d.y = 1.0;
}

bool confereDestroyer(int lin, int col){
    //Confere se não tem nada na posicao
    if(tabuleiro[lin][col] == 0 && tabuleiro[lin][(col+1)] == 0 && tabuleiro[(lin+1)][(col+1)] == 0 && tabuleiro[(lin+1)][(col+2)]== 0 && tabuleiro[(lin+1)][(col+3)] == 0){
        tabuleiro[lin][col] = 1;
        tabuleiro[lin][(col+1)] = 1;
        tabuleiro[(lin+1)][(col+1)] = 1;
        tabuleiro[(lin+1)][(col+2)] = 1;
        tabuleiro[(lin+1)][(col+3)] = 1;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(lin < TAMANHO-2){
            lin++;
            confereDestroyer(lin, col);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if (col < TAMANHO-4) {
            lin = 0;
            col++;
            confereDestroyer(lin, col);
        }
         //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
    //Coloquei esse return
    return false;
}

bool confereSubmarino(int lin, int col){
    //Confere se não tem nada na posicao
    if(tabuleiro[lin][col] == 0 && tabuleiro[lin][(col+1)] == 0 && tabuleiro[lin][(col+2)] == 0 && tabuleiro[lin][(col+3)] == 0 && tabuleiro[lin][(col+4)] == 0 && tabuleiro[(lin-1)][(col+2)] == 0){
        tabuleiro[lin][col] = 1;
        tabuleiro[lin][(col+1)] = 1;
        tabuleiro[lin][(col+2)] = 1;
        tabuleiro[lin][(col+3)] = 1;
        tabuleiro[lin][(col+4)] = 1;
        tabuleiro[(lin-1)][(col+2)] = 1;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(lin < TAMANHO-1){
            lin++;
            confereSubmarino(lin, col);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if (col < TAMANHO-5) {
            lin = 0;
            col++;
            confereSubmarino(lin, col);
        }
         //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
	//Coloquei esse return
    return false;
}

bool conferePortaAvioes(int lin, int col){
    //Confere se não tem nada na posicao
    if(tabuleiro[lin][col] == 0 && tabuleiro[lin][(col+1)] == 0 && tabuleiro[lin][(col+2)] == 0 && tabuleiro[lin][(col+3)] == 0 && tabuleiro[(lin+1)][(col+2)] == 0 && tabuleiro[(lin+1)][(col+3)] == 0 && tabuleiro[(lin+1)][(col+4)] == 0 && tabuleiro[(lin+1)][(col+5)] == 0){
        tabuleiro[lin][col] = 1;
        tabuleiro[lin][(col+1)] = 1;
        tabuleiro[lin][(col+2)] = 1;
        tabuleiro[lin][(col+3)] = 1;
        tabuleiro[(lin+1)][(col+2)] = 1;
        tabuleiro[(lin+1)][(col+3)] = 1;
        tabuleiro[(lin+1)][(col+4)] =1;
        tabuleiro[(lin+1)][(col+5)] = 1;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(lin < TAMANHO-2){
            lin++;
            conferePortaAvioes(lin, col);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if (col < TAMANHO-6) {
            lin = 0;
            col++;
            conferePortaAvioes(lin, col);
        }
         //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
	//Coloquei esse return
    return false;
}

bool confereCorveta(int lin, int col){
    //Confere se não tem nada na posicao
    if(tabuleiro[lin][col] == 0 && tabuleiro[lin][(col+1)] == 0 && tabuleiro[(lin+1)][(col+1)] == 0 ){
        tabuleiro[lin][col] = 1;
        tabuleiro[lin][(col+1)] = 1;
        tabuleiro[(lin+1)][(col+1)] = 1;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(lin < TAMANHO-2){
            lin++;
            confereCorveta(lin, col);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if (col < TAMANHO-2) {
            lin = 0;
            col++;
            confereCorveta(lin, col);
        }
         //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
	//Coloquei esse return
    return false;
}

bool confereLancaAtaque(int lin, int col){
    //Confere se não tem nada na posicao
    if(tabuleiro[lin][col] == 0 && tabuleiro[lin][(col+1)] == 0 && tabuleiro[(lin+1)][(col+2)] == 0){
        tabuleiro[lin][col] = 1;
        tabuleiro[lin][(col+1)] = 1;
        tabuleiro[(lin+1)][(col+2)] = 1;
        return true;
    }
    else{
        //Se houver, anda uma linha até achar um lugar vazio
        if(lin < TAMANHO-2){
            lin++;
            confereLancaAtaque(lin, col);
        }
        //Anda uma coluna para a direita até achar um lugar vazio
        else if (col < TAMANHO-3) {
            lin = 0;
            col++;
            confereLancaAtaque(lin, col);
        }
         //Caso não ache lugar vazio, retorna falso. Dentro do while será gerado outra posicao aleatoria
        else{
            return false;
        }
    }
	//Coloquei esse return
    return false;
}

void posicionaEmbarcacoes(){

    for(int i = 0 ; i < NUM_EMB_BOMB ; i++){
        //Destroyer
        if(posicoesEmb[i] == 1){
            //gera posicao aleatoria para o destroyer
            lin[i] = rand()%(TAMANHO-1) ;
            col[i] = rand()%(TAMANHO-3);
            //cout << "Lin["<<i <<"]: " <<  lin[i] << "Col: " << col[i] << endl;
            //Confere a posicao do destroyer está vazia
            while (!confereDestroyer(lin[i],col[i])){
                lin[i] = rand()%(TAMANHO-1);
                col[i] = rand()%(TAMANHO-3);
            }

        }
        //Submarino
        else if(posicoesEmb[i] == 2){
                //gera posicao aleatoria para o submarino
            lin[i] = rand()%(TAMANHO-1)+1;
            col[i] = rand()%(TAMANHO-4);
            //cout << "Lin["<<i <<"]: " <<  lin[i] << "Col: " << col[i] << endl;
            //Confere a posicao do submarino está vazia
             while (!confereSubmarino(lin[i],col[i])){
                lin[i] = rand()%(TAMANHO-1)+1 ;
                col[i] = rand()%(TAMANHO-4);
            }
        }
        else if(posicoesEmb[i] == 3){
                //gera posicao aleatoria para o porta avioes
            lin[i] = rand()%(TAMANHO-1);
            col[i] = rand()%(TAMANHO-5);
            //cout << "Lin["<<i <<"]: " <<  lin[i] << "Col: " << col[i] << endl;
            //Confere a posicao do porta avioes está vazia
             while (!conferePortaAvioes(lin[i],col[i])){
                lin[i] = rand()%(TAMANHO-1) ;
                col[i] = rand()%(TAMANHO-5);
            }
        }
        else if(posicoesEmb[i] == 4){
                //gera posicao aleatoria para a corveta
            lin[i] = rand()%(TAMANHO-1);
            col[i] = rand()%(TAMANHO-1);
            //cout << "Lin["<<i <<"]: " <<  lin[i] << "Col: " << col[i] << endl;
            //Confere a posicao da corveta está vazia
             while (!confereCorveta(lin[i],col[i])){
                lin[i] = rand()%(TAMANHO-1) ;
                col[i] = rand()%(TAMANHO-1);
            }
        }
        else{
                //gera posicao aleatoria para a lanca de ataque
            lin[i] = rand()%(TAMANHO-1);
            col[i] = rand()%(TAMANHO-2);
            //cout << "Lin["<<i <<"]: " <<  lin[i] << "Col: " << col[i] << endl;
            //Confere a posicao da lanca de ataque está vazia
            while (!confereLancaAtaque(lin[i],col[i])){
                lin[i] = rand()%(TAMANHO-1) ;
                col[i] = rand()%(TAMANHO-2);
            }
        }

    }
}

void imprimeTabuleiroVerdade(){
    for(int i = 0; i < TAMANHO; i++){
        cout << i << "      " ;
        for(int j = 0; j < TAMANHO; j++){
            cout << tabuleiro[i][j] << " " ;
        }
        cout << endl;
    }
}

void imprimeCampo(){
    for(int i = 0; i < TAMANHO; i++){
        cout << i << "      " ;
        for(int j = 0; j < TAMANHO; j++){
            cout << campo[i][j] << " " ;
        }
        cout << endl;
    }
}

void inicializaCampo(){
	for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            campo[i][j]  = -1;
        }
    }
}

void display(void){
    cout << "Entrou display()" << endl;

    glClear(GL_COLOR_BUFFER_BIT);
    // Limpa a janela
    glMatrixMode(GL_PROJECTION);
    //Evita erro de projeção da imagem
    glLoadIdentity();

    cout << "jogoAtual: " << jogoAtual << endl;

    if(jogoAtual == 1){
        quad2x2();
    }
    else{
        if (jogoAtual == 2){
            quad3x3();
        }
        else{
            if (jogoAtual == 3){
                desenhaQuadrados();
                //desenhaGrade();
            }
        }
    }
    cout << "Saiu display()" << endl;
}

void GerenciaMouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        cout << "Pixel x: " << x << endl;
        cout << "Pixel y: " << y << endl;
    }

    //Usa estas regras para o mouse se o jogo for o nivel fácil
    if(jogoAtual == 1){
                //Região do quadrado superior direito
        if (x > 260 && x < 389 && y > 110 && y < 218 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 1;
            indice++;
            cout << "Direito L1" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado superior esquerdo
        if (x > 130 && x < 260 && y > 110 && y < 218 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 2;
            indice++;
            cout << "Esquerdo L1" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado inferior esquerdo
        if (x > 130 && x < 260 && y > 218 && y < 329 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 3;
            indice++;
            cout << "Esquerdo L2" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado inferior direito
        if (x > 260 && x < 389 && y > 218 && y < 329 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 4;
            indice++;
            cout << "Direito L2" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }
    }

    if(jogoAtual == 2){
        //Região do quadrado direito L1
        if (x > 313 && x < 415 && y > 88 && y < 174 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 1;
            indice++;
            cout << "Direito L1" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do meio L1
        if (x > 209 && x < 311 && y > 88 && y < 174 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 2;
            indice++;
            cout << "Meio L1" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do esquerdo L1
        if (x > 104 && x < 207 && y > 88 && y < 174 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 3;
            indice++;
            cout << "Esquerdo L1" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado direito L2
        if (x > 313 && x < 415 && y > 176 && y < 263 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 4;
            indice++;
            cout << "Direito L2" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado meio L2
        if (x > 209 && x < 311 && y > 176 && y < 263 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 5;
            indice++;
            cout << "Meio L2" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado esquerdo L2
        if (x > 104 && x < 207 && y > 176 && y < 263 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 6;
            indice++;
            cout << "Esquerdo L2" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado direito L3
        if (x > 260 && x < 389 && y > 265 && y < 351 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 7;
            indice++;
            cout << "Direito L3" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado meio L3
        if (x > 209 && x < 311 && y > 265 && y < 351 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 8;
            indice++;
            cout << "Meio L3" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }

        //Região do quadrado esquerdo L3
        if (x > 104 && x < 207 && y > 265 && y < 351 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            guess[indice] = 9;
            indice++;
            cout << "Esquerdo L3" << endl;
            if(indice == nivel){
                verificacao = 1;
                verificaAcerto();
            }
        }
    }

    if(jogoAtual == 3){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			//Converte as coordenadas da tela de pixel para inteiro
			pixelToIndice(x, y);
			verificaVidas();
        }
    }
    glutPostRedisplay();
}

void jogoMemoriaFacil(){
    cout << endl;
    cout << "Entrou jogoMemoriaFacil()" << endl;
    int i;

    
    if(nivel == 1){
        cout << "Pintou c a cor preta" << endl;
        display();
        usleep(800000);
    }
    

    // Controla cada jogada até o player errar
    for(i = 0; i < nivel ; i++){        
        usleep(tempo - (10000*nivel));
        //Pinta um quadrado de preto
        flagBlackColor = turn[i];

        cout << "Pintou c a cor preta" << endl;
        display();

        cout << "tempo: " << tempo - (10000*nivel) << endl;

        usleep(tempo - (10000*nivel));

        //Pinta o quadrado com a cor original
        flagBlackColor = 0;
        cout << "Pintou c a cor original" << endl;
        display();
    }
}

void jogoMemoriaDificil(){
    cout << endl;
    cout << "Entrou jogo da memoria Dificil" << endl;
    int i;

    // Controla cada jogada até o player errar
    for(i = 0; i < nivel ; i++){
        usleep(tempo - (10000*nivel));
        //Pinta um quadrado de preto
        flagBlackColor = turn[i];

        cout << "Pintou c a cor preta" << endl;
        display();
        
        cout << "tempo: " << tempo - (10000*nivel) << endl;
        usleep(tempo - (10000*nivel));

        //Pinta o quadrado com a cor original
        flagBlackColor = 0;
        cout << "Pintou c a cor original" << endl;
        display();
    }
}

void jogoBatalhaNaval(){
	inicializaCampo();
    posicionaEmbarcacoes();
	display();
}

void verificaAcerto(){
    cout << endl;
    cout << "Entrou verificaAcerto()" << endl;

    int cont = 0;    

    //Conta a quantidade de acertos do jogador
    if(verificacao == 1){
        for(int j = 0 ; j < indice ; j++){
            if (turn[j] == guess[j]){
                cont++;
            }
        }

        cout << "valor do indice: " << indice << endl;
        cout << "valor do cont: " << cont << endl;

        //Verifica se as escolhas do jogador batem com a sequencia certa
        if(cont == indice){
            cout << "Voce acertou!" << endl;
            indice = 0;

            if(nivel <= 25){
                nivel++;
                cout << "Nivel: " << nivel << endl;

                //Pausa antes de reiniciar a sequencia
                usleep(500000);

                //Inicia o próximo nivel do jogo conforme o jogo que esta sendo jogado
                switch(jogoAtual){
                    case 1:{
                        jogoMemoriaFacil();
                        break;
                    }
                    case 2:{
                        jogoMemoriaDificil();
                        break;
                    }
                }
            }
            else{
                exit(0);
            }
        }
        else{
            cout << "Voce errou!" << endl;
            exit(0);
        }
    }
}

/*Necessário fazer uma função que atualiza os valores da windowHeigth e windowWidth para pixelToIndice funcionar para
qualquer tamanho de janela*/
void pixelToIndice(int x, int y){
    cout << endl;
    cout << "Entrou pixelToInt" << endl;
	//Variaveis que terão o valor que representa a relacao entre Pixel tamanho da tela
	float refX, refY;
	int i, j;
	//Gera a referência da escala para o calculo das coordenadas i e j da matriz
	cout << "windowWidth: " << windowWidth << endl;
	cout << "windowHeight: " << windowHeight << endl;
	cout << "TAMANHO: " << TAMANHO << endl;
	refX = windowWidth / TAMANHO;
	refY = windowHeight / TAMANHO;

	//Conversao explicita de float para inteiro
	j = int(x / refX);
	i = int(y / refY);

	//Passa a informação do tabuleiro verdade para o campo de jogo
	campo[i][j] = tabuleiro[i][j];

	//Verifica se o jogador pisou numa bomba
	if (tabuleiro[i][j] == 2){
		cout << "Perdeu 1 vida" << endl;
		vidas -= 1;
	}

	cout << "refX: " << refX << endl;
	cout << "refY: " << refY << endl;
	cout << "Indice i: " << i << endl;
	cout << "Indice j: " << j << endl;
    return;
}

void verificaVidas(){
	if (vidas <= 0){
		cout << "Fim de jogo" << endl;
		exit(0);
	}
}
