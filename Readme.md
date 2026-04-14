# 1. Architecture

Sniffer (C/libpcap): Runs in host networking mode to access raw socket data. It identifies protocols (TCP/UDP) and packet sizes.

Parser (C/libcurl): Formats packet data into InfluxDB Line Protocol and sends it via HTTP POST to the local loopback.

InfluxDB 2.7: Time-series database storing traffic metrics.

Grafana: Visualization dashboard connected to InfluxDB.


# 2. Import file for local docker
docker load -i network-monitor.tar

# 3. Running with the docker compose
DOCKER_BUILDKIT=0 IFACE=XXX docker-compose up --build (build e up) -d (dettached)

# 4. Check if data is flowing
docker-compose logs -f sniffer.

