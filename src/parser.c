#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <curl/curl.h>

void send_to_influx(const char *proto, int length) {
    CURL *curl;
    CURLcode res;

    char *token = getenv("DOCKER_INFLUXDB_INIT_ADMIN_TOKEN");
    char *org = getenv("DOCKER_INFLUXDB_INIT_ORG");
    char *bucket = getenv("DOCKER_INFLUXDB_INIT_BUCKET");

    if (token == NULL)  printf("MACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACAC");
    if (org == NULL)    printf("MACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACAC");
    if (bucket == NULL) printf("MACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACACAC");

    curl = curl_easy_init();
    if(curl) {
        // Ajusta o URL: nome do serviço no compose (influxdb), porta, org e bucket
        // Change this line in parser.c
	    char url[256];
        snprintf(url,sizeof(url),"http://127.0.0.1:8086/api/v2/write?org=%s&bucket=%s&precision=s",org,bucket);
        
        // O Token que o teu sócio definiu (ou que geras no Influx)

        struct curl_slist *headers = NULL;
        char tokenHeader[256];
        snprintf(tokenHeader,sizeof(tokenHeader),"Authorization: Token %s",token);
        headers = curl_slist_append(headers, tokenHeader);
        headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8");

        // Formatar os dados (Line Protocol)
        char data[100];
        snprintf(data, sizeof(data), "traffic,protocol=%s size=%d", proto, length);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        res = curl_easy_perform(curl);

        // 2. Verifica se a REDE (CURL) funcionou
        if(res != CURLE_OK) {
            fprintf(stderr, "[-] Erro de REDE (Curl): %s\n", curl_easy_strerror(res));
        } else {
            // 3. Verifica o que o INFLUXDB respondeu
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if(http_code == 204) {
                //printf("[+] SUCESSO: Métrica enviada (HTTP 204)\n");
            } else {
                //printf("[!] ERRO INFLUXDB: Código %ld\n", http_code);
                //printf("    (Dica: 401=Token, 404=Bucket/Org, 400=Sintaxe)\n");
            }
        }

        // 4. Limpeza obrigatória para não haver memory leaks
        curl_slist_free_all(headers); 
        curl_easy_cleanup(curl);
    }
}

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
        //printf("Origem: %s | ", inet_ntoa(ip_header->ip_src));
        //printf("Destino: %s | ", inet_ntoa(ip_header->ip_dst));
        
        // 3. Identificar Protocolo [cite: 64]
        if (ip_header->ip_p == IPPROTO_TCP) {
            //printf("Protocolo: TCP\n");
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            //printf("Protocolo: UDP\n");
        } else {
            //printf("Protocolo: Outro (%d)\n", ip_header->ip_p);
        }
        
        // 4. Tamanho do pacote [cite: 65]
        //printf("Tamanho: %d bytes\n", header->len);
        //printf("------------------------------------------\n");

        char *proto_name = "Outro";
    int tamanho = header->len; // O tamanho real do pacote em bytes

    if (ip_header->ip_p == IPPROTO_TCP) {
        proto_name = "TCP";
    } else if (ip_header->ip_p == IPPROTO_UDP) {
        proto_name = "UDP";
    }

    // AQUI É QUE CHAMAS A FUNÇÃO:
    send_to_influx(proto_name, tamanho);
    }
}
