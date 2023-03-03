# SPDX-FileCopyrightText: Copyright (c) 2023 k-mana
# SPDX-License-Identifier: MIT
 
From ubuntu:22.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    ninja-build \
    python3 \
    rake \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /work
