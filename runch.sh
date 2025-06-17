#!/bin/bash

# material.yamlファイルのパス
material_yaml_path="/home/ros2_louis/ros2_ws/src/choreonoid/share/default/materials.yaml"

# 引数として渡された値（数値）を取得
new_friction_value=$2

# GroundとDefaultの組み合わせのfrictionを更新する
if [ -f "$material_yaml_path" ]; then
    # 'materials: [ Ground, Default ]'を含む行のfrictionを変更
    sed -i "/materials: \[ Ground, Default \]/, /friction:/s/friction: .*/friction: $new_friction_value/" "$material_yaml_path"
    echo "GroundとDefaultのfrictionを$new_friction_valueに設定しました。"
else
    echo "material.yamlが見つかりませんでした。"
fi

# choreonoidのパス
choreonoid_path="./build/choreonoid/bin/choreonoid"

# cnoidプロジェクトの指定
cnoid_project="nrs_simulate_$1.cnoid"

# choreonoidが存在するかを確認
if [ -x "$choreonoid_path" ]; then
    # choreonoidを実行
    $choreonoid_path --project $cnoid_project &
else
    echo "choreonoidが見つかりませんでした。"
fi
