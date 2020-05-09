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
BIN2=$(BINDIR1)/road_io
SRCS1=$(SRCDIR1)/road.cpp $(SRCDIR1)/gl_framework.cpp $(SRCDIR1)/road_sep.cpp $(SRCDIR1)/texture.cpp $(SRCDIR1)/road_main.cpp $(SRCDIR1)/road_collection.cpp
SRCS2=$(SRCDIR2)/path.cpp $(SRCDIR2)/gl_framework.cpp $(SRCDIR2)/simulation.cpp

all: $(SHADEROBJ) $(BIN1) $(BIN2)

$(BIN1): $(SRCS1) $(SHADEROBJ)
	g++ $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) $(LDFLAGS) $(LIBS) #For linux
	# g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) # for mac os

$(BIN2): $(SHADEROBJ)
	g++ $(INCLUDES) $(SRCS2) -o $(BIN2) $(SHADEROBJ) $(LDFLAGS) $(LIBS) #For linux
	# g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS2) -o $(BIN2) $(SHADEROBJ) # for mac os

$(SHADEROBJ): $(SHADERCPP)
	g++ $(INCLUDES) -c $(SHADERCPP) -o $(SHADEROBJ) $(LDFLAGS) $(LIBS) #For linux
# 	g++ $(INCLUDES) -c $(SHADERCPP) $(FRAMEWORKS) -o $(SHADEROBJ) #For mac

clean:
	rm -f *~ $(BINDIR1)/* $(OBJECTFILES)/*.o
