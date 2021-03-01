import serial
import requests
import datetime


ser = serial.Serial('/dev/ttyACM0',115200)
s = [0,1]
def send(val):
    url = "http://mhs.ceerduad.com/masuk/kabira.php"

    payload = {'b1': str(val)}
    files = [

    ]
    headers= {}

    response = requests.request("POST", url, headers=headers, data = payload, files = files)

    print(response.text.encode('utf8'))
while True:
    now = datetime.datetime.now()
    data = ser.readline().decode("utf-8")
    print (data)
    
    print(now.second)
    if(now.second==1):
        send(data)

