# Stage 1: Build (Já deves ter isto bem)
FROM gcc:12 AS builder
RUN apt-get update && apt-get install -y libpcap-dev libcurl4-openssl-dev
WORKDIR /app
COPY . .
RUN make clean && make

# Stage 2: Runtime (O CULPADO ESTÁ AQUI)
FROM debian:bookworm-slim
# Tens de instalar as versões de "execução" das bibliotecas
RUN apt-get update && apt-get install -y \
    libpcap0.8 \
    libcurl4 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/network-monitor .

# Não te esqueças de passar a interface no command do docker-compose!
ENTRYPOINT ["./network-monitor"]