CC=g++
CFLAGS=-std=c++11 -c -Wall
LDFLAGS=-L. -lcurl -Wl,-rpath,.
INCLUDES= 
SOURCES=	\
	fs.cpp \
	net.cpp \
	vk.cpp \
	lp.cpp \
	msg.cpp \
	str.cpp \
	cmd.cpp \
	cmds.cpp \
	other.cpp \
	modules.cpp \
	main.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE=vkbot

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

.cpp.o: 	
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
