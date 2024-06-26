{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem
    (
      system: let
        pkgs = import nixpkgs {inherit system;};
        devPackages = with pkgs; [
          gcc
          gnumake
          cmake
          ninja
          llvm_17
          zlib
          libffi
          libedit
          libz
          ncurses
          libxml2
          libpfm
          curl
        ];
      in {
        devShell = pkgs.mkShell {
          packages = devPackages;
        };

        packages.default = pkgs.stdenv.mkDerivation {
          name = "chainsaw-lang";
          src = ./.;
          buildInputs = devPackages;
          configurePhase = ''
            mkdir build
            cmake -S . -B build --install-prefix $out
          '';
          buildPhase = ''
            cmake --build build --target csaw --config Debug
          '';
          installPhase = ''
            cmake --build build --target install --config Debug
          '';
          meta = {
            description = "Chainsaw Programming Language";
          };
        };
      }
    );
}