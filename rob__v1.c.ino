/* =====Projeto de laboratório de introdução a ciência de computação
 * 
 * Primeira versão do código do brinquedo educacional
 * Funções implementada:
 * -Ler portas analógicas;
 * -Filtros para redução dos ruídos da leitura;
 * -Leitura dos botões;
 * -PonteH para funcionamento dos motores;
 * -Imprime os dados para debug;
 * Funções futuras:
 * -Leitura de arquivos em um cartão sd para interação com computador;
 * -Implementação de um buzzer passivo para efeitos sonoros
 * -Criar funções com novos blocos
 * 
 */

#include <SD.h>//biblioteca que permite o uso do módulo de cartão SD
#include <SPI.h>//bIblioteca que permite a comunicação SPI

File arqv;
  
int nAtiv; //numero de atividades
int ativAtual; //qual arquivo de atividade deverá ser aberto

//define se ira utilizar a funçao debug, comentar para desativar
#define DEBUG

//tamanho do filtro de média móvel
#define m 5

// vetores para construção do filtro
float vetA[m]; //
float vetB[m];
float vetC[m];
float vetD[m];
float vetE[m];
float vetF[m];

int comandos[100];// vetor que salva os comandos

// variaveis auxiliares
int comando;
int contComando = 4;
String result[6];

// contador de loops
int cont = 0;

//testa para ver se hove mais de m loops, funciona como bool
int cont2 = 0;

//valores lidos em cada porta analógica
int a,b,c,d,e,f;

// valores lidos após o filtro 
float Fa,Fb,Fc,Fd,Fe,Ff;

//Vetores que funcionarão como armazenadores de comandos para se criar funções
int funcao1[6];
int funcao2[6];

//contador de funções
int contFuncoes = 0 ;

void setup() {
//configura a resistência de cada função com valores iniciais
  funcao1[0]=-1;
  funcao2[0]=-1;
//inicia a comunicação serial
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
//declara a funcionalidade de cada pino, entrada ou saida
  pinMode(A0,INPUT);//bloco 1
  pinMode(A1,INPUT);//bloco 2
  pinMode(A2,INPUT);//bloco 3
  pinMode(A3,INPUT);//bloco 4
  pinMode(A4,INPUT);//bloco 5
  pinMode(A5,INPUT);//potenciometro para repetição
  pinMode(2,INPUT);//botao de salvar
  pinMode(4,INPUT);//botao para criar função
  pinMode(3,INPUT);//botao para executar comandos
  pinMode(5,OUTPUT);//motor 1, sentido horário
  pinMode(6,OUTPUT);//motor 1, sentido antihorário
  pinMode(7,OUTPUT);//motor 2, sentido horário
  pinMode(8,OUTPUT);//motor 2, sentido antihorário
  //inicialização do módulo SD
  if (!SD.begin(9)){
    #ifdef DEBUG
      Serial.println("inicialização do cartão falhou");
      while(1);
    #endif
  }
  //abrindo o arquivp
  arqv=SD.open("Atividades.txt",FILE_WRITE);
  nAtiv=atoi(arqv.read());
  ativAtual=0;
  arqv.close();
}

//filtra os dados tirando uma média a cada m dados
void filtro(){
   Fa += a/(1.0*m);
   Fb += b/(1.0*m);
   Fc += c/(1.0*m);
   Fd += d/(1.0*m);
   Fe += e/(1.0*m);
   Ff += f/(1.0*m);
   if(cont2){
     Fa += -vetA[cont%m];
     Fb += -vetB[cont%m];
     Fc += -vetC[cont%m];
     Fd += -vetD[cont%m];
     Fe += -vetE[cont%m];
     Ff += -vetF[cont%m];
   }
   vetA[cont%m] =  a/(1.0*m);
   vetB[cont%m] =  b/(1.0*m);
   vetC[cont%m] =  c/(1.0*m);
   vetD[cont%m] =  d/(1.0*m);
   vetE[cont%m] =  e/(1.0*m);
   vetF[cont%m] =  f/(1.0*m);
}
#ifdef DEBUG
  void imprime(){
    // imprime os valores finais
    Serial.print((int)Fa/10);
    Serial.print(' ');
    Serial.print((int)Fb/10);
    Serial.print(' ');
    Serial.print((int)Fc/10);
    Serial.print(' ');
    Serial.print((int)Fd/10);
    Serial.print(' ');
    Serial.print((int)Ff/10);
    Serial.print(' ');
    Serial.println((int)(map(Fe,0,1024,1,11)));// a função map transforma os valores de o a 1024 em valores de 1 a 11
  }
#endif

//salva todos os dados lidos após ter pressionado o botão
void salvarDados(){
  for(int i = 0; i<(int)(map(Fe,0,1024,1,11));i++){
    result[0]=analise(Fa);
    comandos[contComando] = comando;
    contComando++;
    result[1]=analise(Fb);
    comandos[contComando] = comando;
    contComando++;
    result[2]=analise(Fc);
    comandos[contComando] = comando;
    contComando++;
    result[3]=analise(Fd);
    comandos[contComando] = comando;
    contComando++;
    result[4]=analise(Ff);
    comandos[contComando] = comando;
    contComando++;
  }
}

