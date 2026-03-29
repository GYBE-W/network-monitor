objs = sniffer.o
name = network-monitor
flags = -Wall -lpcap


$(name): $(objs)
	gcc $(flags) -o $(name) $(objs)

sniffer.o: sniffer.c
	gcc $(flags) -c sniffer.c

clean:
	rm -f $(name) $(objs)