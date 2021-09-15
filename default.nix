let
  webots_overlay = import /etc/nixos/overlays/packages.nix;
in { pkgs ? import <nixpkgs> { overlays = [ webots_overlay ]; } }: with pkgs; mkShell {
  opencvGtk = opencv.override (old : { enableGtk2 = true; });
  buildInputs = [
    cmake
    gcc
    pkg-config
    webots
    (opencv.override (old : { enableGtk2 = true; }))
  ];

  WEBOTS_HOME = "${webots}/local/webots";
}
