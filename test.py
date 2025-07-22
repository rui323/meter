import cv2
import numpy as np

# 1. 黒画像を生成 (512x512)
black_image = np.zeros((512, 512), dtype=np.uint8)

# 2. 横縞模様を生成（白と黒のストライプ、縞の太さは16ピクセル）
striped_image = black_image.copy()
for i in range(0, 512, 16):
    if (i // 16) % 2 == 0:
        striped_image[i:i+16, :] = 255  # 白いストライプ

# 表示用（オプション）
cv2.imshow("Horizontal Stripes", striped_image)
cv2.waitKey(0)

# 3. チェスボードパターンを生成（1マス16ピクセル）
chess_image = black_image.copy()
for y in range(0, 512, 16):
    for x in range(0, 512, 16):
        if (x // 16 + y // 16) % 2 == 0:
            chess_image[y:y+16, x:x+16] = 255  # 白いマス

# 表示用（オプション）
cv2.imshow("Chessboard", chess_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
