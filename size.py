import cv2
import csv
import os

def crop_from_csv(csv_path, image_dir):
    with open(csv_path, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for i, row in enumerate(reader):
            if i == 10:  # 0-based indexなので11行目はi=10
                print("11行目のデータ:", row)
       
                if len(row) < 4:
                    print("不正な行:", row)
                    continue

                filename, x1, y1, r = row[0], int(row[1]), int(row[2]), int(row[3])
                image_path = os.path.join(image_dir, filename)
                img = cv2.imread(image_path)

                if img is None:
                    print(f"画像が読み込めません: {filename}")
                    continue

                h, w = img.shape[:2]
                x_start = max(x1 - r, 0)
                y_start = max(y1 - r, 0)
                x_end = min(x1 + r, w)
                y_end = min(y1 + r, h)

                cropped = img[y_start:y_end, x_start:x_end]

                # 表示
                cv2.imshow("Cropped", cropped)
                cv2.waitKey(0)
                cv2.destroyAllWindows()

                # 保存も可能（例: cropped_image_0.jpg）
                save_path = "data.jpg"
                cv2.imwrite(save_path, cropped)
                print(save_path)

                break  # 最初の1行だけ処理するのでbreak

# 使用例
crop_from_csv("build/result.csv", "results/")
# import csv

# def read_11th_line(csv_path):
#     with open(csv_path, newline='') as csvfile:
#         reader = csv.reader(csvfile)
#         for i, row in enumerate(reader):
#             if i == 10:  # 0-based indexなので11行目はi=10
#                 print("11行目のデータ:", row)
#                 return row
#     print("11行目のデータは存在しません。")
#     return None

# # 使い方
# row = read_11th_line("build/result.csv")
