from threading import Thread
import requests
from random import randint

PORTS = ["80", "8080"]
def send_requests():
    requests.get(f'http://localhost:{PORTS[randint(0,1)]}/')
    return

for i in range(0,10):
    T = Thread(target=send_requests, args=())
    T.start()
    arr = []
    arr.append(T)

for i in arr:
    
    i.join()
