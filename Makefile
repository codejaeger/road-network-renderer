OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include
#For mac os
FRAMEWORKS=-framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon

INCLUDES=$(CPPFLAGS) -I./include
OBJECTFILES=./obj

BINDIR1=./bin/sample_bin
SRCDIR1=./src/sample

SHADERCPP=./src/shader_util.cpp
SHADEROBJ=$(OBJECTFILES)/shader_util.o

BIN1=$(BINDIR1)/01_triangle
SRCS1=$(SRCDIR1)/01_triangle.cpp $(SRCDIR1)/gl_framework.cpp

all: $(BIN1)

$(BIN1): $(SRCS1) $(SHADEROBJ)
	g++ $(INCLUDES) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS) #For linux
# 	g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) # for mac os

$(SHADEROBJ): $(SHADERCPP)
	g++ $(INCLUDES) -c $(SHADERCPP) $(OPENGLLIB) -o $(SHADEROBJ) #For linux 
# 	g++ $(INCLUDES) -c $(SHADERCPP) $(FRAMEWORKS) -o $(SHADEROBJ) #For mac

clean:
	rm -f *~ $(BINDIR1)/* $(OBJECTFILES)/*.cc

