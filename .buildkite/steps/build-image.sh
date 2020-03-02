#!/bin/bash
set -euo pipefail

REVISION=$(git rev-parse HEAD)

if [[ "${BUILDKITE_BRANCH}" == "master" ]]; then
    BUILDTYPE="stable"
else
    BUILDTYPE="latest"
fi

BUILDER_TAG=${BUILDER_TAG:-$BUILDTYPE}

docker build -t cyberway/cyberway.cdt:${REVISION} --build-arg version=${REVISION} --build-arg buildtype=${BUILDER_TAG} -f docker/Dockerfile .
