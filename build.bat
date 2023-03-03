@ECHO OFF

REM SPDX-FileCopyrightText: Copyright (c) 2023 k-mana
REM SPDX-License-Identifier: MIT

SETLOCAL

where docker >NUL
IF %ERRORLEVEL% NEQ 0 (
  ECHO "Please install docker."
  EXIT /B
)

SET build_docker=false
IF "%1" == "docker" SET build_docker=true
FOR /F "usebackq" %%a IN (`docker images --format "{{ .ID }}" pico-mruby-dev`) DO SET id=%%a
IF "%id%" == "" SET build_docker=true
IF "%build_docker%" == "true" docker build --file Dockerfile --tag pico-mruby-dev .

docker run --mount type=bind,source="%CD%",target=/work pico-mruby-dev /bin/bash -c "cmake -G Ninja -B build && cmake --build build"
