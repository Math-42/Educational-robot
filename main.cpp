#include <SDL.h>
#include "SDL_opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "SDL_image.h"
#include <string.h>

/*
Programa feito com intuito de ser utilizado para criar trajetos
a fim de dar ao brinquedo uma opção feedback utilizando a comparação do
caminho pré planejado com o caminho programado, para se isso basta
desenhar os trajetos necessários e salvar todos os arquivos txt
gerados(arqX e tarefas) na pasta do executável em um cartão SD,
depois,com o auxilio de um adaptador coloca-lo no brinquedo educacional.
*/




/*
Função utilizada para carregar as texturas utilizadas nos botões
*/

GLuint loadTexture(const std::string &fileName);

/*
Função que calcula distancia entre o mouse e um ponto genérico na tela
Entradas:
-Cordenadas x e y do mouse;
-Cordenadas x e y dos pontos;
Saída:
-Distância entre os pontos;
*/

float calculaDistancia(int MouseX, int MouseY, int x, int y){
    return sqrt(((MouseX-x)*(MouseX-x))+((MouseY-y)*(MouseY-y)));
}

/*
Função que desenha, promove a interação com os botões na tela utilizando a posição do mouse e coloca as texturas de cada um respectivamente
Entradas:
-Cordenadas x e y do mouse;
-Variaveis booleanas que definem sobre qual botão o mouse está;
-Variável booleana que define se está ou não lendo as atividades anteriores;
-Texturas;
Saídas:
A função altera diretamente os valores das variáveis boleanas e da uma resposta visual com a interação de mouse e botão;
*/

void botoes(int mouseX, int mouseY, bool *btn1, bool *btn2, bool *btn3,bool lendoAtividades,bool *btn4,bool *btn5,unsigned int btn1_text,unsigned int btn2_text,unsigned int btn3_text,unsigned int btn4_text, unsigned int btn5_text);

/*
Função que desenha todo o background do programa, apenas estética, não há entradas e saídas
*/

void background();

/*
Função que salva todos os pontos selecionados pelo usuário
Entradas:
-Matriz com as cordenadas dos pontos selecionados;
-Variável com o número de seleções feitas;
-O número da útima atividade salva;
Saidas:
-Salva em um arquivo todas as seleções utilizando pares ordenados;
-Limpa as seleções feitas previamente;
*/
void salvarDados(float selecoes[][2],int *contSelecoes);

/*
Função que lê as atividades já salvas pelo usuário
Entradas:
-Matriz com as cordenadas dos pontos selecionados;
-Variável com o número de seleções feitas;
-O número da atividade a ser lida;
Saidas:
-Modifica os valores da matriz de pontos selecionados fazendo com que a atividade lida seja desenhada;
*/

void lerDados(float selecoes[][2], int *conSelecoes, int arquivo);

/*
Função que lê as atividades já salvas pelo usuário e as altera
Entradas:
-Matriz com as cordenadas dos pontos selecionados;
-Variável com o número de seleções feitas;
-O número da atividade a ser lida;
Saidas:
-Modifica os valores da matriz de pontos selecionados podendo modificar assim o desenho já feito;
*/

void alterarDados(float selecoes[][2], int *contSelecoes,int arquivo);

/*
Função que desenha as linhas entre os pontos selecionados
Entradas:
-Matriz com as cordenadas dos pontos selecionados;
-Variável com o número de seleções feitas;
Saidas:
-Desenho entre os pontos já selecionados;
*/

void desenharLinhas(float selecoes[][2],int contSelecoes);

/*
Função que desenha todos os pontos da tela de desenho
Entradas:
-Matriz com as cordenadas de todos os pontos;
Saidas:
-Desenho de todos os pontos;
*/

void desenharMatriz(float matriz[][2]);

/*
Função que define as configurações iniciais das bibliotecas SDL e OpenGl como tamanho da tela, tamanho dos buffers e etc
*/

void definicoesSdlOpenGl();

/*
Função que destaca o ponto sobre o qual o mouse está
Entradas:
-Matriz com a cordenada do ponto selecionado;
-Variável que define se o ponto está ou não selecionado;
Saidas:
-Destaca visualmente o ponto selecionado
*/

void destacarSelecao(bool selecionado,int selecao[][2]);

/*
Função que coloca as imagens dos textos de cada atividade
Entradas:
-Textura a serem mostradas;
Saidas:
-Apenas visual;
*/

void texto(unsigned int texto_text);

