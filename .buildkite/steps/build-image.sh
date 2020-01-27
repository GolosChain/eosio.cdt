#!/bin/bash
set -euo pipefail

IMAGETAG=${BUILDKITE_BRANCH:-master}

VERSION=$(git rev-parse HEAD)

if [[ "${IMAGETAG}" == "master" ]]; then
    BUILDTYPE="stable"
elif [[ "${IMAGETAG}" == "alfa" ]]; then
    BUILDTYPE="alfa"
else
    BUILDTYPE="latest"
fi

docker build -t cyberway/cyberway.cdt:${IMAGETAG} --build-arg version=${VERSION} --build-arg buildtype=${BUILDTYPE} -f docker/Dockerfile .
