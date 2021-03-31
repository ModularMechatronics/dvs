#!/bin/bash

cd $(git rev-parse --show-toplevel)

docker build -f ./docker/Dockerfile.dev -t dvs_image_dev .