int main(int argc,char* args[]){
    definicoesSdlOpenGl();
    //Declaracao de variáveis
    unsigned int btn1_text=0;//Textura botão 1
    btn1_text = loadTexture("icones/undo.png");//Caminho e nome do arquivo da textura 1
    unsigned int btn2_text=0;//Textura botão 2
    btn2_text = loadTexture("icones/salvar.png");//Caminho e nome do arquivo da textura 2
    unsigned int btn3_text=0;//Textura botão 3
    btn3_text = loadTexture("icones/ler.png");//Caminho e nome do arquivo da textura 3
    unsigned int btn4_text=0;//Textura botão 4
    btn4_text = loadTexture("icones/anterior.png");//Caminho e nome do arquivo da textura 4
    unsigned int btn5_text=0;//Textura botão 5
    btn5_text = loadTexture("icones/proximo.png");//Caminho e nome do arquivo da textura 5
    unsigned int texto_text=0;//Textura botão 5
    bool carregarTexto=false;//Indica quando a imagem do texto deve ser carregada
    bool lendoAtividades = false;//Booleano que indica que o progama está no modo leitura
    int arquivo = -1;// variavel que indica qual arquivo será lido
    int mouseX=0;//Posição do mouse no eixo X
    int mouseY=0;//Posição do mouse no eixo Y
    bool selecionado = false;//Booleano indicando se um ponto foi selecionado ou não;
    int selecao[1][2];//cordenadas do último ponto selecioando
    float matriz[120][2];//matriz contendo as cordenadas de todos os pontos
    float selecoes[120][2];//matriz de todos os pontos já selecionados
    int contSelecoes=0;//Contador de seleções
    bool desenhando = true;//Booleano que indica que o programa esta no modo de desenho
    bool btn1=false,btn2=false,btn3=false,btn4=false,btn5=false;//booleanos indicando quais botões estão ou não ativados
    bool executando = true;//Booleano que indica a execução do programa
    SDL_Event eventos;//eventos detectados
    while(executando){//loop de execução
        while(SDL_PollEvent(&eventos)){
            switch(eventos.type){
                case SDL_QUIT:// testa se o evento foi o clique do mouse para fechar a janela
                    executando = false;
                    break;
                case SDL_MOUSEMOTION :// testa se o evento foi um movimento do mouse
                    mouseX = eventos.motion.x;//le a posição em x do mouse
                    mouseY = eventos.motion.y;//le a posição em y do mouse
                    for(int i = 0;i<120;i++){
                        float dist = calculaDistancia(mouseX,mouseY,matriz[i][0],matriz[i][1]);// calcula a distancia do mouse até todos os pontos da tela
                        if(dist<=4){//testa se adisância calculada é menor que 4
                            selecao[0][0] = matriz[i][0];//salva a posicao do ponto selecionado em x
                            selecao[0][1] = matriz[i][1];//salva a posição do ponto selecionado em y
                            selecionado = true;//indica que um ponto foi selecionado
                            break;
                        }
                        else{
                            selecionado = false;
                        }
                    }
                break;
                case SDL_MOUSEBUTTONDOWN://testa se o botão do mouse foi pressionado
                    if(selecionado && desenhando == true){//verifica se o programa esta no modo de desenhar e se houve um ponto selecionado
                    int dist;
                        if(contSelecoes!=0){//testa para ver se é o primeiro ponto selecionado ou não
                            dist = calculaDistancia(selecoes[contSelecoes-1][0],selecoes[contSelecoes-1][1],mouseX,mouseY);
                        }
                        else{
                            dist=0;
                        }
                        if(dist<60){// teste para ver se o ponto é adjacente ao ponto anterior
                            bool salvar= true;
                            for(int i =0;i<contSelecoes;i++){
                                if(selecao[0][0] == selecoes[i][0] && selecao[0][1] == selecoes[i][1]){// teste para ver se o ponto já foi selecionado
                                    salvar = false;
                                    break;
                                }
                            }
                            if(salvar){//testa a condição da variável salvar
                                selecoes[contSelecoes][0]=selecao[0][0];//Salva as posições x e y
                                selecoes[contSelecoes][1]=selecao[0][1];
                                contSelecoes++;//soma mais um ao contador de seleções
                            }
                        }
                    }
                    if(btn2 == true){// testa se o botão 2 foi pressionado
                        if(contSelecoes>1){// verifica se mais de um ponto foi selecionado
                            if(arquivo==-1){//testa se é o primeiro arquivo a se gravar ou não
                                salvarDados(selecoes,&contSelecoes);
                                desenhando = true;
                            }
                            else{
                                alterarDados(selecoes,&contSelecoes,arquivo);// caso seja um arquivo já escrito anteriormente se edita os valores contidos nele
                                desenhando = false;//desliga o modo de desenho
                            }
                        }

                    }
                    else if(btn1 == true){//Verifica se o botão 1 foi selecionado
                        if(contSelecoes!=0){
                            contSelecoes--;//Diminui em uma unidade a quantidade de selecões podendo assim selecionar outro ponto
                        }
                        desenhando = true;
                    }
                    else if(btn3 ==true){
                        arquivo=0;
                        desenhando = false;
                        lendoAtividades = !lendoAtividades;//ativa e desativa o botão conforme o clique
                        if(lendoAtividades){
                            lerDados(selecoes,&contSelecoes,arquivo);//lê todos os arquivos de atividades já escritos anteriormente
                            carregarTexto = true;
                        }
                        else{
                            contSelecoes=0;//zera o contador
                            arquivo = -1;
                            desenhando = true;//entra no modo de desenho
                        }
                    }
                    else if(btn4 ==true){
                        if(arquivo>0){
                            arquivo--;//retorna ao arquivo anterior permitindo sua leitura
                            carregarTexto = true;
                        }
                        desenhando = false;
                        lendoAtividades = true;
                        lerDados(selecoes,&contSelecoes,arquivo);
                    }
                    else if(btn5 ==true){
                        char tamanho[2];
                        int nmrTamanho;
                        FILE *fp;
                        fp= fopen("atividades.txt","r");
                        fgets(tamanho,5,fp);//Lê a quantidade de arquivos já escritos
                        nmrTamanho = atoi(tamanho);
                        if(arquivo+1<nmrTamanho){
                            arquivo++;//avança para o próximo arquivo permitindo sua leitura
                            carregarTexto = true;
                        }
                        desenhando = false;
                        lendoAtividades = true;
                        lerDados(selecoes,&contSelecoes,arquivo);
                    }
                    break;
                case SDL_KEYUP:
                    if(eventos.key.keysym.sym == SDLK_KP_ENTER){//testa se o enter foi pressionado
                        desenhando = false;//desliga o modo desenho
                    }
                default:
                    ;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        background();
        botoes(mouseX,mouseY,&btn1,&btn2,&btn3,lendoAtividades,&btn4,&btn5,btn1_text,btn2_text,btn3_text,btn4_text,btn5_text);
        desenharLinhas(selecoes,contSelecoes);
        desenharMatriz(matriz);
        destacarSelecao(selecionado,selecao);
        if(lendoAtividades){//verifica se o programa esta no modo de leitura para escrever o titulo das atividades
            if(carregarTexto){
                char arq[18] = {'i','c','o','n','e','s','/','A','0','.','p','n','g'};
                arq[8]= arquivo + '0';//indica qual imagem dever ser utilizada
                texto_text = loadTexture(arq);
                carregarTexto = false;
            }
            texto(texto_text);
        }
        if(contSelecoes != 0 && desenhando == true && mouseX>18 && mouseX<683 && mouseY>55 && mouseY<580){
            glPushMatrix();
                glColor4ub(0,0,0,255);
                glOrtho(0,700,600,0,-1,1);
                glBegin(GL_LINES);
                    glVertex2f(selecoes[contSelecoes-1][0],selecoes[contSelecoes-1][1]);
                    glVertex2f(mouseX,mouseY);
                glEnd();
            glPopMatrix();
        }
        glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        for(int i =0;i<contSelecoes;i++){
            if(i==0){
                glColor4ub(0,220,0,255);
            }
            else if(i==contSelecoes-1){
                glColor4ub(220,0,0,255);
            }
            else{
                glColor4ub(0,0,220,255);
            }
            glBegin(GL_QUADS);
                glVertex2f(selecoes[i][0]-5,selecoes[i][1]-5);
                glVertex2f(selecoes[i][0]+5,selecoes[i][1]-5);
                glVertex2f(selecoes[i][0]+5,selecoes[i][1]+5);
                glVertex2f(selecoes[i][0]-5,selecoes[i][1]+5);
            glEnd();
        }
        glPopMatrix();
        SDL_GL_SwapBuffers();
    }
    glDisable(GL_BLEND);
    SDL_Quit();
    return 0;
}

GLuint loadTexture(const std::string &fileName){
    SDL_Surface *imagem = IMG_Load(fileName.c_str());
    SDL_DisplayFormatAlpha(imagem);
    unsigned objetct(0);
    glGenTextures(1, &objetct);
    glBindTexture(GL_TEXTURE_2D, objetct);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, imagem->w,imagem->h,0,GL_RGBA,GL_UNSIGNED_BYTE,imagem->pixels);

    SDL_FreeSurface(imagem);

    return objetct;
}

void botoes(int mouseX, int mouseY, bool *btn1, bool *btn2, bool *btn3,bool lendoAtividades,bool *btn4,bool *btn5,unsigned int btn1_text,unsigned int btn2_text,unsigned int btn3_text,unsigned int btn4_text, unsigned int btn5_text){
    glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        if(mouseX>10 && mouseX <50 && mouseY>5 && mouseY<45){//testa se o mouse esta sobre o botao
            glColor4ub(200,200,200,255);//altera a cor caso o mouse esteja sobre o botão
            *btn1 = true;
        }
        else{
            glColor4ub(150,150,150,255);//cor padrao
            *btn1 = false;
        }
        glBegin(GL_QUADS);//define a forma geométrica e seus vértices
            glVertex2f(10,10);
            glVertex2f(50,10);
            glVertex2f(50,45);
            glVertex2f(10,45);
        glEnd();
        glColor4ub(150,150,150,255);
        glEnable(GL_TEXTURE_2D);//habilida a textura para o botão
        glBindTexture(GL_TEXTURE_2D, btn1_text);
        glBegin(GL_QUADS);//desenha o botao a partir dos quatro vértices dados e sua textura
            glTexCoord2d(0,0);      glVertex2f(10,10);
            glTexCoord2d(1,0);      glVertex2f(50,10);
            glTexCoord2d(1,1);      glVertex2f(50,45);
            glTexCoord2d(0,1);      glVertex2f(10,45);
        glEnd();
        glDisable(GL_TEXTURE_2D);//desabilita a textura do botão
        if(mouseX>60 && mouseX <100 && mouseY>5 && mouseY<45){
            glColor4ub(200,200,200,255);
            *btn2 = true;
        }
        else{
            glColor4ub(150,150,150,255);
            *btn2 = false;
        }
        glBegin(GL_QUADS);
            glVertex2f(60,10);
            glVertex2f(100,10);
            glVertex2f(100,45);
            glVertex2f(60,45);
        glEnd();
        glColor4ub(200,200,200,255);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, btn2_text);
        glBegin(GL_QUADS);
            glTexCoord2d(0,0);      glVertex2f(60,10);
            glTexCoord2d(1,0);      glVertex2f(100,10);
            glTexCoord2d(1,1);      glVertex2f(100,45);
            glTexCoord2d(0,1);      glVertex2f(60,45);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        if(mouseX>110 && mouseX <150 && mouseY>5 && mouseY<45){
            glColor4ub(200,200,200,255);
            *btn3 = true;
        }
        else{
            if(lendoAtividades == true){//testa se o programa está no estado "lendoAtividades", o que manterá o botão pressionado
                glColor4ub(185,185,185,255);
            }
            else{
                glColor4ub(150,150,150,255);
            }
            *btn3 = false;
        }
        glBegin(GL_QUADS);
            glVertex2f(110,10);
            glVertex2f(150,10);
            glVertex2f(150,45);
            glVertex2f(110,45);
        glEnd();
        glColor4ub(200,200,200,255);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, btn3_text);
        glBegin(GL_QUADS);
            glTexCoord2d(0,0);      glVertex2f(110,10);
            glTexCoord2d(1,0);      glVertex2f(150,10);
            glTexCoord2d(1,1);      glVertex2f(150,45);
            glTexCoord2d(0,1);      glVertex2f(110,45);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        if(lendoAtividades == true){//testa se o programa está no estado "lendoAtividades", o que adiciona dois novos botões
            if(mouseX>180 && mouseX <220 && mouseY>5 && mouseY<45){
            glColor4ub(200,200,200,255);
            *btn4 = true;
            }
            else{
                glColor4ub(150,150,150,255);
                *btn4 = false;
            }
            glBegin(GL_QUADS);
                glVertex2f(180,10);
                glVertex2f(220,10);
                glVertex2f(220,45);
                glVertex2f(180,45);
            glEnd();
            glColor4ub(200,200,200,255);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, btn4_text);
            glBegin(GL_QUADS);
                glTexCoord2d(0,0);      glVertex2f(180,10);
                glTexCoord2d(1,0);      glVertex2f(220,10);
                glTexCoord2d(1,1);      glVertex2f(220,45);
                glTexCoord2d(0,1);      glVertex2f(180,45);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            if(mouseX>620 && mouseX <660 && mouseY>5 && mouseY<45){
            glColor4ub(200,200,200,255);
            *btn5 = true;
            }
            else{
                glColor4ub(150,150,150,255);
                *btn5 = false;
            }
            glBegin(GL_QUADS);
                glVertex2f(620,10);
                glVertex2f(660,10);
                glVertex2f(660,45);
                glVertex2f(620,45);
            glEnd();
            glColor4ub(200,200,200,255);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, btn5_text);
            glBegin(GL_QUADS);
                glTexCoord2d(0,0);      glVertex2f(620,10);
                glTexCoord2d(1,0);      glVertex2f(660,10);
                glTexCoord2d(1,1);      glVertex2f(660,45);
                glTexCoord2d(0,1);      glVertex2f(620,45);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    glPopMatrix();
}

