#!/bin/bash
set -euo pipefail

REVISION=$(git rev-parse HEAD)
MASTER_REVISION=$(git rev-parse origin/master)
DEVELOP_REVISION=$(git rev-parse origin/develop)

docker images

docker login -u=$DHUBU -p=$DHUBP

if [[ "${REVISION}" == "${MASTER_REVISION}" ]]; then
    TAG=stable
elif [[ "${REVISION}" == "${DEVELOP_REVISION}" ]]; then
    TAG=latest
else 
    TAG=${BUILDKITE_BRANCH}
fi

docker tag cyberway/cyberway.cdt:${REVISION} cyberway/cyberway.cdt:${TAG}
docker push cyberway/cyberway.cdt:${TAG}
