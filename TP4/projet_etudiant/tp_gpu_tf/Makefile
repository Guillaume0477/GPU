CC = g++
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11

obj = glhelper.o main.o camera.o

opengl: $(obj)
	$(CC) $^ -o $@ -lglut -lGLEW -lGL -L/usr/lib/nvidia-443 -pthread

%.o: %.c
	$(CC)  -o $@ -c $<

clean:
	rm -f *.o

mrproper: clean
	rm opengl
