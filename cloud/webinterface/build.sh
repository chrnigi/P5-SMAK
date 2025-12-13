#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)
echo $SCRIPT_DIR
if ! [ -d "$SCRIPT_DIR/chessboardjs" ]; then
    mkdir -p "$SCRIPT_DIR/chessboardjs"
    curl -L "https://chessboardjs.com/releases/chessboardjs-1.0.0.zip" --output "$SCRIPT_DIR/chessboardjs.zip"
    unzip "$SCRIPT_DIR/chessboardjs.zip" -d "$SCRIPT_DIR/chessboardjs"
fi

if ! [ -L "$SCRIPT_DIR/img" ]; then
    ln -s "$SCRIPT_DIR/chessboardjs/img" "$SCRIPT_DIR" 
fi

env tsc "$SCRIPT_DIR/geteval.ts"
