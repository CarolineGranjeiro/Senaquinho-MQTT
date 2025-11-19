//SUBSCRIBER

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>


// Wi-Fi credentials
const char* ssid = "Carol Granjeiro";
const char* password = "cacarangueijo";


// MQTT broker details
const char* mqtt_broker = "8eb4b70fd73f43e1b161669a90f7b69f.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "senaquinho";
const char* mqtt_password = "Senaquinho2025";


const int saidaPin = 10;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println();
  Serial.print("Conectando à rede: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  Serial.print("Payload: ");
  Serial.println(payloadStr);

  // Exemplo: se você quiser ligar um LED quando receber "1"
  int valor = atoi(payloadStr);
  digitalWrite(saidaPin, valor);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");

    if (client.connect("ESP32Subscriber", mqtt_username, mqtt_password)) {

      Serial.println("conectado");

      // ⬇️ SE INSCREVE NOS MESMOS TÓPICOS DO SEU PUBLISHER
      client.subscribe("placa1/temperatura");
      client.subscribe("placa1/umidade");
      client.subscribe("placa1/botao");

      Serial.println("Inscrito nos tópicos:");
      Serial.println("placa1/temperatura");
      Serial.println("placa1/umidade");
      Serial.println("placa1/botao");

    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(saidaPin, OUTPUT);

  setup_wifi();

  espClient.setInsecure();  
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}