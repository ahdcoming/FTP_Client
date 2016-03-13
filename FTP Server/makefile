CC=clang
CFLAGS=-Wall -c -std=gnu99 -Wno-unknown-pragmas -Wno-unused-variable -o3
LDFLAGS=-l pthread -largp
SOURCES=main.c utils.c argparser.c configparser.c ftpclient.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mftp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o mftp
