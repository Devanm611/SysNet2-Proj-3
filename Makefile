CXX	=	g++
CXXFLAGS	=	-Wall	-g	-pthread

all:	server	client

server:	server.cpp	User.cpp
	$(CXX)	$(CXXFLAGS)	-o	server	server.cpp	User.cpp

client:	client.cpp	TcpClient.cpp
	$(CXX)	$(CXXFLAGS)	-o	client	client.cpp	TcpClient.cpp

clean:
	rm	-f	server	client
