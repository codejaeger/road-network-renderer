UNAME_S := $(shell uname -s)

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
SRCDIR3=./src/Multiple-Roads

SHADERCPP=./src/shader_util.cpp
SHADEROBJ=$(OBJECTFILES)/shader_util.o

BIN1=$(BINDIR1)/road-network-renderer
BIN2=$(BINDIR1)/road_io
BIN3=$(BINDIR1)/multiple-roads
SRCS1=$(SRCDIR1)/road.cpp $(SRCDIR1)/gl_framework.cpp $(SRCDIR1)/road_sep.cpp $(SRCDIR1)/texture.cpp $(SRCDIR1)/road_main.cpp $(SRCDIR1)/road_collection.cpp
SRCS2=$(SRCDIR2)/path.cpp $(SRCDIR2)/gl_framework.cpp $(SRCDIR2)/simulation.cpp
SRCS3=$(SRCDIR3)/path.cpp $(SRCDIR3)/gl_framework.cpp $(SRCDIR3)/simulation.cpp

all: $(SHADEROBJ) $(BIN1) $(BIN2) $(BIN3)

$(BIN1): $(SRCS1) $(SHADEROBJ)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1) -o $(BIN1) $(SHADEROBJ);\
  fi

$(BIN2): $(SHADEROBJ)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) $(SRCS2) -o $(BIN2) $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS2) -o $(BIN2) $(SHADEROBJ);\
  fi

$(BIN3): $(SHADEROBJ)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) $(SRCS3) -o $(BIN3) $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS3) -o $(BIN3) $(SHADEROBJ);\
  fi

$(SHADEROBJ): $(SHADERCPP)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) -c $(SHADERCPP) -o $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(INCLUDES) -c $(SHADERCPP) $(FRAMEWORKS) -o $(SHADEROBJ);\
  fi

clean:
	rm -f *~ $(BINDIR1)/* $(OBJECTFILES)/*.o
