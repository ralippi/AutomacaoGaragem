//Declaração das constantes
const int led = 4; //constante led refere-se ao pino digital 8.
const int botao = 7; //constante botão refere-se ao pino digital 2.

//Variável que conterá os estados do botão (0 LOW, 1 HIGH).
int estadoBotao = 0;

//Método setup, executado uma vez ao ligar o Arduino.
void setup() {
pinMode(led,OUTPUT); //Definindo pino digital 8 como de saída.
pinMode(botao,INPUT); //Definindo pino digital 2 como de entrada.
}

//Método loop, executado enquanto o Arduino estiver ligado.
void loop() {
//Lendo o estado do pino 2, constante botao, e atribuindo
//o resultado a variável estadoBotao.
estadoBotao = digitalRead(botao);

//Verificando o estado do botão para definir se acenderá ou
//apagará o led.
if (estadoBotao == HIGH) {
digitalWrite(led, !digitalRead(led));
delay(500);
} else {

}
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
