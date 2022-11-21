#Desenvolvido por: Guilherme Goes Zanetti

from socket import *
import time
import math

num_pings = 10
wait_time = 1 #seconds

# Create a UDP socket. UDP is datagram based.
destination = ("127.0.0.1", 30000)
udpClientSocket = socket(AF_INET, SOCK_DGRAM)
#Define the max time between pings (wait time)
udpClientSocket.setblocking(False)

#initialize packet array
packets = [-1] * num_pings

def getTime():
    return int(time.time()*1000)%10000

def encode_message(sequenceNumber, requisitionType, timestamp): 
    #encode
    message = bytearray(f'{sequenceNumber:05d}'.encode())
    message.extend(f"{requisitionType}".encode())
    message.extend(f"{timestamp:04d}".encode())
    message.extend("Guilherme Goes".encode())

    return message

#function to decode message received
def decode_message(message):
    #Decode
    try:
        sequenceNumber = int(message[0:5].decode())
    except:
        print(f"Error: Couldn't decode sequence number {message[0:5]} of package")
        raise ValueError
    try:
        requisitionType = message[5:6].decode()
    except:
        print(f"Error: Couldn't decode requisition type {message[5:6]} of package {sequenceNumber}")
        raise ValueError
    try:
        timestamp = int(message[6:10].decode())
    except:
        print(f"Error: Couldn't decode timestamp {message[6:10]} of package {sequenceNumber}")
        raise ValueError
    
    return (sequenceNumber, requisitionType, timestamp)

#Function to handle a received UDP packet
def handle_response(reply):
    if(len(reply)<10): #If reply doesn't have all info needed
        #ignores it
        return -1
    try:
        sequenceNumber, requisitionType, timestamp = decode_message(reply)
        print(f"Received >> Sequence: {sequenceNumber} / Requisition: {requisitionType} / timestamp: {timestamp}")
    except: #if there was some error in decoding, ignore package
        return -1

    #Check if there might be an error in the package
    if(sequenceNumber < 0 or sequenceNumber >= num_pings):
        #something is wrong
        return -1
    if(requisitionType != "1"):
        print(f"Error: Received a PING from packet {sequenceNumber} instead of a PONG")
        return -1

    current_time = getTime()
    if current_time < timestamp:
        current_time += 10000
    RTT = current_time - timestamp
    packets[sequenceNumber] = RTT

    return sequenceNumber



##MAIN
######### Send the packets and register the RTT when received response
for i in range(0, num_pings):
    message = encode_message(i, 0, getTime())
    # Send message to server
    sequenceNumber, requisitionType, timestamp = decode_message(message)
    print(f"\nSent >> Sequence: {sequenceNumber} / Requisition: {requisitionType} / timestamp: {timestamp}")
    udpClientSocket.sendto(message, destination)
    #Wait for response
    try:
        time_start = getTime()
        time_now = getTime()

        while(time_now - time_start < wait_time*1000): #exit when waited for more than the time limit
            time_now = getTime()
            if time_now < time_start: #if time measured passed from 9999 to 0000
                time_now += 10000
            try:
                reply = udpClientSocket.recv(40)
                sequenceNumberReply = handle_response(reply) #handle any packet that arrives
                if sequenceNumberReply == i:
                    #Exit loop because the current packet was received
                    break
            except BlockingIOError:
                continue
            except:
                print("Unexpected Error!")
                continue
        if time_now - time_start >= wait_time*1000:
            print(f"Package {i} took more than {wait_time} seconds to come back, package might be lost")
    except ConnectionResetError:
        print(f"ERROR: Couldn't connect with server on {destination[0]}:{destination[1]}")
        exit()
    
    

######### Calculate rtts and loss
packets_received = [packet for packet in packets if packet!=-1]

#print(packets)

num_packets_received = len(packets_received)
loss = 100*(num_pings - num_packets_received )/num_pings
#RTT min, médio, max e desvio padrão.
if num_packets_received <= 0:
    rtt_min = 0
    rtt_medio = 0
    rtt_max = 0
    rtt_desvio = 0
else:
    rtt_min = min(packets_received)
    rtt_medio = sum(packets_received)/num_packets_received
    rtt_max = max(packets_received)
    rtt_desvio = 0
    for rtt in packets_received:
        rtt_desvio += (rtt - rtt_medio)**2
    rtt_desvio = math.sqrt(rtt_desvio/num_packets_received)

print(f"\n{num_pings} packets transmitted, {num_packets_received} received, {loss}% packet loss")
print(f"rtt min/avg/max/mdev = {'%.3f' % rtt_min}/{'%.3f' % rtt_medio}/{'%.3f' % rtt_max}/{'%.3f' % rtt_desvio} ms")
