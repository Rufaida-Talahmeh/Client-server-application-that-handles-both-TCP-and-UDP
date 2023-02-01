# Server
gcc server.c -o server 
./server [PortNum]  
like: ./server 5555 

# UDP client
gcc UDP-client.c -o UDP-client 
./UDP-client [IPaddress] [PortNum]  
like: ./UDP-client 127.0.0.1 5555

# TCP client
gcc TCP-client.c -o TCP-client 
./TCP-client [IPaddress] [PortNum]  
like: ./TCP-client 127.0.0.1 5555
