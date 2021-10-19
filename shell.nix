let
  webots_overlay = import /etc/nixos/overlays/packages.nix;
in { pkgs ? import <nixpkgs> { overlays = [ webots_overlay ]; } }: with pkgs; mkShell {
  opencvGtk = opencv.override (old : { enableGtk2 = true; });
  buildInputs = [
    cmake
    bluez
    gcc
    pkg-config
    webots
    (opencv.override (old : { enableGtk2 = true; }))
    clang-tools
  ];

  WEBOTS_HOME = "${webots}/local/webots";
}
