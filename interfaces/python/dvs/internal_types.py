
class Color:

    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b
    
    @staticmethod
    def RED():
        return Color(255, 0, 0)