#!/bin/bash
set -euo pipefail

REVISION=$(git rev-parse HEAD)

docker images

docker login -u=$DHUBU -p=$DHUBP

if [[ ${BUILDKITE_BRANCH} == "master" ]]; then
    TAG=stable
elif [[ ${BUILDKITE_BRANCH} == "develop" ]]; then
    TAG=latest
else
    TAG=${BUILDKITE_BRANCH}
fi

docker pull cyberway/cyberway.cdt:${REVISION}
docker tag cyberway/cyberway.cdt:${REVISION} cyberway/cyberway.cdt:${TAG}
docker push cyberway/cyberway.cdt:${TAG}
