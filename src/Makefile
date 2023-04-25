OBJS1 = main.cpp
OBJS2 = bitboards.cpp
OBJS3 = board.cpp
OBJS4 = bot.cpp
OBJS5 = button.cpp
OBJS7 = display.cpp
OBJS8 = eval.cpp
OBJS9 = legal.cpp
OBJS10 = ltexture.cpp
OBJS11 = move.cpp
OBJS12 = movegen.cpp
OBJS13 = piece.cpp
OBJS14 = square.cpp
OBJS15 = zobrist.cpp

# #OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# #This is the target that compiles our executable
all : 
	g++ $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS7) $(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11) $(OBJS12) $(OBJS13) $(OBJS14) $(OBJS15) -IC:\NEN\SDL2-devel-2.26.3-mingw\SDL2-2.26.3\x86_64-w64-mingw32\include -LC:\NEN\SDL2-devel-2.26.3-mingw\SDL2-2.26.3\x86_64-w64-mingw32\lib -w "-Wl,-subsystem,windows" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o $(OBJ_NAME)

