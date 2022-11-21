from socket import *
import time
import random

# What's your IP address and witch port should we use?
receive_host = '127.0.0.1'
receive_port = 30000
print ("Receive host: " + receive_host +" / Receive Port: "+str(receive_port))

# Create a UDP socket
# Notice the use of SOCK_DGRAM for UDP packets
serverSocket = socket(AF_INET, SOCK_DGRAM)
# Assign IP address and port number to socket
serverSocket.bind((receive_host, receive_port))

serverSocket.settimeout(20)

random.seed()

while True:
    try:
        message, address = serverSocket.recvfrom(40)
        print (f'Receive: {message}')

        #Decode
        sequenceNumber = int(message[0:5])
        requisitionType = message[5:6].decode()
        timestamp = int(message[6:10].decode())
        messagem = message[10:].decode()

        #encode
        message = bytearray(f'{sequenceNumber:05d}'.encode())
        message.extend("1".encode())
        message.extend(f"{timestamp:04d}".encode())
        message.extend(messagem.encode())

        sleep_time = random.random()*1.5
        print("Sleep time: " + str(sleep_time))
        time.sleep(sleep_time)

        serverSocket.sendto(message, address)
    except ConnectionResetError:
        continue
    except TimeoutError:
        break
