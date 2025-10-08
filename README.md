# Sistema de Irrigação Inteligente para Cultivo de Café

## Visão Geral do Projeto
Este projeto implementa um sistema de irrigação automatizado para o cultivo de café, utilizando um ESP32 para monitorar e controlar diversos parâmetros cruciais para o desenvolvimento saudável da plantação. O sistema integra múltiplos sensores para monitorar nutrientes (NPK), pH do solo e umidade, tomando decisões automatizadas sobre a irrigação com base nas condições ideais para o cultivo do café.

## Requisitos do Sistema

### Hardware
- ESP32 DevKit
- 3 Botões (simulando sensores NPK)
- Sensor LDR (simulando sensor de pH)
- Sensor DHT22 (monitoramento de umidade)
- Módulo Relé (controle da bomba d'água)

### Conexões dos Componentes

![Diagrama de Conexões](conexoes.png)

**Detalhamento das Conexões:**

- **Botões NPK:**
  - Botão N: GPIO32
  - Botão P: GPIO33
  - Botão K: GPIO25
  - Todos com pull-down e conectados ao GND

- **Sensor pH (LDR):**
  - Pino analógico: GPIO34
  - VCC: 3.3V
  - GND: GND

- **Sensor DHT22:**
  - Pino de dados: GPIO27
  - VCC: 3.3V
  - GND: GND

- **Relé da Bomba:**
  - Pino de controle: GPIO26
  - VCC: 3.3V
  - GND: GND

## Parâmetros Ideais para Cultivo do Café

### Condições Monitoradas

1. **pH do Solo**
   - Faixa ideal: 5.5 a 6.5
   - Monitoramento através do sensor LDR
   - Valores convertidos para escala de pH (0-14)
   - Crucial para absorção adequada de nutrientes

2. **Umidade do Solo**
   - Faixa ideal: 60% a 80%
   - Monitorado pelo sensor DHT22
   - Sistema de irrigação ativa quando < 60%
   - Prevenção de irrigação excessiva > 80%

3. **Nutrientes (NPK)**
   - Nitrogênio (N): Crescimento vegetativo
   - Fósforo (P): Desenvolvimento das raízes
   - Potássio (K): Qualidade dos frutos
   - Monitoramento binário (presente/ausente)

4. **Temperatura**
   - Faixa ideal: 18°C a 25°C
   - Monitorada pelo sensor DHT22
   - Alertas para valores fora da faixa

## Lógica de Funcionamento

### Sistema de Irrigação
1. **Coleta de Dados**
   - Leitura contínua de todos os sensores
   - Intervalo de 2 segundos entre leituras
   - Conversão de valores analógicos para escalas apropriadas

2. **Análise de Condições**
   - Verificação de todos os parâmetros
   - Comparação com valores ideais
   - Geração de alertas quando necessário

3. **Tomada de Decisão**
   - Irrigação ativada se umidade < 60%
   - Consideração do estado de nutrientes
   - Monitoramento do pH para ajustes

### Sistema de Alertas
- Indicação de pH fora da faixa ideal
- Alertas de umidade inadequada
- Notificação de deficiências de nutrientes
- Avisos de temperatura inadequada

## Implementação

### Estrutura do Código
- Modular e organizado em funções específicas
- Sistema de monitoramento contínuo
- Lógica de decisão baseada em parâmetros científicos
- Interface serial para debug e monitoramento

### Principais Funções
```cpp
DadosSensores lerSensores()
// Coleta dados de todos os sensores e retorna estrutura unificada

StatusSistema verificarCondicoesCafe()
// Analisa condições e determina necessidade de irrigação

void imprimirStatus()
// Exibe informações no monitor serial
```

## Monitoramento
- Display contínuo de leituras no monitor serial
- Atualização a cada 2 segundos
- Exibição de todos os parâmetros monitorados
- Sistema de alertas em tempo real

## Manutenção e Calibração

### Ajustes Necessários
1. **Sensor de pH (LDR)**
   - Calibrar a conversão analógica para escala de pH
   - Ajustar sensibilidade conforme necessidade

2. **Sistema de Irrigação**
   - Ajustar tempos de ativação da bomba
   - Calibrar vazão conforme necessidade da plantação

3. **Sensores NPK**
   - Possibilidade de implementar sensores reais
   - Ajustar lógica conforme sensores utilizados

## Demonstração
https://youtu.be/Maczs9Bmi7M

## Colaboradores
Erik Criscuolo, Daniel Emilio Baião, 
Marcus Vinícius Loureiro Garcia,
Sidney William de Paula Dias