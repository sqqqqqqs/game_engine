game_engine_linux:
	clang++ -std=c++17 src/*.cpp box2d/collision/*.cpp box2d/common/*.cpp box2d/dynamics/*.cpp box2d/rope/*.cpp -llua5.4 -I./ -I./External/glm -I./External/rapidjson -I./External/ -I./SDL2/ -I./SDL2_image/  -I./SDL2_ttf -I./SDL2_mixer -I./Lua -I./LuaBridge -I./LuaBridge/details -I./box2d/ -I./box2d/dynamics/ -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer  -O3 -o game_engine_linux

#clean:
#	rm -f *.o game_engine_linux
