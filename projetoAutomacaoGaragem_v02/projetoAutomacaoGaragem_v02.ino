#include <Servo.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>

Servo motor;
const int botaoMotor = 9;
int estadoBotaoMotor = 0;
const int led = 4;
const int botaoLed = 7;
int estadoBotaoLed = 0;
int i;


// Update these with values suitable for your network.
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x55};

// Callback function header
void callback(char *topic, byte *payload, unsigned int length);

EthernetClient ethClient;

// Dados do MQTT Cloud
PubSubClient client("m10.cloudmqtt.com", 14882, callback, ethClient);

// Funcçao que irá receber o retorno do servidor.
void callback(char *topic, byte *payload, unsigned int length)
{
  char c = payload;
  Serial.println(c);
  Serial.println(topic);

  if (topic == "portao") {

  }

  byte *p = (byte *)malloc(length);
  memcpy(p, payload, length);
  free(p);
}







void setup()
{
  motor.attach(8);
  pinMode(botaoMotor, INPUT);
  Serial.begin(9600);
  motor.write(180);

  pinMode(led, OUTPUT);
  pinMode(botaoLed, INPUT);

  Serial.begin(9600);
  Serial.println("Iniciando...");
  Ethernet.begin(mac);

  // Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente
  if (client.connect("charles", "charles", "charles"))
  {
    // Envia uma mensagem para o cloud no topic portao
    client.publish("portao", 1);

    // Conecta no topic para receber mensagens
    client.subscribe("portao");
    
    Serial.println("conectado portao");
  } else {
    Serial.println("erro ao conectar portao");
  }

}



void loop ()
{

  client.loop();

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
