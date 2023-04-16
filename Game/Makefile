# #OBJS specifies which files to compile as part of the project
OBJS1 = main.cpp
OBJS2 = bitboard.cpp
OBJS3 = display.cpp
OBJS4 = board.cpp
OBJS5 = button.cpp
OBJS6 = button.cpp 

# #OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# #This is the target that compiles our executable
all : $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) 
	g++ $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) -IC:\NEN\SDL2-devel-2.26.3-mingw\SDL2-2.26.3\x86_64-w64-mingw32\include\SDL2 -LC:\NEN\SDL2-devel-2.26.3-mingw\SDL2-2.26.3\x86_64-w64-mingw32\lib -w "-Wl,-subsystem,windows" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o $(OBJ_NAME)
