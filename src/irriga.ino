#include <DHT.h>

// Definição dos pinos
const int PIN_BTN_N = 32;      // Botão Nitrogênio
const int PIN_BTN_P = 33;      // Botão Fósforo
const int PIN_BTN_K = 25;      // Botão Potássio
const int PIN_SENSOR_PH = 34;  // Sensor pH (LDR)
const int PIN_DHT = 27;        // Sensor DHT22
const int PIN_BOMBA = 26;      // Relé da bomba d'água

// Configuração do sensor DHT22
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

// Estrutura para armazenar os dados dos sensores
struct DadosSensores {
    bool nivelN;
    bool nivelP;
    bool nivelK;
    float ph;
    float temperatura;
    float umidade;
};

// Estrutura para armazenar o status do sistema
struct StatusSistema {
    bool irrigar;
    String mensagens[5];
    int numMensagens;
};

void setup() {
    // Inicializa comunicação serial
    Serial.begin(115200);
    
    // Configura os pinos
    pinMode(PIN_BTN_N, INPUT);
    pinMode(PIN_BTN_P, INPUT);
    pinMode(PIN_BTN_K, INPUT);
    pinMode(PIN_BOMBA, OUTPUT);
    
    // Inicializa o sensor DHT
    dht.begin();
    
    Serial.println("Sistema de Irrigação Inteligente - Cultura do Café");
    Serial.println("Iniciando...");
}

// Função para ler todos os sensores
DadosSensores lerSensores() {
    DadosSensores dados;
    
    // Lê os botões NPK (LOW quando pressionado)
    dados.nivelN = !digitalRead(PIN_BTN_N);
    dados.nivelP = !digitalRead(PIN_BTN_P);
    dados.nivelK = !digitalRead(PIN_BTN_K);
    
    // Lê o sensor de pH (LDR)
    int valorADC = analogRead(PIN_SENSOR_PH);
    dados.ph = (valorADC * 14.0) / 4095.0;  // Converte para escala pH (0-14)
    
    // Lê temperatura e umidade
    dados.temperatura = dht.readTemperature();
    dados.umidade = dht.readHumidity();
    
    return dados;
}

// Função para verificar as condições ideais do café
StatusSistema verificarCondicoesCafe(const DadosSensores& dados) {
    StatusSistema status;
    status.numMensagens = 0;
    
    // Verifica pH (ideal: 5.5 - 6.5)
    bool phOk = dados.ph >= 5.5 && dados.ph <= 6.5;
    if (!phOk) {
        String msg = "pH " + String(dados.ph, 1) + " fora do ideal (5.5-6.5)";
        status.mensagens[status.numMensagens++] = msg;
    }
    
    // Verifica umidade (ideal: 60% - 80%)
    bool umidadeOk = dados.umidade >= 60 && dados.umidade <= 80;
    if (!umidadeOk) {
        String msg = "Umidade " + String(dados.umidade, 1) + "% fora do ideal (60-80%)";
        status.mensagens[status.numMensagens++] = msg;
    }
    
    // Verifica temperatura (ideal: 18°C - 25°C)
    bool temperaturaOk = dados.temperatura >= 18 && dados.temperatura <= 25;
    if (!temperaturaOk) {
        String msg = "Temperatura " + String(dados.temperatura, 1) + "C fora do ideal (18-25C)";
        status.mensagens[status.numMensagens++] = msg;
    }
    
    // Verifica nutrientes
    String deficiencias = "";
    if (!dados.nivelN) deficiencias += "N ";
    if (!dados.nivelP) deficiencias += "P ";
    if (!dados.nivelK) deficiencias += "K ";
    
    if (deficiencias.length() > 0) {
        String msg = "Deficiência de nutrientes: " + deficiencias;
        status.mensagens[status.numMensagens++] = msg;
    }
    
    // Define se deve irrigar (quando umidade < 60%)
    status.irrigar = dados.umidade < 60;
    
    return status;
}

void imprimirStatus(const DadosSensores& dados, const StatusSistema& status) {
    Serial.println("\n=== Leitura dos Sensores ===");
    Serial.println("Nitrogênio: " + String(dados.nivelN ? "Presente" : "Ausente"));
    Serial.println("Fósforo: " + String(dados.nivelP ? "Presente" : "Ausente"));
    Serial.println("Potássio: " + String(dados.nivelK ? "Presente" : "Ausente"));
    Serial.println("pH: " + String(dados.ph, 2));
    Serial.println("Temperatura: " + String(dados.temperatura, 1) + "°C");
    Serial.println("Umidade: " + String(dados.umidade, 1) + "%");
    
    Serial.println("\n=== Status do Sistema ===");
    Serial.println(status.irrigar ? "IRRIGAÇÃO ATIVADA - Umidade abaixo do ideal" : "IRRIGAÇÃO DESATIVADA");
    
    if (status.numMensagens > 0) {
        Serial.println("\nAlertas:");
        for (int i = 0; i < status.numMensagens; i++) {
            Serial.println("- " + status.mensagens[i]);
        }
    } else {
        Serial.println("\nTodas as condições estão ideais para o cultivo do café");
    }
}

void loop() {
    // Lê os dados dos sensores
    DadosSensores dados = lerSensores();
    
    // Verifica as condições para o café
    StatusSistema status = verificarCondicoesCafe(dados);
    
    // Atualiza o estado da bomba
    digitalWrite(PIN_BOMBA, status.irrigar);
    
    // Imprime o status no monitor serial
    imprimirStatus(dados, status);
    
    // Aguarda 2 segundos antes da próxima leitura
    delay(2000);
}