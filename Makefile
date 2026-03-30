objs = sniffer.o parser.o
name = network-monitor
flags = -Wall
libs = -lpcap


$(name): $(objs)
	gcc $(flags) -o $(name) $(objs) $(libs)

sniffer.o: sniffer.c
	gcc $(flags) -c sniffer.c $(libs)

parser.o: parser.c
	gcc $(flags) -c parser.c $(libs)

clean:
	rm -f $(name) $(objs)