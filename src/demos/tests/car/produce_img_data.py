import cv2

if __name__ == "__main__":
    
    img = cv2.imread('src/demos/tests/car/car.png', cv2.IMREAD_GRAYSCALE)
    img_gray = cv2.resize(img, None, fx=0.5, fy=0.5)

    with open("src/demos/tests/car/car.bin", "wb") as newFile:

        img_byte_array = bytearray(img_gray)
        newFile.write(img_byte_array)
