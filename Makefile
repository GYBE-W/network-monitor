files = sniffer.c
name = network-monitor
flags = -Wall


all:
	gcc -o $(name) $(files) $(flags)