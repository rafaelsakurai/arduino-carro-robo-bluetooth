/*
  Programa para controlar o carro robo via bluetooth.
  O carro pode ser comprado em:
  http://dx.com/p/arduino-compatible-bluetooth-controlled-robot-car-kits-146418
*/
#include <Servo.h>

//#define trigPin 13
#define echoPin 12
#define servoPin 9

int ENA=5;
int IN1=2; // Gira as rodas da direita para traz
int IN2=3; // Gira as rodas da direita para frente
int ENB=6;
int IN3=4; // Gira as rodas da esquerda para traz
int IN4=7; // Gira as rodas da esquerda para frente

int direcao = 1; //Direcao que o carrinho deve seguir
int tempo = 100; //Tempo de delay 

/*
 Servo motor que fica na frente do carrinho e controla a direcao do ultrasonico,
 nesse programa servira apenas para mexer a parte da frente ao ligar o carrinho.
*/
Servo cabeca;

// Inicializa os componentes do carrinho
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  cabeca.attach(servoPin);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  cabeca.write(90);
  
  Serial.begin(9600);
}

// Movimenta o carrinho para frente
void paraFrente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(tempo);
}

// Movimenta o carrinho para traz
void paraTraz() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(tempo);
}

// Movimenta o carrinho para esquerda
void paraEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(tempo);
}

// Movimenta o carrinho para direita
void paraDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(tempo);
}

// Para o carrinho
void parar() {
  Serial.println("parar()");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/*
  Obtem a direcao recebida pelo bluetooth, se nenhum comando for recebido
  mantem a direcao atual.
*/ 
int direcaoBluetooth() {
  char acao = ' ';
  
  //Verifica se o serial esta disponivel.
  if(Serial.available()) {
    //Le o comando recebido pelo bluetooth
    acao = Serial.read();
  }
  
  if (acao == 'H') { //Iniciar
    direcao = 1;
  } else if (acao == 'L') { //Desligar, parar o carro
    direcao = 1;
  } else if(acao == 'P') { //Parar
    direcao = 1;
  } else if(acao == 'F') { //Frente
    direcao = 2;
  } else if (acao == 'T') { //Traz
    direcao = 3;
  } else if (acao == 'D') { //Direita
    direcao = 4;
  } else if (acao == 'E') { //Esquerda
    direcao = 5;
  }
  
  return direcao;
}

// Executa a direcao recebida pelo bluetooth
void executarDirecao(int acao) {
  if(acao == 0) {
    //Faz uma graca inicial, girando o carro para a direita e depois para a esquerda
    cabeca.write(0);
    paraDireita();paraDireita();paraDireita();paraDireita();paraDireita();paraDireita();
    delay(500);
    cabeca.write(180);
    paraEsquerda();paraEsquerda();paraEsquerda();paraEsquerda();paraEsquerda();paraEsquerda();
    delay(500);
    cabeca.write(90);
    direcao = 1;
  } else if (acao == 1) {
    parar();
  } else if (acao == 2) {
    paraFrente();
  } else if (acao == 3) {
    paraTraz();
  } else if (acao == 4) {
    paraDireita();
  } else if (acao == 5) {
    paraEsquerda();
  }
}

void loop() {
  int direcao = 1;
  direcao = direcaoBluetooth();
  executarDirecao(direcao);
  delay(tempo);
}
