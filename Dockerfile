# Stage 1: Build
FROM gcc:12 AS builder
RUN apt-get update && apt-get install -y libpcap-dev libcurl4-openssl-dev
WORKDIR /app
COPY . .
RUN make

# Stage 2: Runtime
FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y libpcap0.8 && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=builder /app/network-monitor .
ENTRYPOINT ["./network-monitor"]
