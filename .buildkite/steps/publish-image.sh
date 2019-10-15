#!/bin/bash
set -euo pipefail

IMAGETAG=${BUILDKITE_BRANCH:-master}
BRANCHNAME=${BUILDKITE_BRANCH:-master}

docker images

docker login -u=$DHUBU -p=$DHUBP
docker push cyberway/cyberway.cdt:${IMAGETAG}

if [[ "${IMAGETAG}" == "master" ]]; then
    docker tag cyberway/cyberway.cdt:${IMAGETAG} cyberway/cyberway.cdt:stable
    docker push cyberway/cyberway.cdt:stable
fi

if [[ "${IMAGETAG}" == "develop" ]]; then
    docker tag cyberway/cyberway.cdt:${IMAGETAG} cyberway/cyberway.cdt:latest
    docker push cyberway/cyberway.cdt:latest
fi
