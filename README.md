# 🌿 FarmTech Solutions - Sistema de Irrigação Inteligente (Fase 2)

## 🎯 Objetivo do Projeto
Desenvolver um sistema de irrigação automatizado e inteligente para a cultura do Café, utilizando um microcontrolador ESP32 (simulado via Wokwi/VS Code) para monitorar sensores virtuais (Umidade, NPK e pH). O projeto integra um pipeline avançado de IoT e Data Science para tomada de decisão e análise estatística.

---

## 🍅 Cultura Agrícola e Lógica de Irrigação
A cultura agrícola escolhida foi o **Café**. A bomba de água (Relé) só será acionada se as condições da lavoura forem adequadas **E** houver necessidade de água.

### Lógica de Decisão (Regra para Ligar a Bomba)
O Relé será acionado **APENAS** quando **TODAS** as seguintes condições forem verdadeiras:

- A Umidade do Solo (DHT22) estiver **ABAIXO de 60%** (Necessidade de Água)
- **TODOS** os nutrientes N, P, e K estiverem presentes (respectivos botões pressionados)
- O pH (LDR) estiver na faixa ideal para o Café (**5.5 a 6.5**)
- **NÃO HOUVER** um Bloqueio Externo ativo (Decisão da API do Tempo)

#### Tabela de Lógica e Simulação

| Elemento              | Requisito da Lógica         | Simulação no Wokwi                  |
|-----------------------|-----------------------------|-------------------------------------|
| Nitrogênio (N)        | Presente (true)             | Botão N Pressionado                 |
| Fósforo (P)           | Presente (true)             | Botão P Pressionado                 |
| Potássio (K)          | Presente (true)             | Botão K Pressionado                 |
| pH (LDR)              | 5.5-6.5 (levemente ácido).  | Potenciômetro do LDR                |
| Umidade do Solo (DHT22)| Baixa (< 60%)              | Slider do DHT22                     |
| Bloqueio Externo      | Ausente (0)                 | Serial Remoto (Script Python)       |

---

## 🔌 Circuito, Pinos e Tecnologias
O projeto utiliza o **ESP32 Devkit V1** simulado no Wokwi via extensão do VS Code/PlatformIO.


![Diagrama de Conexões](conexoes.png)

**Detalhamento das Conexões:**

- **Botões NPK:**
  - Botão N: GPIO32
  - Botão P: GPIO14
  - Botão K: GPIO13
  - Todos com pull-up e conectados ao GND

- **Sensor pH (LDR):**
  - Pino analógico: GPIO34
  - VCC: 3.3V
  - GND: GND

- **Sensor DHT22:**
  - Pino de dados: GPIO21
  - VCC: 3.3V
  - GND: GND

- **Relé da Bomba:**
  - Pino de controle: GPIO17
  - VCC: 3.3V
  - GND: GND

**Tecnologias Utilizadas:** C/C++ (Arduino Framework), PlatformIO, Wokwi, Python, Bash/Shell Script, R.

---

## 🛠️ Pipeline Automatizado de Data Science
Implementamos um pipeline robusto que automatiza a coleta, integração e análise de dados, eliminando a necessidade de intervenção manual para transferir logs e dados da API.

###Pré requisito para o python
- Arquivo requirements.txt está as bibliotecas necessárias para o Python

### 1. Integração com API Pública (Open-Meteo)
- **Conceito:** Suspender a irrigação se houver previsão de chuva.
- **API Utilizada:** Open-Meteo (sem chave API)
- **Fluxo:**
	- O script Bash (`run_analysis.sh`) executa o script Python (`python_api/api_weather.py`)
	- O Python consulta a Open-Meteo para obter a previsão de chuva (0 ou 1)
	- O Bash usa a porta serial remota do Wokwi (`rfc2217://localhost:4000`) para enviar automaticamente o 0 ou 1 para o ESP32, que atualiza a variável `bloqueio_irrigacao`

### 2. Análise Estatística em R
- **Conceito:** Usar estatística para avaliar a qualidade do solo e gerar um Score de Deficiência, refinando a lógica de irrigação.
- **Fluxo:**
	- O script Python (`r_analysis/log_capturer.py`) captura 30 linhas de dados CSV geradas pelo ESP32, salvando em `r_analysis/esp32_log_bruto.csv`
	- O script Python (`r_analysis/data_cleaner.py`) lê o log bruto, limpa os dados, adiciona features de Data Science (`Indice_NPK`) e salva o arquivo final (`dados_para_r.csv`)
	- O script R (`r_analysis/data_analysis.R`) lê o arquivo limpo e executa a análise de Score de Deficiência

### 3. Orquestração do Pipeline
O script Bash (`run_analysis.sh`) gerencia todo o processo:
1. Roda a API Python (instalando os pré requisitos necessários) e envia o resultado para o ESP32
2. Roda o capturador de log Python para coletar os dados
3. Roda o script de limpeza de dados Python
4. Roda a análise estatística em R

#### Execução do Pipeline
```bash
chmod +x run_analysis.sh
./run_analysis.sh
```

---

## 📁 Estrutura de Arquivos e Entregáveis

| Arquivo/Pasta                  | Descrição                                         | Status    |
|--------------------------------|---------------------------------------------------|-----------|
| README.md                      | Documentação principal do projeto                 | Completo  |
| platformio.ini                 | Configuração do ambiente ESP32                    | Entregue  |
| wokwi.toml                     | Configuração da simulação e Serial Remota         | Entregue  |
| diagram.json                   | Diagrama de conexões do circuito                  | Entregue  |
| src/main.cpp                   | Código C++ do ESP32 (Lógica + Saída CSV)          | Entregue  |
| python_api/api_weather.py      | Opcional 1: Lógica de decisão de chuva            | Entregue  |
| r_analysis/data_analysis.R     | Opcional 2: Script de análise estatística em R    | Entregue  |
| r_analysis/log_capturer.py     | Automação: Captura de log Serial                  | Entregue  |
| r_analysis/data_cleaner.py     | Automação: Limpeza e integração final de dados    | Entregue  |
| run_analysis.sh                | Orquestrador Bash do pipeline de Data Science     | Entregue  |
| media/video_demo_link.txt      | Link para a demonstração no YouTube (5 minutos)   | Pendente  |

---

## ▶️ Demonstração em Vídeo
O vídeo deve demonstrar o funcionamento básico (sensores e relé) e a execução do pipeline de Data Science (`run_analysis.sh`).

**Link do Vídeo (YouTube - Não Listado):**
https://youtu.be/pz6-2eiDTZE

## Colaboradores Grupo 9
Erik Criscuolo, Daniel Emilio Baião, 
Marcus Vinícius Loureiro Garcia,
Sidney William de Paula Dias, Hugo Rodrigues Carvalho Silva
