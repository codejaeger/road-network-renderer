OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include
FRAMEWORKS=-framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon

INCLUDES=$(CPPFLAGS) -I./include
OBJECTFILES=./obj

BINDIR1=./bin/Road-Model
SRCDIR1=./src/Road-Model
SRCDIR2=./src/Bezier-Curve

SHADERCPP=./src/shader_util.cpp
SHADEROBJ=$(OBJECTFILES)/shader_util.o
PATHOBJ=$(OBJECTFILES)/path.o

BIN1=$(BINDIR1)/road-network-renderer
SRCS1=$(SRCDIR1)/road.cpp $(SRCDIR1)/gl_framework.cpp $(SRCDIR1)/road_sep.cpp $(SRCDIR1)/texture.cpp $(SRCDIR1)/road_main.cpp $(SRCDIR1)/road_collection.cpp
SRCS2=$(SRCDIR2)/path.cpp

all: $(SHADEROBJ) $(PATHOBJ) $(BIN1)

$(BIN1): $(SRCS1) $(PATHOBJ) $(SHADEROBJ)
#	g++ $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) $(PATHOBJ) $(LDFLAGS) $(LIBS) #For linux
	g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) $(PATHOBJ) # for mac os

# check later why $(SHADEROBJ) file not needed for pathobj linking
$(PATHOBJ): $(SRCS2)
#	g++ $(INCLUDES) -c $(SRCS2) -o $(PATHOBJ) $(LDFLAGS) $(LIBS) #For linux 
	g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) -c $(SRCS2) -o $(PATHOBJ) # for mac os	

$(SHADEROBJ): $(SHADERCPP)
	g++ $(INCLUDES) -c $(SHADERCPP) -o $(SHADEROBJ) $(LDFLAGS) $(LIBS) #For linux 
# 	g++ $(INCLUDES) -c $(SHADERCPP) $(FRAMEWORKS) -o $(SHADEROBJ) #For mac

clean:
	rm -f *~ $(BINDIR1)/* $(OBJECTFILES)/*.o

