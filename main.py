import requests

url = "http://mhs.ceerduad.com/masuk/kabira.php"

payload = {'b1': '100'}
files = [

]
headers= {}

response = requests.request("POST", url, headers=headers, data = payload, files = files)

print(response.text.encode('utf8'))