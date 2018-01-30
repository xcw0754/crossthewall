#!/usr/bin/env python3
#coding:utf-8
import socket

sock = socket.socket()
sock.connect(("127.0.0.1", 9200))

def main():
    while True:
        sth = input("Please type something:\n")
        if sth == "quit" or sth=="exit":
            sock.sendall(bytes(sth, encoding="utf-8"))
            break
        else:
            sock.sendall(bytes(sth, encoding="utf-8"))
            print ("OK\n")

if __name__ == "__main__":
    main()
