# Build image

```shell
docker build -t alert-gui:latest .

docker run --rm -it \
    --name "$CONTAINER_NAME" \
    -e DISPLAY="$DISPLAY" \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    --network host \
    "$IMAGE_NAME" \
    bash
```
