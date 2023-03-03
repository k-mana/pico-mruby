#!/bin/sh

# SPDX-FileCopyrightText: Copyright (c) 2023 k-mana
# SPDX-License-Identifier: MIT

type docker > /dev/null
if [ $? -ne 0 ]; then
	echo "Please install docker."
	exit 1
fi

id=$(echo `docker images --format "{{ .ID }}" pico-mruby-dev`)
if [ -z "$id" ] || [ "$1" = docker ]; then
	docker build --file Dockerfile --tag pico-mruby-dev .
fi

docker run --user `id -u`:`id -g` --mount type=bind,source="$(pwd)"/,target=/work pico-mruby-dev /bin/bash -c "cmake -G Ninja -B build && cmake --build build"
