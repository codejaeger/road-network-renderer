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

BINDIR1=./bin

SRCDIR0=./src
SRCDIR1=./src/Bezier-Curve
SRCDIR2=./src/Road-Model
SRCDIR3=./src/Road-Graph
SRCDIR4=./src/Traffic-Manager
SRCDIR5=./src/Models
SRCDIR6=./src/SkyMaps

SHADERCPP=./src/shader_util.cpp
SHADEROBJ=$(OBJECTFILES)/shader_util.o

BIN1=$(BINDIR1)/input
BIN2=$(BINDIR1)/output

SRCS1=$(SRCDIR0)/input_main.cpp $(SRCDIR0)/input_gl_framework.cpp
SRCS2=$(SRCDIR1)/path.cpp

SRCS3=$(SRCDIR0)/output_main.cpp $(SRCDIR0)/output_gl_framework.cpp
SRCS4=$(SRCDIR2)/road_collection.cpp $(SRCDIR2)/road.cpp $(SRCDIR2)/road_sep.cpp $(SRCDIR2)/intersection.cpp $(SRCDIR2)/texture.cpp
SRCS5=$(SRCDIR3)/graph.cpp
SRCS6=$(SRCDIR4)/car_node.cpp $(SRCDIR1)/path.cpp $(SRCDIR4)/intersection_lights.cpp $(SRCDIR4)/manager.cpp
SRCS7=$(SRCDIR5)/car_model.cpp $(SRCDIR5)/traffic_light_model.cpp
SRCS8=$(SRCDIR6)/skymaps.cpp

all: $(SHADEROBJ) $(BIN1) $(BIN2)

$(BIN1): $(SRCS1) $(SRCS2) $(SHADEROBJ)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) $(SRCS1) $(SRCS2) -o $(BIN1) $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1) $(SRCS2) -o $(BIN1) $(SHADEROBJ);\
  fi

$(BIN2): $(SRCS3) $(SRCS4) $(SRCS5) $(SRCS6) $(SRCS7) $(SHADEROBJ)
	@if [ $(UNAME_S) = "Linux" ]; then\
		g++ $(INCLUDES) $(SRCS3) $(SRCS4) $(SRCS5) $(SRCS6) $(SRCS7) $(SRCS8) -o $(BIN2) $(SHADEROBJ) $(LDFLAGS) $(LIBS);\
  fi
	@if [ $(UNAME_S) = "Darwin" ]; then\
		g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS3) $(SRCS4) $(SRCS5) $(SRCS6) $(SRCS7) $(SRCS8) -o $(BIN2) $(SHADEROBJ);\
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
