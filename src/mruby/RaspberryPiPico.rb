MRuby::CrossBuild.new('RaspberryPiPico') do |conf|
  conf.toolchain :gcc
  conf.cc do |cc|
    cc.command = "arm-none-eabi-gcc"
    cc.flags = "-mcpu=cortex-m0plus -mthumb -O3 -DNDEBUG -ffunction-sections -fdata-sections"
    cc.defines << %w(MRB_BYTECODE_DECODE_OPTION)
  end

  conf.archiver do |archiver|
    archiver.command = "arm-none-eabi-ar"
  end

  conf.gem :core => "mruby-array-ext"
  conf.gem :core => "mruby-catch"
  conf.gem :core => "mruby-class-ext"
  conf.gem :core => "mruby-compar-ext"
  conf.gem :core => "mruby-complex"
  conf.gem :core => "mruby-enum-chain"
  conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-enum-lazy"
  conf.gem :core => "mruby-enumerator"
  conf.gem :core => "mruby-error"
  conf.gem :core => "mruby-eval"
  conf.gem :core => "mruby-exit"
  conf.gem :core => "mruby-fiber"
  conf.gem :core => "mruby-hash-ext"
  conf.gem :core => "mruby-kernel-ext"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-metaprog"
  conf.gem :core => "mruby-method"
  conf.gem :core => "mruby-numeric-ext"
  conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-objectspace"
  conf.gem :core => "mruby-os-memsize"
  conf.gem :core => "mruby-pack"
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-proc-ext"
  conf.gem :core => "mruby-random"
  conf.gem :core => "mruby-range-ext"
  conf.gem :core => "mruby-rational"
  conf.gem :core => "mruby-sprintf"
  conf.gem :core => "mruby-string-ext"
  conf.gem :core => "mruby-struct"
  conf.gem :core => "mruby-symbol-ext"
  conf.gem :core => "mruby-time"
  conf.gem :core => "mruby-toplevel-ext"

  
#  conf.gem :core => "mruby-bin-config"
#  conf.gem :core => "mruby-bin-debugger"
#  conf.gem :core => "mruby-bin-mirb"
#  conf.gem :core => "mruby-bin-mrbc"
#  conf.gem :core => "mruby-bin-mruby"
#  conf.gem :core => "mruby-bin-strip"
#  conf.gem :core => "mruby-compiler"
#  conf.gem :core => "mruby-io"
#  conf.gem :core => "mruby-sleep"
#  conf.gem :core => "mruby-socket"
#  conf.gem :core => "mruby-test"

end
