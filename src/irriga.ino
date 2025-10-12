#include <DHT.h>

// Definição dos pinos
const int PIN_BTN_N = 32;      // Botão Nitrogênio
const int PIN_BTN_P = 14;      // Botão Fósforo
const int PIN_BTN_K = 13;      // Botão Potássio
const int PIN_SENSOR_PH = 34;  // Sensor pH (LDR)
const int PIN_DHT = 21;        // Sensor DHT22
const int PIN_BOMBA = 17;      // Relé da bomba d'água

// Variáveis para armazenar o estado dos nutrientes
bool estadoN = false;
bool estadoP = false;
bool estadoK = false;

// Variáveis para armazenar o estado anterior dos botões
bool lastBtnN = HIGH;
bool lastBtnP = HIGH;
bool lastBtnK = HIGH;

// Configuração do sensor DHT22
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

// Estrutura para armazenar os dados dos sensores
struct DadosSensores {
    bool nivelN;
    bool nivelP;
    bool nivelK;
    float ph;
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
    pinMode(PIN_BTN_N, INPUT_PULLUP);
    pinMode(PIN_BTN_P, INPUT_PULLUP);
    pinMode(PIN_BTN_K, INPUT_PULLUP);
    pinMode(PIN_BOMBA, OUTPUT);
    
    // Inicializa o sensor DHT
    dht.begin();
    
    Serial.println("Sistema de Irrigação Inteligente - Cultura do Café");
    Serial.println("Iniciando...");
}

// Função para ler todos os sensores
DadosSensores lerSensores() {
    DadosSensores dados;
    
    // Lê os estados atuais dos botões (LOW quando pressionado)
    bool currentBtnN = digitalRead(PIN_BTN_N);
    bool currentBtnP = digitalRead(PIN_BTN_P);
    bool currentBtnK = digitalRead(PIN_BTN_K);
    
    // Verifica mudança de estado do botão N (falling edge)
    if (currentBtnN == LOW && lastBtnN == HIGH) {
        estadoN = !estadoN;  // Inverte o estado
    }
    lastBtnN = currentBtnN;
    
    // Verifica mudança de estado do botão P (falling edge)
    if (currentBtnP == LOW && lastBtnP == HIGH) {
        estadoP = !estadoP;  // Inverte o estado
    }
    lastBtnP = currentBtnP;
    
    // Verifica mudança de estado do botão K (falling edge)
    if (currentBtnK == LOW && lastBtnK == HIGH) {
        estadoK = !estadoK;  // Inverte o estado
    }
    lastBtnK = currentBtnK;
    
    // Atualiza os níveis com os estados armazenados
    dados.nivelN = estadoN;
    dados.nivelP = estadoP;
    dados.nivelK = estadoK;
    
    // Lê o sensor de pH (LDR)
    int valorADC = analogRead(PIN_SENSOR_PH);
    dados.ph = (valorADC * 14.0) / 4095.0;  // Converte para escala pH (0-14)
    
    // Lê umidade
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
    
    // Aguarda 1 segundo antes da próxima leitura
    delay(1000);
}