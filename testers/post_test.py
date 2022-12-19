import requests, json

url = 'http://localhost:80'

# Test 1
requests.post(url, data= json.dumps({'name': 'John', 'age': 27}))