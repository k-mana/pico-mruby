# SPDX-FileCopyrightText: Copyright (c) 2023 k-mana
# SPDX-License-Identifier: MIT

if ( -not (Get-Command docker) )
{
  Write-Output "Please install docker."
  exit 1
}

$id = docker images --format "{{ .ID }}" pico-mruby-dev
if ( $null -eq $id -or "docker" -eq $Args[0] )
{
  docker build --file Dockerfile --tag pico-mruby-dev .
}

docker run --mount type=bind,source="$(pwd)",target=/work pico-mruby-dev /bin/bash -c "cmake -G Ninja -B build && cmake --build build"
