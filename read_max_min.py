import cv2
import pytesseract
import sys

def read_max_min(image_path):
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cv2.imshow("normal",gray)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    _, thresh_img = cv2.threshold(gray, 190, 255, cv2.THRESH_BINARY)
    # cv2.imshow("normal",thresh_img)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    config = "--psm 11 -c tessedit_char_whitelist=0123456789.-"
    value = pytesseract.image_to_string(gray, lang='eng', config=config)
    print(value)

if __name__ ==  '__main__':
    args = sys.argv
    if 2 <= len(args):
        path = "cropped/"+args[1]
        print("read:"+path)
        read_max_min(path)
    else:
        print("No")
