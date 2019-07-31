ARG buildtype=stable

FROM cyberway/builder:$buildtype as builder
ARG branch=master

ADD https://api.github.com/repos/cyberway/cyberway.cdt/git/refs/heads/$branch /etc/version.json

RUN git clone -b $branch https://github.com/cyberway/cyberway.cdt.git --recursive

RUN cd cyberway.cdt \
    && echo "$branch:$(git rev-parse HEAD)" > version \
    && cmake -H. -B"/tmp/build" \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_INSTALL_PREFIX=/opt/cyberway.cdt \
    && cmake --build /tmp/build --target install

FROM ubuntu:18.04

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/cyberway.cdt /opt/cyberway.cdt
COPY --from=builder /cyberway.cdt/version /opt/cyberway.cdt/version
