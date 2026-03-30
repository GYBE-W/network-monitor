objs = sniffer.o parser.o
name = network-monitor
flags = -Wall
libs = -lpcap


$(name): $(objs)
	gcc $(flags) -o $(name) $(objs) $(libs)

sniffer.o: src/sniffer.c
	gcc $(flags) -c src/sniffer.c $(libs)

parser.o: src/parser.c
	gcc $(flags) -c src/parser.c $(libs)

clean:
	rm -f $(name) $(objs)