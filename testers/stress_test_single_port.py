from threading import Thread
import requests

def send_requests():
    requests.get('http://localhost:80/')
    return

for i in range(0,50):
    T = Thread(target=send_requests, args=())
    T.start()
    arr = []
    arr.append(T)

for i in arr:
    
    i.join()
