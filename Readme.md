docker run --rm -it \
    --network host \
    --cap-add NET_RAW \
    --cap-add NET_ADMIN \
    network-monitor wlp2s0