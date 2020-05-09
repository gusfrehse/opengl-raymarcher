cxx = g++
src_files = main.cpp
include_dirs = -I.
libs = -lSDL2 -ldl


game: main.o glad.o shader.o
	$(cxx) $^ -o $@ $(libs)

main.o : main.cpp
	$(cxx) -c $^ $(include_dirs)

glad.o : glad.c
	$(cxx) -c $^ $(include_dirs)

shader.o : shader.cpp
	$(cxx) -c $< $(include_dirs)

run: game
	./game
