import time
import json
from random import randint
import paho.mqtt.publish as mqtt

#Setari MQTT
mqttBroker = "mqtt.beia-telemetrie.ro"
port = 1883
topic = "training/device/DinuAlexandru"

#Functia pentru generarea numarului aleator
def random_number():
    return randint(0, 10)

#Generarea numarului aleator
while True:
    value = random_number()
    payload_dict = {"random_number": value}
    try:
        mqtt.single(topic, qos=1, hostname=mqttBroker, payload=json.dumps(payload_dict))
    except:
        pass
    print(value)
    time.sleep(10)