void background(){
    glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        glColor4ub(190,190,190,255);
        glBegin(GL_QUADS);
            glVertex2d(0,0);
            glVertex2d(700,0);
            glVertex2d(700,600);
            glVertex2d(0,600);
        glEnd();
        glColor4ub(0,0,0,235);
        glBegin(GL_QUADS);
            glVertex2d(18,58);
            glVertex2d(682,58);
            glVertex2d(682,582);
            glVertex2d(18,582);
        glEnd();
        glBegin(GL_QUADS);
            glVertex2d(5,5);
            glVertex2d(695,5);
            glVertex2d(695,50);
            glVertex2d(5,50);
        glEnd();
        glColor4ub(230,230,230,235);
        glBegin(GL_QUADS);
            glVertex2d(7,7);
            glVertex2d(693,7);
            glVertex2d(693,48);
            glVertex2d(7,48);
        glEnd();
        glColor4ub(255,255,255,235);
        glBegin(GL_QUADS);
            glVertex2d(20,60);
            glVertex2d(680,60);
            glVertex2d(680,580);
            glVertex2d(20,580);
        glEnd();
    glPopMatrix();
}


void salvarDados(float selecoes[][2],int *contSelecoes){
    FILE *fp;
    FILE *fp2;
    char numeroAtividade[2];
    fp2 = fopen("atividades.txt","r");
    if(fp2==NULL){// lê se há ativiades já feitas através da existencia do arquivo atividades para assim definir o número da próxima atividade
        numeroAtividade[0]= '0';
        fclose(fp2);
    }
    else{
        fgets(numeroAtividade,2,fp2);
        fclose(fp2);
    }
    if(numeroAtividade[0]-'0'<=9){// limita o número de atividades a 10
        int cont = 0;
        char arquivo[10] = {'a','r','q'};
        char extensao[5]= {'.','t','x','t'};
        while(cont!=4){
            arquivo[3]= numeroAtividade[0];//define qual será o numero da atividade a ser salva
            arquivo[cont+4] = extensao[cont];
            cont++;
        }
        fp = fopen(arquivo,"a");
        fp2 = fopen("atividades.txt","w");
        fprintf(fp2,"%d",(numeroAtividade[0]-'0')+1);
        fclose(fp2);
        float x = 0;
        float y = 0;
        fprintf(fp,"%.0f\n%.0f\n%d\n",selecoes[0][0],selecoes[0][1],*contSelecoes);// cria as três primeiras linhas do arquivo
        for(int i =0; i<*contSelecoes-1; i++){//salva os valores restantes da atividade
            x =(selecoes[i+1][0]- selecoes[i][0])/55;
            y =(selecoes[i+1][1]- selecoes[i][1])/55;
            fprintf(fp,"%.0f\n",x);
            fprintf(fp,"%.0f\n",y);
        }
        fclose(fp);
        *contSelecoes = 0;//reseta a área de desenho
    }
}

