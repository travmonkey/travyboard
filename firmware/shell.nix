{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    llvmPackages_12.libcxxClang
    cmake
    picotool
  ];

  shellHook = ''
    export PICO_TOOLCHAIN_PATH=${pkgs.gcc-arm-embedded}/bin
    export PICO_SDK_PATH=/home/travis/repos/pico-sdk/
  '';
}
