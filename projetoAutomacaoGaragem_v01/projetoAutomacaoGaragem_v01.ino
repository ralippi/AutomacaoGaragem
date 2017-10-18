#include <Servo.h>

Servo motor;
const int botaoMotor = 9;
int estadoBotaoMotor = 0;
const int led = 4;
const int botaoLed = 7;
int estadoBotaoLed = 0;

int i;


void setup()
{
  motor.attach(8);
  pinMode(botaoMotor, INPUT);
  Serial.begin(9600);
  motor.write(180);

  pinMode(led, OUTPUT);
  pinMode(botaoLed, INPUT);
}

void loop ()
{

  estadoBotaoMotor = digitalRead(botaoMotor);
  estadoBotaoLed = digitalRead(botaoLed);

  //Teste da posicao do servo motor
  Serial.print(motor.read());
  Serial.print("\n");

  if (estadoBotaoLed == HIGH) {
    digitalWrite(led, !digitalRead(led));
    delay(500);
  }

  if (estadoBotaoMotor == HIGH) {

    if (motor.read() >= 90 && motor.read() <= 100) {
      //Fechar Portao
      //Luz Apaga
      Serial.print(motor.read());
      Serial.print("\n");
      for (i = 90; i <= 180; i++)
      {
        motor.write(i);
        delay(10);
      }
    } else {
      //Abrir portao
      //Luz Acence
      Serial.print(motor.read());
      Serial.print("\n");

      for (i = 180; i >= 90; i--)
      {
        motor.write(i);
        delay(10);
      }
      digitalWrite(led, HIGH);
    }
  }
}

//////////////////////////////////////////////

//Função ABRIR Portao
void abrirPortao ()
{
  Serial.print(motor.read());
  Serial.print("\n");

  for (i = 180; i >= 90; i--)
  {
    motor.write(i);
    delay(10);
  }
}

//Função FECHAR Portao
void fecharPortao ()
{
  Serial.print(motor.read());
  Serial.print("\n");

  for (i = 90; i >= 180; i--)
  {
    motor.write(i);
    delay(10);
  }
}

//Função ACENDER Luz
void acenderLed ()
{
  digitalWrite(led, HIGH);
  delay(500);
}

//Função APAGAR Luz
void apagarLed ()
{
  digitalWrite(led, LOW);
  delay(500);
}

//////////////////////////////////////////////
