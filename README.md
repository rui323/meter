# meter
## 画像
 - cropped/
    - cropped_1.jpg：未加工のくり抜かれた画像
    - cropped_1_n.jpg：回転させた画像
    - image_max_pic_1.jpg：最大値の画像
    - image_min_pic_1.jpg：最小値の画像
## プログラム
### point.py
 - 画像を指定して、クリック操作で座標を記録
#### 実行コード
~~~bash!
python3 point.py 1
~~~
### relative_position.py
 - 記録されているcsvデータを参照、画像のどの位置にあるか出力
#### 実行コード
~~~bash!
python3 relative_position.py 1
~~~
### rotate.py
 - 画像を指定して回転させる
#### 実行コード
~~~bash!
python3 rotate.py
~~~
### trimming.py
 - 記録された座標をもとに画像から切り抜く
#### 実行コード
~~~bash!
python3 trimming.py 
~~~
### read_max_min.py
 - 画像からOCRで数値を読み取る
#### 実行コード
~~~bash!
python3 read_max_min.py image_max_pic_1.jpg
python3 read_max_min.py image_min_pic_1.jpg
~~~
### line_detect.py
 - 画像から長針を見つけ圧力値を報告
 - 最小最大値は手動で設定
#### 実行コード
~~~bash!
python3 line_detect.py cropped_1_n.jpg
~~~

## C++
### detect.cpp tesseract.cpp
 - 文字認識プログラム
### line_detect.cpp
 - メーターの外淵が誤った線検出をしそうなのでHoughCircleで削除
 - 二値化処理
 - 線検出
 - 画像の中央を通っているか
 - 長針、短針の判別
 - 真上に向かうベクトルと長針のベクトルのなす角度を求める
 - その角度にから値を計算する