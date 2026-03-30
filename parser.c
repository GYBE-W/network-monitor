#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

// Esta é a função que passas para o pcap_loop
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ether_header *eth_header;
    struct ip *ip_header;
    
    // 1. Ethernet Header (14 bytes fixos) 
    eth_header = (struct ether_header *) packet;
    
    // Verificamos se o pacote contém um IP (EtherType 0x0800)
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        
        // 2. IP Header (salta os 14 bytes do Ethernet) 
        ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        
        // Extrair IPs usando inet_ntoa (converte binário para string "192.168...") [cite: 60, 61]
        printf("Origem: %s | ", inet_ntoa(ip_header->ip_src));
        printf("Destino: %s | ", inet_ntoa(ip_header->ip_dst));
        
        // 3. Identificar Protocolo [cite: 64]
        if (ip_header->ip_p == IPPROTO_TCP) {
            printf("Protocolo: TCP\n");
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            printf("Protocolo: UDP\n");
        } else {
            printf("Protocolo: Outro (%d)\n", ip_header->ip_p);
        }
        
        // 4. Tamanho do pacote [cite: 65]
        printf("Tamanho: %d bytes\n", header->len);
        printf("------------------------------------------\n");
    }
}