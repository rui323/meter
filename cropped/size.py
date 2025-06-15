import cv2
import csv

header = ["name","width","height"]
with open("data.csv","w", newline='') as f:
    writer = csv.writer(f)
    writer.writerow(header)
    for i in range(8):
        path = "cropped_"+str(i+1)+"_n.jpg"
        img = cv2.imread(path)
        h,w,_ = img.shape
        rows = [path,w,h]
        writer.writerow(rows)
