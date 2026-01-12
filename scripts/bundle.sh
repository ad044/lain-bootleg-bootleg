#!/bin/bash

# Usage: ./scripts/bundle.sh <build_dir> <project_name>

BUILD_DIR="$1"
PROJECT_NAME="$2"
APP_BUNDLE="$BUILD_DIR/$PROJECT_NAME.app"
FRAMEWORKS_DIR="$APP_BUNDLE/Contents/Frameworks"
MACOS_DIR="$APP_BUNDLE/Contents/MacOS"
EXE_PATH="$MACOS_DIR/$PROJECT_NAME"

# Detect architecture to set Homebrew prefix
ARCH=$(uname -m)
if [ "$ARCH" == "arm64" ]; then
    HB_PREFIX="/opt/homebrew"
else
    HB_PREFIX="/usr/local"
fi

echo "Bundling $PROJECT_NAME for $ARCH..."

# Run dylibbundler to collect standard dependencies
mkdir -p "$FRAMEWORKS_DIR"
dylibbundler -of -cd -b -x "$EXE_PATH" -d "$FRAMEWORKS_DIR" -p "@executable_path/../Frameworks/" > /dev/null 2>&1 || true

# Manual Python Framework handling
if [ ! -f "$FRAMEWORKS_DIR/Python" ]; then
    PY_SRC=$(find "$HB_PREFIX" -name Python 2>/dev/null | grep "Python.framework/Versions/3.*/Python$" | grep -v "Resources/Python.app" | sort -r | head -n 1)

    if [ -n "$PY_SRC" ]; then
        FILE_TYPE=$(file "$PY_SRC")
        if [[ "$FILE_TYPE" == *"executable"* ]] && [[ "$FILE_TYPE" != *"dynamically linked shared library"* ]]; then
             PY_SRC=$(find "$(dirname "$PY_SRC")/../.." -name "libpython3.*.dylib" | head -n 1)
        fi
    fi

    if [ -n "$PY_SRC" ]; then
        cp "$PY_SRC" "$FRAMEWORKS_DIR/Python"
        chmod +w "$FRAMEWORKS_DIR/Python"
        install_name_tool -id "@loader_path/Python" "$FRAMEWORKS_DIR/Python"
    fi
fi

# Clean up library paths in Frameworks
find "$FRAMEWORKS_DIR" -type f | while read LIB; do
    [ -f "$LIB" ] || continue
    chmod +w "$LIB"
    LIB_NAME=$(basename "$LIB")

    # Remove duplicate RPATHs if present
    install_name_tool -delete_rpath "@executable_path/../Frameworks/" "$LIB" 2>/dev/null
    install_name_tool -id "@loader_path/$LIB_NAME" "$LIB" 2>/dev/null

    # Relink Homebrew dependencies to @loader_path
    otool -L "$LIB" | grep -E "$HB_PREFIX|Python.framework" | awk '{print $1}' | while read DEP; do
        DEP_NAME=$(basename "$DEP")
        if [[ "$DEP" == *"Python"* ]]; then
            install_name_tool -change "$DEP" "@loader_path/Python" "$LIB"
        else
            install_name_tool -change "$DEP" "@loader_path/$DEP_NAME" "$LIB"
        fi
    done
done

# Fix main executable paths
install_name_tool -add_rpath "@executable_path/../Frameworks/" "$EXE_PATH" 2>/dev/null

otool -L "$EXE_PATH" | grep -E "$HB_PREFIX|Python.framework" | awk '{print $1}' | while read DEP; do
    DEP_NAME=$(basename "$DEP")
    if [[ "$DEP" == *"Python"* ]]; then
        install_name_tool -change "$DEP" "@executable_path/../Frameworks/Python" "$EXE_PATH"
    else
        install_name_tool -change "$DEP" "@executable_path/../Frameworks/$DEP_NAME" "$EXE_PATH"
    fi
done

# Sign the bundle
codesign --force --sign - --deep "$APP_BUNDLE"