void lerDados(float selecoes[][2], int *conSelecoes, int arquivo){
    char comando[20];
    char nome[10] = {'a','r','q','0','.','t','x','t'};
    nome[3]= arquivo+'0';//define qual atividade será lida
    int i = 0,cont=0;
    FILE *fp;
    int valor;
    fp = fopen(nome,"r");
    while(fgets(comando,20,fp)!=NULL){// lê o arquivo até a última linha
        if(i<3){//define a função dos três primeiros números lidos, posição inicial em x e y e a quantidade de pontos
			valor = atoi(comando);// converte a string para inteiro
			if(i==0){
                selecoes[0][0]=valor;
			}
			else if(i==1){
				selecoes[0][1]=valor;
				cont++;
			}
			else if(i==2){
				*conSelecoes = valor;
			}
		}
		else{// alteração dos valores da matriz de pontos selecionados
			valor = atoi(comando);
			if(i%2!=0){
                selecoes[cont][0]= selecoes[cont-1][0]+valor*55;
			}
			else{
                selecoes[cont][1]= selecoes[cont-1][1]+valor*55;
                cont++;
			}
		}
		i++;
    }
    fclose(fp);
}

void alterarDados(float selecoes[][2], int *contSelecoes,int arquivo){
    char nome[10] = {'a','r','q','0','.','t','x','t'};
    nome[3] = arquivo+'0';//indica qual arquivo deverá ser alterado
    FILE *fp;
    fp = fopen(nome,"w");
    fprintf(fp,"%.0f\n%.0f\n%d\n",selecoes[0][0],selecoes[0][1],*contSelecoes);
    for(int i =0; i<*contSelecoes-1; i++){//imprime as alterações feitas no arquivo
        fprintf(fp,"%.0f\n",(selecoes[i+1][0]- selecoes[i][0])/55);
        fprintf(fp,"%.0f\n",(selecoes[i+1][1]- selecoes[i][1])/55);
    }
    fclose(fp);
}

