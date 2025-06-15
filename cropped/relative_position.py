import csv
import cv2

def calc_relative_position(min_x, min_y, max_x, max_y, img_width, img_height):
    # 相対位置
    rel_x = min_x / img_width
    rel_y = min_y / img_height

    # 相対サイズ
    rel_w = (max_x - min_x) / img_width
    rel_h = (max_y - min_y) / img_height

    # 面積比
    area_ratio = rel_w * rel_h

    return rel_x, rel_y, rel_w, rel_h, area_ratio
    

def print_data(path):
    data = []

    with open(path, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            # 数値変換が必要な項目だけintに変換
            name = row["name"]
            width = int(row["width"])
            height = int(row["height"])
            min_min_x = int(row["min_min_x"])
            min_max_x = int(row["min_max_x"])
            min_min_y = int(row["min_min_y"])
            min_max_y = int(row["min_max_y"])
            max_min_x = int(row["max_min_x"])
            max_max_x = int(row["max_max_x"])
            max_min_y = int(row["max_min_y"])
            max_max_y = int(row["max_max_y"])

    rel_x,rel_y,rel_w,rel_h,area_ratio = calc_relative_position(min_min_x, min_min_y, min_max_x, min_max_y, width, height)
    print("min box: ")
    print("rel_x = "+str(rel_x))
    print("rel_y = "+str(rel_y))
    print("rel_w = "+str(rel_w))
    print("rel_h = "+str(rel_h))
    print("area ratio = "+str(area_ratio))

    rel_x,rel_y,rel_w,rel_h,area_ratio = calc_relative_position(max_min_x, max_min_y, max_max_x, max_max_y, width, height)
    print("max box: ")
    print("rel_x = "+str(rel_x))
    print("rel_y = "+str(rel_y))
    print("rel_w = "+str(rel_w))
    print("rel_h = "+str(rel_h))
    print("area ratio = "+str(area_ratio))

if __name__ == '__main__':
    for i in range(8):
        path = "pic_"+str(i+1)+".csv"
        print(path)
        print_data(path)
        