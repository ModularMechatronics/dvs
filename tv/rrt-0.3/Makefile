.SUFFIXES: .C .o

C++ = g++

C++FLAGS = -O2 

INC = -I/usr/local/lib/g++-include/ -L/usr/X11/lib/ -I/home/dpihlqui/Downloads/LEDA-6.3-free-fedora-core-8-64-g++-4.1.2/incl/ -I/home/dpihlqui/Downloads/ann_1.1.2/include/

# LIBS = -lW -lP -lG -lL -lX11 -lm 
LIBS = -lX11 -lm /home/dpihlqui/Downloads/LEDA-6.3-free-fedora-core-8-64-g++-4.1.2/libleda.so

OBJ = model.o rrt.o 

.C.o:
	$(C++) $(C++FLAGS) -c $< $(INC) 

rrt: gui.C $(OBJ)
	$(C++) $(C++FLAGS) gui.C $(OBJ) -o rrt $(INC) $(LIBS)

convert: convert.C 
	$(C++) $(C++FLAGS) convert.C -o convert $(INC) $(LIBS)

gifmerge:
	gcc -o gifmerge gifmerge.c

clean:
	-rm *.o rrt
