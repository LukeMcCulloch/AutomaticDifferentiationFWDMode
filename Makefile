##########################################################################################

# Specify library locations here (add or remove "#" marks to comment/uncomment lines for your platform)

# Mac OS X
# INCLUDE_PATH      =
# LIBRARY_PATH      =
# BLAS_LIBS         = -framework Accelerate
# SUITESPARSE_LIBS  = -lspqr -lumfpack -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -ltbb -lm -lsuitesparseconfig
# OPENGL_LIBS       = -framework OpenGL -framework GLUT

# # Linux
INCLUDE_PATH      = -I./include -I./src
LIBRARY_PATH      =
BLAS_LIBS         = -llapack -lblas -lgfortran
# SUITESPARSE_LIBS  = -lspqr -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -lm
# OPENGL_LIBS       = -lglut -lGL -lGLU -lX11
SUITESPARSE_LIBS  = -lspqr -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm -lumfpack -lamd #-lmetis 
OPENGL_LIBS       = -lGL -lGLU -lglut -lGLEW #-lX11

# # Windows / Cygwin
# INCLUDE_PATH      = -I/usr/include/opengl -I/usr/include/suitesparse
# LIBRARY_PATH      = -L/usr/lib/w32api -L/usr/lib/suitesparse
# BLAS_LIBS         = -llapack -lblas
# SUITESPARSE_LIBS  = -lspqr -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm
# OPENGL_LIBS       = -lglut32 -lglu32 -lopengl32

########################################################################################

TARGET = run/ADcpp
CC = g++
LD = g++
CFLAGS = -O3 -std=c++17  -Wall -Werror=c++-compat -ansi -pedantic  $(INCLUDE_PATH) 
LFLAGS = -O3 -Wall -Werror=c++-compat -ansi -pedantic $(LIBRARY_PATH)
LIBS = $(OPENGL_LIBS) $(SUITESPARSE_LIBS) $(BLAS_LIBS)

########################################################################################
## !! Do not edit below this line

HEADERS := $(wildcard include/*.h) $(wildcard include/*.hpp)
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "headers = " $(HEADERS)
	@echo "sources = " $(SOURCES)
	@echo "objects = " $(OBJECTS)
	$(LD) $(OBJECTS) -o $(TARGET) $(CFLAGS) $(LFLAGS) $(LIBS)

obj/%.o: src/%.cpp ${HEADERS}
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(TARGET).exe
