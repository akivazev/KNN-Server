# compiler
CXX = clang++

# compiler flags
CXXFLAGS = -Wall -g -std=c++11

# include directories
#INCLUDES = -I./src/**

# source files
SRC_DIR = cli knnserver knnset

SRC_ALL = $(wildcard src/cli/*.cpp) $(wildcard src/knnserver/*.cpp) $(wildcard src/knnset/*.cpp)

SRC_SERVER = $(wildcard src/cli/*.cpp) $(wildcard src/knnset/*.cpp) src/knnserver/AbstractPeer.cpp \
	src/knnserver/KNN_Client_Handler.cpp src/knnserver/KNN_MainServer.cpp src/main/server_main.cpp

SRC_CLIENT = $(SRC_ALL) src/main/client_main.cpp

# object files
OBJS_SERVER = $(SRC_SERVER:.cpp=.o)

OBJS_CLIENT = $(SRC_CLIENT:.cpp=.o)


all: server.out client.out

server: server.out

server.out: $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^
	#rm $(OBJS_SERVER)

client.out: $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean_objs:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT)
clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT) *.out