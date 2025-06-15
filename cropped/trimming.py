import cv2
import csv
def trimming(path):
    
    with open(path, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            # 数値変換が必要な項目だけintに変換
            name = row["name"]
            width = int(row["width"])
            height = int(row["height"])
            min_left = int(row["min_min_x"])
            min_right = int(row["min_max_x"])
            min_top = int(row["min_min_y"])
            min_buttom = int(row["min_max_y"])
            max_left = int(row["max_min_x"])
            max_right = int(row["max_max_x"])
            max_top = int(row["max_min_y"])
            max_buttom = int(row["max_max_y"])

    img = cv2.imread(name)

    tri_min = img[min_top : min_buttom, min_left : min_right]
    tri_max = img[max_top : max_buttom, max_left : max_right]

    return tri_min, tri_max

if __name__ == '__main__':
    for i in range(8):
        csv_path = "pic_"+str(i+1)+".csv"

        min, max = trimming(csv_path)

        save_name_min = "image_min_pic_"+str(i+1)+".jpg"
        save_name_max = "image_max_pic_"+str(i+1)+".jpg"

        cv2.imwrite(save_name_min, min)
        cv2.imwrite(save_name_max, max)