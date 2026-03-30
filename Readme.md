# 1. Importar o ficheiro para o Docker local
docker load -i network-monitor.tar


# 2. Correr normalmente (com as flags de rede do teu plano)
docker run --rm -it \
    --network host \
    --cap-add NET_RAW \
    --cap-add NET_ADMIN \
    network-monitor wlp2s0