# -----------------------------------Constants----------------------------------
AllWarnings    = -Wall -Wextra -pedantic
SomeWarnings   = -Wall -Wextra
LittleWarnings = -Wall
NoWarnings     = 
# -----------------------------------Constants----------------------------------

Mode = DEBUG_MODE
# ----------------------------------Debug-mode----------------------------------
ifeq ($(Mode), DEBUG_MODE)
	ModeLinkerOptions   = -g -fsanitize=address -fno-optimize-sibling-calls
	ModeCompilerOptions = -O0 -g -fsanitize=address -fno-optimize-sibling-calls
endif
# ----------------------------------Debug-mode----------------------------------

# ---------------------------------Release-mode---------------------------------
ifeq ($(Mode), RELEASE_MODE)
	ModeLinkerOptions   = 
	ModeCompilerOptions = -O3
endif
# ---------------------------------Release-mode---------------------------------

# ------------------------------------Options-----------------------------------
LIBS = sdl2 sdl2_ttf sdl2_image

CXX = clang++

LXXFLAGS = $(shell pkg-config --libs $(LIBS)) $(ModeLinkerOptions)
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(ModeCompilerOptions) $(NoWarnings) -std=c++17
# ------------------------------------Options-----------------------------------

# -------------------------------------Files------------------------------------
LibsDir     = libs
IncludeDir  = include
SrcDir      = src
BinDir      = bin
IntDir      = $(BinDir)/intermediates

rwildcard   = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
LibArchives = $(LibsDir)/sgl/sgl.a $(LibsDir)/sml/sml.a
Deps 		= $(call rwildcard,$(IncludeDir),*.h)
CppSrc 		= $(notdir $(call rwildcard,$(SrcDir),*.cpp))
Objs        = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))

Exec = editor.out
# -------------------------------------Files------------------------------------

# ----------------------------------Make rules----------------------------------
$(Exec): $(LibArchives) $(Objs) $(Deps)
	$(CXX) -o $(Exec) $(LibArchives) $(Objs) $(LXXFLAGS)

vpath %.cpp $(dir $(call rwildcard,$(SrcDir),*.cpp))
$(IntDir)/%.o: %.cpp $(Deps)
	$(CXX) -I $(IncludeDir) -I $(LibsDir) -c $< $(CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin/intermediates
	mkdir -p libs

.PHONY: clean
clean:
	rm -f $(call rwildcard,$(IntDir),*.o) $(Exec)
# ----------------------------------Make rules----------------------------------