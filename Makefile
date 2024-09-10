# set the App name
NAME = rpg_side_screen
# set compiler
CC = g++ -std=c++20 -Wno-narrowing
STATIC_LIBS = -static-libstdc++ -static-libgcc
INCLUDES = -Iinclude -Iinclude/sdl -Iinclude/headers -Llib
LIBS = -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gpu -lSDL2_mixer -lole32 -mwindows
DEBUG_LIBS = -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gpu -lSDL2_mixer -lpsapi -lpdh -lole32 -mwindows
# set all the files needed
DEBUG_FOLDER = .build
RELEASE_FOLDER = .release

ICON_DIR = res/icon/icon.res
BIN = bin/*.o
DEBUG_FLAGS = -D_ASSERTS -D_DEBUG -D_IMGUI
GAME_FLAGS = -D_IMGUI -DWIN_WIDTH=400 -DWIN_HEIGHT=200 -DGAME_SCALE=1

bin_dir:
	mkdir bin

imgui_o: $(patsubst src/imgui/%.cpp,bin/%.o,$(wildcard src/imgui/*.cpp))
app_o: $(patsubst src/core/%.cpp,bin/%.o,$(wildcard src/core/*.cpp))
entity_o : $(patsubst src/entity/%.cpp,bin/%.o,$(wildcard src/entity/*.cpp))
renderer_o : $(patsubst src/renderer/%.cpp,bin/%.o,$(wildcard src/renderer/*.cpp))
resources_o : $(patsubst src/res/%.cpp,bin/%.o,$(wildcard src/res/*.cpp))
game_o : $(patsubst src/game/%.cpp,bin/%.o,$(wildcard src/game/*.cpp))
tools_o : $(patsubst src/tools/%.cpp,bin/%.o,$(wildcard src/tools/*.cpp))

# Rule to build all .cpp files in the src/ImGui folder
bin/%.o: src/imgui/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/core/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/entity/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/renderer/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/res/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/game/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

bin/%.o: src/tools/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

debug: imgui_o app_o entity_o renderer_o resources_o game_o tools_o
	${CC} -g -O0 $(STATIC_LIBS) $(INCLUDES) -o $(DEBUG_FOLDER)/fortress.exe ${BIN} $(ICON_DIR) $(DEBUG_LIBS) -mconsole

build: imgui_o app_o entity_o renderer_o resources_o game_o tools_o
	${CC} -s -O3 -finline-functions -flto $(STATIC_LIBS) $(INCLUDES) -o $(RELEASE_FOLDER)/${NAME}.exe ${BIN} $(ICON_DIR) $(LIBS)

compile: bin_dir imgui_o app_o entity_o renderer_o resources_o game_o tools_o
	${CC} $(STATIC_LIBS) $(INCLUDES) -Llib -o ${NAME}_debug ${BIN} $(ICON_DIR) $(LIBS) -mconsole

web: src/core/*.cpp src/entity/*.cpp src/renderer/*.cpp src/res/*.cpp src/game/*.cpp src/tools/*.cpp
	em++ $^ -o .web/yare.js -g -lm --bind -std=c++20 -Wno-narrowing -s USE_SDL=2 -O3 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_STACK=64MB -s INITIAL_MEMORY=96MB --preload-file res --preload-file res/font --preload-file res/icon --preload-file res/sprites --use-preload-plugins


