files = sniffer.c
name = network-monitor
flags = -Wall -lpcap


all:
	gcc -o $(name) $(files) $(flags)