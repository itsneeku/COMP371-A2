{
  inputs.utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        projectName = "OpenGL_Project";

        buildDeps = with pkgs; [ cmake glfw glm glew ];
        runtimeDeps = with pkgs; [ ];
        devDeps = with pkgs; [ cmake python312Packages.glad ];
        allPackages =
          pkgs.lib.removeDuplicates (buildDeps ++ runtimeDeps ++ devDeps);
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          name = projectName;
          src = ./.;
          nativeBuildInputs = buildDeps;
          buildInputs = runtimeDeps;
          cmakeFlags = [ "-DPROJECT_NAME=${projectName}" ];
        };

        devShells.default = pkgs.mkShell {
          packages = devDeps;
          shellHook = ''
            echo "Entering dev shell for ${projectName}..."

            # Create symbolic links to buildDeps in devDeps
            rm -r "./includeNixStore" &> /dev/null
            mkdir -p "./includeNixStore"
            for pkg in ${toString buildDeps}; do
              if [ -d "$pkg/include" ]; then
                ln -sf "$pkg/include" "./includeNixStore/$(basename $pkg)"
              else
                ln -sf "$pkg" "./includeNixStore/$(basename $pkg)"
              fi
            done

            # Generate glad headers if not present
            if [ ! -d "./include/glad" ]; then
              glad --api gl= --generator c --profile core --out-path .
            fi
          '';
        };
      });
}
