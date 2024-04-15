import cv2
import numpy as np

if __name__ == "__main__":

    img_file_name = "img.png"

    img = cv2.imread("src/demos/demo_app/tests/duoplot_text/" + img_file_name)
    # img_resized = cv2.resize(img, None, fx=0.5, fy=0.5)
    image_to_use = img

    img_width = image_to_use.shape[1]
    img_height = image_to_use.shape[0]

    new_img = np.transpose(image_to_use, (2, 0, 1))
    # new_img[0] = np.flipud(new_img[0])
    # new_img[1] = np.flipud(new_img[1])
    # new_img[2] = np.flipud(new_img[2])

    with open("src/demos/demo_app/tests/duoplot_text/img.bin", "wb") as new_file:

        full_byte_array = (
            bytearray(np.uint16(img_width).tobytes())
            + bytearray(np.uint16(img_height).tobytes())
            + bytearray(new_img)
        )
        new_file.write(full_byte_array)
