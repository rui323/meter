#!/bin/bash

# ルートディレクトリ
ROOT_DIR="send/video"

# 指定されたディレクトリ構造を探索
for incident in "$ROOT_DIR"/incident_{0,15,30,45,60,75}; do
    for mu in "$incident"/mu{03,05,10}; do
        for v in "$mu"/v{0.0,0.1,0.01,0.001}; do
            # PNGファイルが存在する場合に削除
            if ls "$v"/*.png 1> /dev/null 2>&1; then
                echo "Deleting PNG files in: $v"
                rm "$v"/*.png
            else
                echo "No PNG files found in: $v"
            fi
        done
    done
done

echo "Deletion complete."