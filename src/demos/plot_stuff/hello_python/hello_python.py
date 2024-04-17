import duoplot
import numpy as np


def main():
    duoplot.set_current_element("hello_python")
    duoplot.clear_view()
    duoplot.set_title("Hello from Python!")

    x = np.linspace(-1.5, 1.5, 100, dtype=np.float32)
    y = np.linspace(-1.5, 1.5, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r * 10.0) / (r * 10.0)

    duoplot.view(-50, 18)
    duoplot.surf(x, y, z)


"""
To run this, the warning printouts for the communication should be commented out

# Make sure paths to shader, images etc are correct so that duoplot can be executed from anywhere

# Pre commands
bash
PS1='> '
cd /Users/danielpi/work/dvs
cd src/build
make hello_c hello_cpp -j6
cd ../..

cp src/demos/plot_stuff/hello_python/hello_python.py hello_python.py
cp src/build/demos/plot_stuff/hello_cpp hello_cpp
cp src/build/demos/plot_stuff/hello_c hello_c

# From /Users/danielpi/work/dvs:
myCommand & echo $!
cd src/build && ./main_application/duoplot &
cd ../..

python3 hello_python.py
./hello_cpp
./hello_c

# Post commands
rm -f ./hello_python.py
rm -f ./hello_cpp
rm -f ./hello_c

"""

if __name__ == "__main__":
    main()
