ARG buildtype=stable
FROM cyberway/builder:$buildtype as builder

COPY . cyberway.cdt

RUN cd cyberway.cdt \
    && cmake -H. -B"/tmp/build" \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_INSTALL_PREFIX=/opt \
    && cmake --build /tmp/build --target install

FROM ubuntu:18.04
ARG $version

COPY --from=builder /opt/cyberway.cdt /opt/cyberway.cdt

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && echo $version > /opt/cyberway.cdt/version

