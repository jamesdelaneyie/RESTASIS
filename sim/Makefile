# STAEXE desktop simulator
# Builds the same choreography C++ used on the Mega, with fake I/O.

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -DSIMULATOR
INCLUDES  = -I. -I..

ROOT = ..

SRCS = \
	main.cpp \
	display_sim.cpp \
	motors_sim.cpp \
	$(ROOT)/staexe_choreography.cpp \
	$(ROOT)/staexe_state.cpp

OBJS = $(SRCS:.cpp=.o)
# Object files from parent dir land beside sources; normalize names:
BIN = staexe-sim

.PHONY: all clean

all: $(BIN)

$(BIN): main.cpp display_sim.cpp motors_sim.cpp $(ROOT)/staexe_choreography.cpp $(ROOT)/staexe_state.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ \
		main.cpp display_sim.cpp motors_sim.cpp \
		$(ROOT)/staexe_choreography.cpp $(ROOT)/staexe_state.cpp \
		-pthread

clean:
	rm -f $(BIN)