//Da sentido a cada valor lido, transformando-os em instruções para o robô. *ainda serão adicionados novos comandos
String analise(float x){
  x = (int)x/10;
  if(x>= 70 && x<=80){
    comando = 1;
    return "Frente";
  }
  if(x>= 30 && x<=40){
    comando = 2;
    return "Atras";
  }
  if(x>= 7 && x<=11){
    comando = 3;
    return "Direita";
  }
  if(x>= 40 && x<=50){
    comando = 4;
    return "Esquerda";
  }
  if(x<=6){
    comando = 5;
    return "Espere";
  }
  // salva todos os comandos contidos em cada função e seu número de repetições
  if(x>=funcao1[0]-5 && x<=funcao1[0]+5){
    for(int j=0;j<funcao1[1];j++){
      for(int i = 2;i<6;i++){
        comandos[contComando] = funcao1[i];
        contComando++;
        if(j==funcao1[1]-1 && i==5){
          comando = funcao1[i];
          return "Função 1";
        }
      }
    }
  }
  if(x>=funcao2[0]-5 && x<=funcao2[0]+5){
    for(int j=0;j<funcao2[1];j++){
      for(int i = 2;i<6;i++){
        comandos[contComando] = funcao2[i];
        contComando++;
        if(j==funcao2[1]-1 && i==5){
          comando = funcao2[i];
          return "Função 2";
        }
      }
    }
  }
}

// cria uma função nova para um bloco a partir da combinação de outros quatro comandos
void criarFuncao(){
  int funcTemp[6];
    result[0]=analise(Fe);//define o número de repetições que ocorrerá
    funcTemp[1]=comando;
    result[0]=analise(Fa);
    funcTemp[2]=comando;
    result[1]=analise(Fb);
    funcTemp[3]=comando;
    result[2]=analise(Fc);
    funcTemp[4]=comando;
    result[3]=analise(Fd);
    funcTemp[5]=comando;
    result[4]=analise(Ff);
    funcTemp[0]=Ff;//define a resistencia do bloco da função
  if(contFuncoes==0){
    for(int i=0;i<6;i++){
      funcao1[i]=funcTemp[i];
    }
    contFuncoes++;
  }
  else{
     for(int i=0;i<6;i++){
      funcao2[i]=funcTemp[i];
    }
    contFuncoes=0;
  }
}

// função principal que chama todas as outras
void loop() {
// lê o valor em cada porta
  a = analogRead(A0);
  b = analogRead(A1);
  c = analogRead(A3);
  d = analogRead(A4);
  f = analogRead(A2);
  e = analogRead(A5);
  filtro();
  //zera o contador a cada m dados
  if(cont>m){
    #ifdef DEBUG
      imprime();
    #endif
    cont=0;
    cont2 =1;
  }
  //le o estado da porta onde esta conectado o botão de salvar os dados
  if(digitalRead(2)==HIGH && digitalRead(4)==LOW ){
    salvarDados();
#ifdef DEBUG
    for(int i = 0; i<(int)(map(Fe,0,1024,1,11));i++){
        delay(500);
        Serial.println(result[0]);
        delay(1000);
        Serial.println(result[1]);
        delay(1000);
        Serial.println(result[2]);
        delay(1000);
        Serial.println(result[3]);
        delay(500);
    }
#endif 
 }
 else if(digitalRead(2)==HIGH && digitalRead(4)==HIGH){
  criarFuncao();
 }
  //Le o estado da porta onde esta conectado o botão de executar os comandos salvos
 if(digitalRead(3)){
    if(executar()==1){
      acerto();
    }
    else{
      erro();
    }
 }
 delay(10);
 cont++;
}

//Comandos já implementados
void andar(){
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
}

void pararAndar(){
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
}

void andarAtras(){
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}

void virarDireita(){
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}

void virarEsquerda(){
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
}
// comando executado caso encontre um problema com o percurso programa e o projetado
void erro(){
  delay(300);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  delay(320);
  pararAndar();
  delay(500);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  delay(500);
  pararAndar();
  delay(500);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  delay(250);
  pararAndar();
}
// comando executado caso o brinquedo chegue até o final da trajetória sem problemas
void acerto(){
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  delay(2200);
  pararAndar();
}
//Função que chama cada comando lendo os valores salvos, dando um pequeno delay entre cada
int executar(){
  String nome;
  nome = "";
  nome += "arq";
  nome += ativAtual;
  nome += ".txt";
  arqv=SD.open(nome);
  arqv.read();
  arqv.read(); //pular as duas primeiras linhas do arquivo
  int nComandos=atoi(arqv.read()); //numero de comands a serem lidos no arquivo
  int X=0,Y=0; //coordenadas calculadas a partir dos dados do arquivo
  int u=0,v=0; //coordenadas calculadas a partir do comando da crianca
  int dir=0; //Inteiro numero indica a direcao que o brinquedo aponta (0-frente, 2-esquerda, 3-tras,4-direita) */
  delay(1000);
  for(int i = 0; i< contComando ; i++){
    //Serial.println(comandos[i]);
    switch(comandos[i]){//executa os comandos e calcula a posição final do brinquedo
      case 1:
        andar();
        if(dir==0) v++;
        else if(dir==1)u--;
        else if(dir==2)v--;
        else if(dir==3)u++;
        delay(1000);
      break;
      case 2:
        pararAndar();
        andarAtras();
        delay(1000);
      break;
      case 3:
        pararAndar();
        virarDireita();
        delay(400);
        if(dir==0) v--;
        else if(dir==1) u++;
        else if(dir==2) v++;
        else if(dir==3) u--;
      break;
      case 4:
        pararAndar();
        virarEsquerda();
        if(dir>0)dir--;
        else if(dir==0) dir=3;
        delay(400);
      break;
      case 5:
        pararAndar();
        delay(300);
        if(dir<3)dir++;
        else if(dir==3) dir=0;
      break;
    }
  }
  pararAndar();
  for(int i=0;i<nComandos-1;i++){// calcula a posiçao final dada pela atividade
    X+=atoi(arqv.read());
    Y+=atoi(arqv.read());
  }
  arqv.close();
  if(X==u && Y==v){//compara os resultados obtidos com os esperados
    if(ativAtual<nAtiv){
      ativAtual++;
    }
    return 1;
  }
  else{
    return 0;
  }
}
