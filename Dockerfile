# ESTÁGIO 1: Compilação (Builder) [cite: 76]
FROM gcc:12 AS builder [cite: 77]

# Instalar a biblioteca de desenvolvimento da pcap [cite: 78]
RUN apt-get update && apt-get install -y libpcap-dev

WORKDIR /app [cite: 79]

# Copiar os teus ficheiros conforme a estrutura do plano [cite: 80, 81, 82]
COPY src/ ./src/
COPY include/ ./include/
COPY Makefile .

# Compilar o binário [cite: 83]
RUN make

# ESTÁGIO 2: Runtime (Imagem final pequena) [cite: 84]
FROM debian:bookworm-slim [cite: 85]

# Instalar apenas o runtime da libpcap (sem o lixo do compilador) [cite: 86]
RUN apt-get update && apt-get install -y libpcap0.8 && \
    rm -rf /var/lib/apt/lists/* [cite: 86]

WORKDIR /app [cite: 87]

# Copiar apenas o executável que criámos no estágio anterior [cite: 88]
COPY --from-builder /app/sniffer . 

# Comando para iniciar (podes passar a interface como argumento) [cite: 89]
ENTRYPOINT ["./sniffer"]