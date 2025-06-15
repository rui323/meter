import cv2
import sys
import numpy as np

def calc_angle(theta_deg, min_pressure, max_pressure):
    MAX_ANGLE = -132
    MIN_ANGLE = 132

    if MAX_ANGLE < theta_deg < MIN_ANGLE:
        pressure = min_pressure + (max_pressure - min_pressure) / (MAX_ANGLE - MIN_ANGLE) * (theta_deg - MIN_ANGLE)
    elif theta_deg <= MAX_ANGLE: # 最大値角度-132を下回ったとき
        pressure = max_pressure
    elif theta_deg >= MIN_ANGLE:
        pressure = min_pressure
        
    return pressure

def point_line_distance(x1, y1, x2, y2, cx, cy):
    # 点と線分の距離を返す（ユークリッド距離）
    A = np.array([x1, y1])
    B = np.array([x2, y2])
    P = np.array([cx, cy])

    AB = B - A
    AP = P - A
    t = np.clip(np.dot(AP, AB) / np.dot(AB, AB), 0, 1)
    closest = A + t * AB
    return np.linalg.norm(P - closest)

def detect_line(path):
    image = cv2.imread(path)
    h,w,_ = image.shape
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # print(h,w)
    # 座標を指定してその画素値をもとに二値化ーーーーーーーー
    x = int(1/2 * w)+10
    y = int(1/2 * h)+10
    # print(x,y)
    value = gray[y,x]
    out = cv2.circle(image, (x,y), 2, (255,0,0))
    # cv2.imshow("circle", out)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    # _, bit = cv2.threshold(gray, value, 255, cv2.THRESH_BINARY)
    # ーーーーーーーーーーーーーーーーーー
    # ヒストグラムから頻度を見て発見
    # ヒストグラムを計算（256階調）
    hist = cv2.calcHist([gray], [0], None, [256], [0,256])

    # 最も頻度が高い画素値を取得
    mode_value = np.argmax(hist)
  
    # _, bit = cv2.threshold(gray, mode_value, 255, cv2.THRESH_BINARY)
    # otsu
    _, bit = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    
    # cv2.imshow("gray", gray)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    # cv2.imshow("bit", bit)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    CROP_RADIUS = w // 2 -  (w * 50 // 480)
    center_point = (w // 2, h // 2)
    # 円形マスク（白＝255の領域だけ残す）
    mask = np.ones((h, w), dtype=np.uint8)
    cv2.circle(mask, center_point, CROP_RADIUS, 255, cv2.FILLED)
    
    # 背景を白に初期化
    output = np.ones((h, w), dtype=np.uint8) * 255

    # 円の中だけbitをコピー（bitwise_andではなく、maskで条件代入）
    output[mask == 255] = bit[mask == 255]

    # 二値画像をマスクでくり抜く
    # cropped = cv2.bitwise_and(bit, bit, mask=mask)
    # cv2.imshow("crop", output)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    reverse = cv2.bitwise_not(output)
    cv2.imshow("rev", reverse)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    # センター座標
    cx = center_point[0]
    cy = center_point[1]
    # 中心からの座標
    dist = w
    center = None
    # 円検出
    circles = cv2.HoughCircles(reverse,cv2.HOUGH_GRADIENT,1,20,param1=120,param2=15,minRadius=10,maxRadius=30)
    circles = np.uint16(np.around(circles))
    for i in circles[0,:]:
        dist_temp = np.sqrt((int(i[0])-int(cx))**2 + (int(i[1])-int(cy))**2)
        if dist >= dist_temp:
            dist = dist_temp
            center = i

    # 外側の円を描く
    cv2.circle(image,(center[0],center[1]),center[2],(0,0,255),2)
    # 中心の円を描く
    cv2.circle(image,(center[0],center[1]),2,(0,255,),2)
    cv2.imshow("rev_circle", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    
    

# 線検出---------------------------------------
    rho=1
    theta=np.pi/180
    threshold = w * 100 // 480
    minLineLength = w * 200 // 480 
    maxLineGap = w * 50 // 480

    print(threshold,minLineLength,maxLineGap)
    lines = cv2.HoughLinesP(reverse, rho, theta, threshold, minLineLength, maxLineGap)

    # 見つけられなかったときの確認用
    # for line in lines:
    #     x1, y1, x2, y2 = line[0]

    #     line_img = cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 3)
    # cv2.imshow("test", line_img)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    

    # メーターの中心centerを通過するもの残す
    filtered_lines = []
    for line in lines:
        x1, y1, x2, y2 = line[0]
        dist = point_line_distance(x1, y1, x2, y2, center[0], center[1])
        if dist < 20 * w //480:  # 5ピクセル以内を通れば「通ってる」と判定
            filtered_lines.append((x1, y1, x2, y2))

    # 確認用描画
    # for x1, y1, x2, y2 in filtered_lines:
    #     cv2.line(image, (x1, y1), (x2, y2), (255, 255, 255), 2)

    
    print(len(lines))
    print(len(filtered_lines))
    # print(filtered_lines)


    # cv2.imshow("lines", red_line_img)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

    print(w,h)
    # 一番長い線を採用=長針
    max_length = 0
    for line in filtered_lines:
    # line1: 任意の線（x1, y1, x2, y2）
        x1, y1, x2, y2 = line
        length = np.sqrt((x1-x2)**2 + (y1-y2)**2)
        if max_length <= length:
            tuple_data = line
            max_length = length

    x1, y1, x2, y2 = tuple_data
    print(x1,y1,x2,y2)
    # 中心から外側に向かうベクトル作成　始点と終点それぞれ中心からの距離を計算し、長い方が外側と判定
    if abs(center[0]-x1) >= abs(center[0]-x2):
        v1 = np.array([x1 - x2, y1 - y2])
    else:
        v1 = np.array([x2 - x1, y2 - y1])

    # line2: 画面中央から真上への基準線（ここでは (0, -1) とする）　画像が左上(0,0)だから
    v2 = np.array([0, -1])  # 上向きが0度
    print(v1,v2)
    # 方向付きの角度を atan2 で取得　時計回り：負　反時計回り：正
    angle_rad = np.arctan2(v1[0]*v2[1] - v1[1]*v2[0], v1[0]*v2[0] + v1[1]*v2[1])
    angle_deg = np.degrees(angle_rad)

    print(f"角度: {angle_deg:.2f}°")
    print(angle_rad,angle_deg)


    # 赤線を引く　採用した線(長針)をプロット
    red_line_img = cv2.line(image, (x1,y1), (x2,y2), (0,0,255), 3)
    cv2.imshow("lines", red_line_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    # 圧力値計算関数　角度(deg)、最小値、最大値
    angle = calc_angle(angle_deg,-0.1,0.25)
    print(angle)
    



if __name__ == '__main__':
    args = sys.argv
    path = "cropped/"+args[1]
    print("read:"+path)
    detect_line(path)