OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include
FRAMEWORKS=-framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
INCLUDES=$(CPPFLAGS)

BIN1=01_triangle

SRCS1= road_sep.cpp road.cpp simulation.cpp texture.cpp gl_framework.cpp shader_util.cpp 
INCLUDES=road_sep.hpp road.hpp simulation.hpp texture.hpp gl_framework.hpp shader_util.hpp
all: $(BIN1)
$(BIN1): $(SRCS1)
	#g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)
	g++ $(GLFWLIB) $(GLEWLIB) $(FRAMEWORKS) $(INCLUDES) $(SRCS1)

clean:
	rm -f *~ *.o $(BIN1)


