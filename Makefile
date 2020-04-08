OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

INCLUDES=$(CPPFLAGS) -I./include
OBJECTFILES=./obj

BINDIR1=./bin/sample_bin
SRCDIR1=./src/sample

SHADERCPP=./src/shader_util.cpp
SHADEROBJ=$(OBJECTFILES)/shader.cc

BIN1=$(BINDIR1)/01_triangle
SRCS1=$(SRCDIR1)/01_triangle.cpp $(SRCDIR1)/gl_framework.cpp $(SHADERCPP)

all: $(BIN1) $(SHADEROBJ)

$(BIN1): $(SRCS1)
	g++ $(INCLUDES) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(SHADEROBJ): $(SHADERCPP)
	g++ $(INCLUDES) -c $(SHADERCPP) $(OPENGLLIB) -o $(SHADEROBJ) 

clean:
	rm -f *~ $(BINDIR1)/* $(OBJECTFILES)/*.cc

