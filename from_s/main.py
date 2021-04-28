
import numpy

class Color:

    def __init__(self, red: numpy.uint8 = 0, green: numpy.uint8 = 0, blue: numpy.uint8 = 0):

        self.red = red
        self.green = green
        self.blue = blue

class Alpha:
    
    def __init__(self, alpha_val: numpy.uint8):
        self.data = alpha_val


class SomeClass:
    def __init__(self, data_val):
        self.data = data_val

def plot(x, y, *args):

    for prop in args:
        if type(prop) not in [Alpha, Color]:
            raise Exception(f"Invalid propety {prop}")
    print("hejsan")

if __name__ == "__main__":

    plot(1, 2, Color(22, 33, 44), Alpha(255))
