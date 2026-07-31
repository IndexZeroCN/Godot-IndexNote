#!/usr/bin/env bash
#
# Build GDExtension for Android
# Usage: ./build_android.sh [debug|release|all]
#
# Prerequisites:
#   - NDK 23.2.8568313 installed in $ANDROID_HOME/ndk/23.2.8568313/
#   - scons installed (pip install scons)

set -e

export ANDROID_HOME="${ANDROID_HOME:-E:/AndroidSDK}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Architectures to build
ARCHS=("arm64" "arm32" "x86_64")
TARGETS=()

case "${1:-all}" in
    debug)
        TARGETS=("template_debug")
        ;;
    release)
        TARGETS=("template_release")
        ;;
    all)
        TARGETS=("template_debug" "template_release")
        ;;
    *)
        echo "Usage: $0 [debug|release|all]"
        exit 1
        ;;
esac

for target in "${TARGETS[@]}"; do
    for arch in "${ARCHS[@]}"; do
        echo "========================================"
        echo "Building: platform=android target=$target arch=$arch"
        echo "========================================"
        scons platform=android target="$target" arch="$arch" ANDROID_HOME="$ANDROID_HOME" -j"$(nproc 2>/dev/null || echo 4)"
    done
done

echo ""
echo "Android build complete!"
echo "Output files in: addons/index-note/bin/"
ls -la addons/index-note/bin/*.android.*.so 2>/dev/null || echo "(No Android .so files found)"
