#!/bin/bash

# ルートディレクトリ（videoディレクトリ）
ROOT_DIR="video"

# 指定されたディレクトリ構造を探索
for incident in "$ROOT_DIR"/incident_{15,45,75}; do
    for mu in "$incident"/mu{03,05,10}; do
        for v in "$mu"/v{0.0,0.1,0.01,0.001}; do
            # 画像があるか確認
            if ls "$v"/movie*.png 1> /dev/null 2>&1; then
                echo "Processing: $v"
                # v0.xディレクトリに移動
                cd "$v" || exit
                # ffmpegを使って動画を作成
                ffmpeg -r 30 -i movie%08d.png -r 30 movie.mp4
                # 元のディレクトリに戻る
                cd - > /dev/null
            else
                echo "No PNG files found in: $v"
            fi
        done
    done
done

echo "Processing complete."