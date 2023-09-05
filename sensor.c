#include <DHT.h>
#define DHTPIN 4     // Use D4 para o NodeMCU (corresponde ao pino GPIO2)
#define DHTTYPE DHT11 // Tipo do sensor (DHT11)

#include <WiFi.h>
#include <WebServer.h>

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "lucasmachado";
const char* password = "senai12345";
WebServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Endereço IP do Ponto de Acesso: ");
  Serial.println(myIP);
  
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient(); // Tratar as solicitações dos clientes
  
  delay(2000);  // Aguarde 2 segundos entre as leituras

  float temperature = dht.readTemperature(); // Leitura da temperatura em graus Celsius
  float humidity = dht.readHumidity();       // Leitura da umidade relativa em porcentagem

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha ao ler o sensor DHT11!");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Leitura de Sensores</h1>";
  html += "<p>Temperatura: ";
  html += dht.readTemperature();
  html += " °C</p>";
  html += "<p>Umidade: ";
  html += dht.readHumidity();
  html += " %</p>";
  html += "<p>Lucas Machado e Kaue Teixeira</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}