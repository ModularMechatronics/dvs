#!/bin/bash

cd $(git rev-parse --show-toplevel)

docker build -f ./docker/Dockerfile.prod -t dvs_image_prod .