import requests
import json
from datetime import datetime, timedelta

# --- CONFIGURAÇÃO OPEN-METEO ---
LATITUDE = -23.5505  # Lat/Lon para sua fazenda
LONGITUDE = -46.6333
HORAS_MONITORAMENTO = 24 
# --- FIM CONFIGURAÇÃO ---

def verificar_previsao_chuva():
    """Consulta a API Open-Meteo (sem chave) e verifica se há precipitação nas próximas 24h."""
    url = (f"https://api.open-meteo.com/v1/forecast?"
           f"latitude={LATITUDE}&longitude={LONGITUDE}&"
           f"hourly=precipitation&"
           f"forecast_days=1&"
           f"timezone=America%2FSao_Paulo")
    
    try:
        response = requests.get(url)
        response.raise_for_status() 
        data = response.json()
        
        previsao_chuva = False
        precipitacoes = data.get('hourly', {}).get('precipitation', [])
        
        # Verifica precipitação maior que 0.0 mm nas próximas horas
        for i in range(min(HORAS_MONITORAMENTO, len(precipitacoes))):
            if precipitacoes[i] > 0.0:
                previsao_chuva = True
                break
        
        status_final = 1 if previsao_chuva else 0 # 1=Bloqueio, 0=OK
        
        print(f"\n[DECISÃO API] Chuva prevista: {'SIM' if previsao_chuva else 'NAO'}")
        print(f"Status para ESP32: {status_final}")
        return status_final

    except requests.exceptions.RequestException as e:
        print(f"Erro ao conectar ou consultar a API: {e}. Retornando status 0 (OK) por segurança.")
        return 0 

if __name__ == "__main__":
    verificar_previsao_chuva()
    # O resultado DESTE script é usado PELO run_analysis.sh para enviar o dado ao ESP32.