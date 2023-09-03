import cv2
from PIL import Image
from PIL import ImageOps
import numpy as np
import sys


def colored_background(r, g, b, text):
    return f'\033[48;2;{r};{g};{b}m{text}\033[0m'

# default data
cap = cv2.VideoCapture(0)
gray = ".,;:?!'()[]{}/|&*+-=#$%@"
gray = ''.join(reversed(gray))

# set value to variable
args_list = sys.argv
args_list.pop(0)

keys = args_list[0::2]
values = args_list[1::2]

if keys == ['-mode']:
    param = {key.replace("-", ''): value for key, value in zip(keys, values)}

    resolution = 5
    spacing = 1
    mode = param['mode']

    while True:
        ret, frame = cap.read()

        # CV2 IMG TO PILLOW IMG
        pil_image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        result = ImageOps.mirror(pil_image)

        # CONST DATA
        SIZE = result.size
        SIZE_L = int(SIZE[0] / resolution)
        SIZE_H = int(SIZE[1] / resolution)

        result = result.resize((SIZE_L, SIZE_H))

        # GET GRAY COLOR WITH RGB AND CONVERT GRAY COLOR TO ASCII
        col = ""

        for y in range(SIZE_H):
            row = ""

            for x in range(SIZE_L):

                    px = result.getpixel((x, y))

                    if mode == "ascii":
                        gray_value = int(0.2989 * px[0] + 0.5870 * px[1] + 0.1140 * px[2])
                        gray_percent = int(gray_value * 100 / 255)

                        gray_char = gray[int((gray_percent/100 * (len(gray)-1)))]
                        result.putpixel((x, y), (gray_value, gray_value, gray_value))
                        row += gray_char + " " * spacing

                    elif mode == "px":
                        spacing = 0
                        row += colored_background(px[0], px[1], px[2], "  ") + (" " * spacing)
                    
                    elif mode == 'rgb':
                        maxIdx = px.index(max(px))

                        if maxIdx == 0:
                            row += colored_background(255, 0, 0, " ") + (" " * spacing)
                        elif maxIdx == 1:
                            row += colored_background(0, 255, 0, " ") + (" " * spacing)
                        else:
                            row += colored_background(0, 0, 255, " ") + (" " * spacing)
                    
                    elif mode == 'rgb2':
                        maxIdx = px.index(max(px))

                        if maxIdx == 0:
                            row += colored_background(px[0], 0, 0, " ") + (" " * spacing)
                        elif maxIdx == 1:
                            row += colored_background(0, px[1], 0, " ") + (" " * spacing)
                        else:
                            row += colored_background(0, 0, px[2], " ") + (" " * spacing)

                    elif mode == 'gray':
                        row += colored_background(px[0], px[0], px[0], "  ") + (" " * spacing)
                    
                    elif mode == 'red':
                        row += colored_background(px[0], 0, 0, "  ") + (" " * spacing)
                    
                    elif mode == 'green':
                        row += colored_background(0, px[1], 0, "  ") + (" " * spacing)
                    
                    elif mode == 'blue':
                        row += colored_background(0, 0, px[2], "  ") + (" " * spacing)

            col += row+"\n"

        print(col)

        # PILLOW IMG TO CV2 IMG
        mirrored_frame = cv2.cvtColor(np.array(result), cv2.COLOR_RGB2BGR)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

else:
    print("[-mode]: ascii, px, rgb, rgb2, gray, red, green, blue")
