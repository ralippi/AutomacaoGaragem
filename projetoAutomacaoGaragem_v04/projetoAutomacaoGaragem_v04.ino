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
// Atualizar ultimo valor para ID do seu Kit para evitar duplicatas
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x55 };
// Endereço do Cloud MQTT
char* server = "m10.cloudmqtt.com";
// Valor da porta do servidor MQTT
int port = 14882;

EthernetClient ethClient;


// FUNÇÃO que irá receber o retorno do servidor.
void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  char* payloadAsChar = payload;

  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  Serial.print("Topic received: "); Serial.println(topic);
  //  Serial.print("Message: "); Serial.println(msg);

  // Dentro do callback da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  int msgComoNumero = msg.toInt();

  Serial.print("Numero lido: "); Serial.println(msgComoNumero);
  Serial.flush();

  switch (msgComoNumero) {
    //Entrar-Abrir na Garagem
    case 1:
      abrirPortao();
      acenderLed();
      break;
    //Entrar-Fechar na Garagem
    case 2:
      fecharPortao();
      break;
    //Sair-Abrir da Garagem
    case 3:
      abrirPortao();
      break;
    //Sair-Fechar da Garagem
    case 4:
      fecharPortao();
      apagarLed();
      break;
    case 5:
      ligarDesligarLed();
      break;
    default:
      Serial.println("Fail!");
      break;
  }
}


// Dados do MQTT Cloud
PubSubClient client(server, port, whenMessageReceived, ethClient);


void setup() {

  motor.attach(8);
  pinMode(botaoMotor, INPUT);
  Serial.begin(9600);
  motor.write(180);

  pinMode(led, OUTPUT);
  pinMode(botaoLed, INPUT);

  Serial.begin(9600);
  Serial.println("Connecting...");
  while (!Serial) {}
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
  } else {
    Serial.println(Ethernet.localIP());
  }

  // Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente
  if (client.connect("celular", "iot", "iot")) {
    Serial.println("Connected");
    // Envia uma mensagem para o cloud no topic portao
    client.publish("portao", "hello world");
    Serial.println("portao sent");
    // Conecta no topic para receber mensagens
    client.subscribe("portao");
    Serial.println("conectado portao");
  } else {
    Serial.println("Failed to connect to MQTT server");
  }
}


void loop() {
  // A biblioteca PubSubClient precisa que este método seja chamado em cada iteração de `loop()`
  // para manter a conexão MQTT e processar mensagens recebidas (via a função callback)
  client.loop();

  estadoBotaoMotor = digitalRead(botaoMotor);
  estadoBotaoLed = digitalRead(botaoLed);

  //Teste da posicao do servo motor
  //Serial.print(motor.read());
  //Serial.print("\n");

  if (estadoBotaoLed == HIGH) {
    digitalWrite(led, !digitalRead(led));
    delay(500);
  }

  if (estadoBotaoMotor == HIGH) {
    if (motor.read() >= 90 && motor.read() <= 100) {
      //Fechar Portao
      fecharPortao();
    } else {
      //Abrir portao/Luz Acence
      abrirPortao();
      acenderLed();
    }
  }
}

//////////////////////////////////////////////

//Função ABRIR Portao
void abrirPortao () {
  if (motor.read() == 180) {
    for (i = 180; i >= 90; i--) {
      motor.write(i);
      delay(10);
    }
  }
  // client.publish("portao", "Portão Aberto");
}

//Função FECHAR Portao
void fecharPortao () {
  if (motor.read() == 90) {
    for (i = 90; i <= 180; i++) {
      motor.write(i);
      delay(10);
    }
  }
  //  client.publish("portao", "Portão Fechado");
}

//Função ACENDER Luz
void acenderLed () {
  digitalWrite(led, HIGH);
  delay(500);
//  client.publish("portao", 1);
}

//Função APAGAR Luz
void apagarLed () {
  digitalWrite(led, LOW);
  delay(500);
  // client.publish("portao", "Led Apagado");
}

void  ligarDesligarLed() {
  digitalWrite(led, !digitalRead(led));
  delay(500);
  //  if (digitalRead(led) == HIGH) {
  //    client.publish("portao", "Led Aceso");
  //  } else {
  //    client.publish("portao", "Led Apagado");
  //  }
}

//////////////////////////////////////////////
