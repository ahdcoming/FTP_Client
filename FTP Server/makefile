CFLAGS=-Wall -c

all: mftp test_arges

test_args: mftp
	./mftp -a -f file -s hostname -p 22 -n user -P password -m ASCII -l logfile -w config -b 20


mftp: main.o argparser.o
	gcc -o mftp -l argp main.o argparser.o

main.o: main.c options.h argparser.h
	gcc $(CFLAGS) main.c

argparser.o: argparser.c options.h argparser.h
	gcc $(CFLAGS) argparser.c

ftpclient.o: ftpclient.c options.h ftpclient.h
	gcc $(CFLAGS) ftpclient.c

clean:
	rm mftp *.o

