CXXFLAGS = --std=c++11 -g -pthread

CXXFLAGS += -I./asio/include -DASIO_STANDALONE=1

server: server.cpp server.h  Makefile
	g++ -o ctw-server server.cpp $(CXXFLAGS)

start:
	./ctw-server -p 9200
test:
	./client.py

clean:
	rm ctw-server
