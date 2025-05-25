EXECUTABLE = raytrace

CC = g++
FLAGS = -g -Wall --std=c++17

INCLUDE_PATHS = -I /opt/homebrew/include
LIBRARY_PATHS = -L /opt/homebrew/lib
LIBRARIES = -lSDL3

SOURCE_FILES = \
	main.cpp \
	Ray.cpp \
	Sphere.cpp \
	HittableList.cpp \
	Interval.cpp \
	Camera.cpp

$(EXECUTABLE):
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(SOURCE_FILES) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBRARIES)

clean:
	rm -rf $(EXECUTABLE) *.dSYM
