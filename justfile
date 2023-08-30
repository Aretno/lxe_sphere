# -*-Makefile-*-

test:
  just run macs/run.mac

build:
  cmake -S src -B build/app
  cmake --build build/app

run N='': build
  #!/usr/bin/env sh
  ./build/app/lxe_sphere {{N}}
  exit $?

view:
  nix run --impure github:guibou/nixGL -- just run

clean:
  rm build -rf
