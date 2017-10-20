#include <Servo.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>


const int botaoMotor = 9;
int estadoBotaoMotor = 0;
const int led = 4;
const int ledVermelho = 5;
const int ledVerde = 6;
const int ledAmarelo = 3;
const int botaoLed = 7;
int estadoBotaoLed = 0;
int i;
// Atualizar ultimo valor para ID do seu Kit para evitar duplicatas
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x55 };
// Endereço do Cloud MQTT
char* server = "m10.cloudmqtt.com";
// Valor da porta do servidor MQTT
int port = 14882;
Servo motor;
EthernetClient ethClient;


// FUNÇÃO que irá receber o retorno do servidor.
void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  char* payloadAsChar = payload;

  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  Serial.print("Topic received: "); Serial.println(topic);

  // Dentro do whenMessageReceived (callback) da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  int msgComoNumero = msg.toInt();

  Serial.print("Numero lido: "); Serial.println(msgComoNumero);
  Serial.flush();

  switch (msgComoNumero) {
    //Entrar-Abrir na Garagem
    case 1:
      digitalWrite(ledAmarelo, HIGH);
      abrirPortao();
      acenderLed();
      digitalWrite(ledAmarelo, LOW);
      break;
    //Entrar-Fechar na Garagem
    case 2:
      digitalWrite(ledAmarelo, HIGH);
      fecharPortao();
      digitalWrite(ledAmarelo, LOW);
      break;
    //Sair-Abrir da Garagem
    case 3:
      digitalWrite(ledAmarelo, HIGH);
      abrirPortao();
      digitalWrite(ledAmarelo, LOW);
      break;
    //Sair-Fechar da Garagem
    case 4:
      digitalWrite(ledAmarelo, HIGH);
      fecharPortao();
      apagarLed();
      digitalWrite(ledAmarelo, LOW);
      break;
    case 5:
      digitalWrite(ledAmarelo, HIGH);
      acenderLed();
      digitalWrite(ledAmarelo, LOW);
      break;
    case 6:
      digitalWrite(ledAmarelo, HIGH);
      apagarLed();
      digitalWrite(ledAmarelo, LOW);
      break;
    default:
      digitalWrite(ledAmarelo, HIGH);
      Serial.println("Option not available");
      digitalWrite(ledAmarelo, LOW);
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
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  Serial.begin(9600);
  Serial.println("Connecting...");
  digitalWrite(ledAmarelo, HIGH);
  
  while (!Serial) {}

  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAmarelo, LOW);
  } else {
    Serial.println(Ethernet.localIP());
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, LOW);
  }

  // Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente
  if (client.connect("celular", "iot2", "iot2")) {
    Serial.println("Connected");
    // Envia uma mensagem para o cloud no topic portao
    client.publish("portao2", "hello world");
    Serial.println("portao2 sent");

    //Feedback Luz que a conexão com o servidor esta OK
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, LOW);
    // Conecta no topic para receber mensagens
    client.subscribe("portao2");
    Serial.println("conectado portao2");
  } else {
    Serial.println("Failed to connect to MQTT server");
    //Feedback Luz que a conexão com o servidor esta OK
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
  }
}


void loop() {
  // A biblioteca PubSubClient precisa que este método seja chamado em cada iteração de `loop()`
  // para manter a conexão MQTT e processar mensagens recebidas (via a função callback)
  client.loop();
  
  estadoBotaoMotor = digitalRead(botaoMotor);
  estadoBotaoLed = digitalRead(botaoLed);

  //TESTE da posicao do servo motor
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
//FUNÇÕES
//////////////////////////////////////////////

////FUNÇÃO Para mandar msg ao MQTT indicando que o LED esta Aceso
//void msgLedAcesso() {
//  client.publish("portao", 5);
//}
//
////FUNÇÃO Para mandar msg ao MQTT indicando que o LED esta Apagado
//void msgLedApagado() {
//  client.publish("portao", "6");
//}

//Função ABRIR Portao
void abrirPortao () {
  if (motor.read() == 180) {
    for (i = 180; i >= 90; i--) {
      motor.write(i);
      delay(10);
    }
  }
}

//Função FECHAR Portao
void fecharPortao () {
  if (motor.read() == 90) {
    for (i = 90; i <= 180; i++) {
      motor.write(i);
      delay(10);
    }
  }
}

//Função ACENDER Luz
void acenderLed () {
  digitalWrite(led, HIGH);
  delay(500);
  //  msgLedAcesso();
}

//Função APAGAR Luz
void apagarLed () {
  digitalWrite(led, LOW);
  delay(500);
  //  msgLedApagado();
}

void  ligarDesligarLed() {
  digitalWrite(led, !digitalRead(led));
  delay(500);
  //    if (digitalRead(led) == HIGH) {
  //      client.publish("portao", "Led Aceso");
  //    } else {
  //      client.publish("portao", "Led Apagado");
  //    }
}

//////////////////////////////////////////////
