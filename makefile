all : client server rbc

client: client.c
	gcc client.c -o client

server: server.c
	gcc server.c -o server

rbc: rbc.c rbc.h
	gcc rbc.c -o rbc

clean:
	rm -rf client server rbc