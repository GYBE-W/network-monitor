#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include "parser.h"
#include <curl/curl.h>

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


int main(int argc, char* argv[]) {
    // 1. Verificação de segurança (evita Segmentation Fault se não passares o argv[1])
    if (argc < 2) {
        fprintf(stderr, "Erro: Deves passar o nome da interface (ex: ./network-monitor wlp2s0)\n");
        return 1;
    }

    char* interface_name = argv[1];

    // 2. INICIALIZAÇÃO CRÍTICA DO CURL
    // Isto prepara a memória e os protocolos para o envio para a DB
    curl_global_init(CURL_GLOBAL_ALL);

    printf("I am a sniffer (e agora envio dados para o InfluxDB!)\n");

    pcap_t* wifi = abrir_interface_wifi(interface_name);
    
    if (wifi == NULL) {
        curl_global_cleanup();
        return 1;
    }

    // 3. O Loop de Captura
    // O 0 significa "corre para sempre" até carregar em Ctrl+C
    pcap_loop(wifi, 0, packet_handler, NULL);

    // 4. LIMPEZA (Só chega aqui se o pcap_loop terminar)
    printf("\nA encerrar o sniffer...\n");
    pcap_close(wifi);
    curl_global_cleanup(); // Liberta a memória do Curl

    return 0;
}