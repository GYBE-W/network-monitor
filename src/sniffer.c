#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include "parser.h"

pcap_t* abrir_interface_wifi(char *nome_interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // 1. Abrir a interface para captura "live"
    // BUFSIZ é o tamanho do buffer, 1 ativa o modo promíscuo, 1000 é o timeout em ms
    handle = pcap_open_live(nome_interface, BUFSIZ, 1, 1000, errbuf);
    
    if (handle == NULL) {
        fprintf(stderr, "Não foi possível abrir a interface %s: %s\n", nome_interface, errbuf);
        return NULL;
    }

    // 2. Verificar se estamos a capturar Ethernet (Wi-Fi normalmente emula Ethernet)
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "%s não fornece cabeçalhos Ethernet.\n", nome_interface);
        return NULL;
    }

    return handle;
}


int main(int argc, char*argv[]){
    char*interface_name=argv[1];
    printf("I am a sniffer\n");
    char*ligma="ligma";
    printf("%u\n",PCAP_CHAR_ENC_UTF_8);
    pcap_t*wifi=abrir_interface_wifi(interface_name);
    pcap_loop(wifi,0, packet_handler, NULL);
    printf("gormiti");
    //pcap_init(PCAP_CHAR_ENC_LOCAL,ligma);
}