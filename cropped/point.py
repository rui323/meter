import cv2
import numpy as np
import sys
import time 
import csv

count = 0
list_point = []

min_flag = False
max_flag = False
def mousePoints(event, x, y, flags, params):
    global count
    if event == cv2.EVENT_LBUTTONDOWN:
        # print(x,y)
        temp = [x,y]
        list_point.append(temp)
        print(count)
        count = count + 1



def point(image_path):
    global min_flag, max_flag, count, list_point
    count = 0
    list_point = [] 
    min_flag = False
    max_flag = False

    img = cv2.imread(image_path)
    print("read: " + image_path)

    cv2.namedWindow('Original image')
    cv2.setMouseCallback('Original image', mousePoints, list_point)

    while True:
        display = img.copy()
        cv2.imshow('Original image', display)
        cv2.waitKey(1)
        
        if count == 4 and not(min_flag):
            print("min pos")
            min_flag = True
            
        if count == 8 and not(max_flag):
            print("max pos")
            max_flag = True
            cv2.destroyAllWindows()
            break

    h,w,_ = img.shape 
    min_points = list_point[0:4]
    max_points = list_point[4:8]
   
    min_points = sum(min_points,[])
    max_points = sum(max_points,[])

    # print(min_points)
    # print(max_points)
    min_min_x = min(min(min_points[0],min_points[2]),min(min_points[4],min_points[6]))
    max_min_x = max(max(min_points[0],min_points[2]),max(min_points[4],min_points[6]))
    min_min_y = min(min(min_points[1],min_points[3]),min(min_points[5],min_points[7]))
    max_min_y = max(max(min_points[1],min_points[3]),max(min_points[5],min_points[7]))
    min_pos = [min_min_x, max_min_x, min_min_y, max_min_y]

    min_max_x = min(min(max_points[0],max_points[2]),min(max_points[4],max_points[6]))
    max_max_x = max(max(max_points[0],max_points[2]),max(max_points[4],max_points[6]))
    min_max_y = min(min(max_points[1],max_points[3]),min(max_points[5],max_points[7]))
    max_max_y = max(max(max_points[1],max_points[3]),max(max_points[5],max_points[7]))
    max_pos = [min_max_x, max_max_x, min_max_y, max_max_y]
    return h,w,list_point,min_pos,max_pos
    


if __name__ == '__main__':
    args = sys.argv
    if 2 <= len(args):
        path = "cropped_"+args[1]+"_n.jpg"
        
    else:
        print("引数にファイル名をいれてください")

    header = ["name","width","height","min_min_x","min_max_x","min_min_y","min_max_y","max_min_x","max_max_x","max_min_y","max_max_y"]
    file = "pic_"+args[1]+".csv"
    with open(file,"w", newline='') as f:
        writer = csv.writer(f)
        writer.writerow(header)
        h,w,points,min_pos,max_pos = point(path)

        rows = [path, w, h, min_pos[0], min_pos[1], min_pos[2], min_pos[3], max_pos[0], max_pos[1], max_pos[2], max_pos[3]]
        writer.writerow(rows)
       
