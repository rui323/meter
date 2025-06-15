import cv2

list_90_right = ["1"]
list_90_left = ["2","3","4","5","6","8"]

for i in list_90_left:
    path = "cropped_"+i
    img = cv2.imread(path+".jpg")
    img_n = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)
    path_n = path + "_n.jpg"
    cv2.imwrite(path_n,img_n)

for i in list_90_right:
    path = "cropped_"+i
    img = cv2.imread(path+".jpg")
    img_n = cv2.rotate(img, cv2.ROTATE_90_COUNTERCLOCKWISE)
    path_n = path + "_n.jpg"
    cv2.imwrite(path_n,img_n)