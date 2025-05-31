// Inclusão das bibliotecas necessárias para conexão Wi-Fi e requisições HTTP
#include <WiFi.h>
#include <HTTPClient.h>

// Credenciais da rede Wi-Fi
const char* ssid = "S23 de Francisco";
const char* password = "pyx728fkehiwwdp";

// Pino digital conectado ao sensor KY-032
const int digPin = 12;

// URL da API do ThingSpeak para envio de dados
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "Z9IG5H47M54IJRKC"; // Chave da API do canal

// Variável para contar quantas vezes um obstáculo foi detectado
int countDetections = 0;

// Variáveis para controle de tempo
unsigned long previousMillis = 0;
const long interval = 15000; // Envia dados a cada 15 segundos

void setup() {
  // Configuração dos pinos
  pinMode(digPin, INPUT);   // Pino do sensor como entrada
  pinMode(13, OUTPUT);      // LED no pino 13 como saída (indicador)

  Serial.begin(115200);     // Inicializa comunicação serial para depuração

  // Conexão com a rede Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  // Leitura do sensor de obstáculo (LOW indica obstáculo detectado)
  int digValue = digitalRead(digPin);

  // Se obstáculo detectado, incrementa o contador
  if (digValue == LOW) {
    Serial.println("Objeto detectado");
    countDetections++;
  } else {
    Serial.println("Nenhum objeto detectado");
  }

  // Acende o LED como indicador de leitura ativa
  digitalWrite(13, HIGH);

  // Mostra valores lidos no monitor serial
  Serial.print("Valor lido do sensor: ");
  Serial.println(digValue);
  Serial.print("Contador de detecções: ");
  Serial.println(countDetections);

  // Verifica se já passou o intervalo de tempo para envio
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Se estiver conectado ao Wi-Fi, envia os dados
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      // Monta a URL com os dados a serem enviados
      String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(countDetections);
      http.begin(url); // Inicializa a requisição HTTP
      int httpResponseCode = http.GET(); // Envia a requisição GET
      countDetections = 0; // Zera o contador após o envio

      // Exibe resposta da requisição
      if (httpResponseCode > 0) {
        Serial.print("Código de resposta HTTP: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Erro ao enviar dados: ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
      }

      http.end(); // Finaliza conexão HTTP
    } else {
      // Caso Wi-Fi desconecte, tenta reconectar
      Serial.println("WiFi desconectado. Tentando reconectar...");
      WiFi.begin(ssid, password);
    }
  }

  delay(100); // Pequeno atraso para evitar sobrecarga no loop
}

