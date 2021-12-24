# Raspberry Pi Pico用のmruby

## ソースコードの取得
```
$ git clone https://github.com/k-mana/pico-mruby.git --recursive
```
または
```
$ git clone https://github.com/k-mana/pico-mruby.git
$ cd pico-mruby
$ git submodule update --init --recursive
```

## ビルドツールのインストール
```
$ sudo apt install cmake build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib rake
```

## ビルド
```
$ cd pico-mruby
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
### 注記
pico-sdkのissue[#623](https://github.com/raspberrypi/pico-sdk/issues/623)が発生すると動作しません。
その場合は、この[パッチ](src/pico-sdk/pico_stdio.patch)を適用してください。

## アプリケーション
デフォルトはUSBシリアル

[hello_world](src/hello_world)
* build/src/hello_world/hello_world.uf2
* スクリプトまたはバイトコードを埋め込んだサンプル。

[pico_mirb](src/pico_mirb)
- build/src/pico_mirb/pico_mirb.uf2
- REPL(Read-Eval-Print Loop)

[pico_mruby](src/pico_mruby)
* build/src/pico_mruby/pico_mruby.uf2
* USBマスストレージにcode.rbまたはcode.mrbが存在する場合、自動的に実行する。
* 実行する優先順位は以下です。
	1. code.mrb
	2. code.rb
	3. REPL
* USBマスストレージに書き込むと自動的に再起動します。
* code.rbまたはcode.mrbの実行中にシリアル入力をするとREPLモードになります。

-------------------------------------------------------------------------------
# mruby for Raspberry Pi Pico

## Get the source code
```
$ git clone https://github.com/k-mana/pico-mruby.git --recursive
```
or
```
$ git clone https://github.com/k-mana/pico-mruby.git
$ cd pico-mruby
$ git submodule update --init --recursive
```

## Install the build tool
```
$ sudo apt install cmake build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib rake
```

## Build
```
$ cd pico-mruby
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
### Note
It does not work when issue [#623](https://github.com/raspberrypi/pico-sdk/issues/623) of pico-sdk occurs.
In that case, please apply this [patch](src/pico-sdk/pico_stdio.patch).

## Application
Default is USB Serial.

[hello_world](src/hello_world)
* build/src/hello_world/hello_world.uf2
* Sample with embedded script or bytecode

[pico_mirb](src/pico_mirb)
* build/src/pico_mirb/pico_mirb.uf2
* REPL(Read-Eval-Print Loop)

[pico_mruby](src/pico_mruby)
* build/src/pico_mruby/pico_mruby.uf2
* If code.rb or code.mrb exists in the USB mass storage, it will be executed automatically.
* The priorities to execute are as follows:
	1. code.mrb
	2. code.rb
	3. REPL
* Writing to USB mass storage will automatically reboot the system.
* If you make a serial input while code.rb or code.mrb is running, it will enter REPL mode.