void desenharLinhas(float selecoes[][2],int contSelecoes){
    glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        glColor4ub(0,0,0,255);
        for(int i = 0; i<contSelecoes-1;i++){//desenha todas as linhas entre os pontos selecionados
            glBegin(GL_LINES);
                glVertex2f(selecoes[i][0],selecoes[i][1]);
                glVertex2f(selecoes[i+1][0],selecoes[i+1][1]);
            glEnd();
        }
    glPopMatrix();
}

void desenharMatriz(float matriz[][2]){
    glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        glColor4ub(0,0,0,255);
        int cont=0;
        for(int i = 0; i<12;i++){//desenha todos os pontos da matriz
            for(int j=0;j<10;j++){
                glBegin(GL_QUADS);
                    glVertex2f(40+(i*55),70+(j*55));
                    glVertex2f(48+(i*55),70+(j*55));
                    glVertex2f(48+(i*55),78+(j*55));
                    glVertex2f(40+(i*55),78+(j*55));
                    matriz[cont][0]=((48+(i*55))+(40+(i*55)))/2.0;
                    matriz[cont][1]=((70+(j*55))+(78+(j*55)))/2.0;
                    cont++;
                glEnd();
            }
        }
    glPopMatrix();
}

void definicoesSdlOpenGl(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //nome da janela aberta
    SDL_WM_SetCaption("Programa de acompanhamento",NULL);
    //tamanho da janela
    SDL_SetVideoMode(700,600,32,SDL_OPENGL);
    //cor inicial
    glClearColor(1,1,1,1);
    //area exibida
    glViewport(0,0,700,600);
    //sombra
    glShadeModel(GL_SMOOTH);
    //2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// desenhos geométricos
    //3D
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void destacarSelecao(bool selecionado,int selecao[][2]){
    if(selecionado){//caso um botão for selecionado sua cor será destacada
        glPushMatrix();
            glOrtho(0,700,600,0,-1,1);
            glColor4ub(100,100,100,255);
                glBegin(GL_QUADS);
                    glVertex2f(selecao[0][0]-5,selecao[0][1]-5);
                    glVertex2f(selecao[0][0]+5,selecao[0][1]-5);
                    glVertex2f(selecao[0][0]+5,selecao[0][1]+5);
                    glVertex2f(selecao[0][0]-5,selecao[0][1]+5);
                glEnd();
        glPopMatrix();
    }
}

void texto(unsigned int texto_text){
    glPushMatrix();
        glOrtho(0,700,600,0,-1,1);
        glColor4ub(200,200,200,255);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texto_text);
            glBegin(GL_QUADS);
                glTexCoord2d(0,0);      glVertex2f(305,10);
                glTexCoord2d(1,0);      glVertex2f(535,10);
                glTexCoord2d(1,1);      glVertex2f(535,45);
                glTexCoord2d(0,1);      glVertex2f(305,45);
            glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
}
