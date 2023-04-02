# Exercício 4 AVA

MQTT com ESP32

Implementações para atividade avaliativa da UNISAL

# Modificações:

a. Modifique a tarefa DHT_task para ser uma tarefa mais ampla, como "Sensor_Task", a fim de realizar a leitura dos seguintes sinais a cada 5s:
- Temperatura (via DHT22)
- Umidade (via DHT22)
- Posição do Potenciômetro(convertida para 0 a 180 graus, via Potenciômetro)

b. Monitore o sinal do botao conectado ao equipamento, configurado em Pull-Up.
Caso seja pressionado, envie um pacote para unisal/iot/matheusesperanca/alert, com o seguinte corpo:
{
  "id": id do dispositivo
  "temp": temperatura
  "umid": umidade
  "pot": angulo (0-180) do eixo do potenciômetro
}

c. Com "subscribe" no tópico unisal/iot/matheusesperanca/cmd, interprete o seguinte corpo de payload JSON para controlar o LED conectado no sinal 19:
{
  "code" : 42,
  "cmd" : 0 ou 1
}

Dessa forma, o ESP32 só deverá acionar o LED se receber um pacote JSON com "cmd_code" igual a 42, e caso "cmd" seja 0, desliga o LED, caso seja 1, liga o LED.
