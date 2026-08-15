# STAEXE desktop simulator
# Builds the same choreography C++ used on the Mega, with fake I/O.
#
# macOS tip: if you still see missing system headers, run:
#   xcode-select --install
#   export SDKROOT="$(xcrun --sdk macosx --show-sdk-path)"

UNAME_S := $(shell uname -s)

# Prefer clang++ on macOS; allow override via `make CXX=...`
# (ignore a bare env CXX=c++, which often lacks SDK include paths)
ifeq ($(UNAME_S),Darwin)
  ifneq ($(origin CXX),command line)
    CXX := clang++
  endif
  SDKROOT ?= $(shell xcrun --sdk macosx --show-sdk-path 2>/dev/null)
  ifneq ($(SDKROOT),)
    CXXFLAGS_OS := -isysroot $(SDKROOT) -stdlib=libc++
  else
    CXXFLAGS_OS := -stdlib=libc++
  endif
  LDFLAGS_OS :=
else
  ifneq ($(origin CXX),command line)
    CXX := g++
  endif
  CXXFLAGS_OS :=
  LDFLAGS_OS := -pthread
endif

CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -DSIMULATOR $(CXXFLAGS_OS)
INCLUDES  = -I. -I../firmware/STAEXE

ROOT = ../firmware/STAEXE
BIN  = staexe-sim

.PHONY: all clean

all: $(BIN)

$(BIN): main.cpp display_sim.cpp motors_sim.cpp $(ROOT)/staexe_choreography.cpp $(ROOT)/staexe_state.cpp arduino_shim.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ \
		main.cpp display_sim.cpp motors_sim.cpp \
		$(ROOT)/staexe_choreography.cpp $(ROOT)/staexe_state.cpp \
		$(LDFLAGS_OS)

clean:
	rm -f $(BIN)
