###########################################
#Makefile for my C++ programs
###########################################

TRG = open
OBJ = open.o ftrace.o
SRC = open.cc ftrace.cc
LLVM = -21
CONFIG = llvm-config$(LLVM)
ARCH = $(shell $(CONFIG) --host-target)
CC = /usr/bin/clang$(LLVM) --target=$(ARCH)
CXX = /usr/bin/clang++$(LLVM) --target=$(ARCH)
SCAN = scan-build$(LLVM) --use-cc=/usr/bin/clang$(LLVM) --use-c++=/usr/bin/clang++$(LLVM)
TIDY = /usr/bin/clang-tidy$(LLVM) -quiet
FORMAT = /usr/bin/clang-format$(LLVM)
C_FLAGS = -std=c23 -Wall -Wno-unused
CXX_FLAGS = -std=c++26 -Wall -Wno-unused
DEBUG = -g -O0
RELEASE = -flto -O3
LIBS = -lcamera -lcamera-base -L$(shell uname -m) -lspdlog -lm
LLVMLIBC = /usr/lib/llvm$(LLVM)/lib/
CLANG_FLAGS = -nostdlib
CLANG_LINK_FLAGS = /usr/lib/llvm$(LLVM)/lib/libllvmlibc.a -static
CLANGXX_FLAGS = -stdlib=libc++
CLANGXX_LINK_FLAGS = -nostdlib++ -lc++
TIDY_FLAGS = -- --target=$(ARCH) $(CXX_FLAGS) $(DEBUG)

.PHONY : all
$(TRG) : open.o ftrace.o
	$(CXX) open.o ftrace.o -o $(TRG) $(LIBS)
	rm -v open.o ftrace.o
open.o : open.cc
	$(CXX) open.cc -c $(CXX_FLAGS) $(DEBUG) -I/usr/include/libcamera -I./inc
ftrace.o : ftrace.cc
	$(CXX) ftrace.cc -c $(CXX_FLAGS) $(DEBUG)
format :
	$(FORMAT) open.cc -style=file -i
	$(FORMAT) ftrace.h -style=file -i
	$(FORMAT) ftrace.cc -style=file -i
clean :
	rm -v $(TRG)
scan :
	$(SCAN) make
tidy :
	$(TIDY) open.cc $(TIDY_FLAGS) -I/usr/include/libcamera -I./inc
	$(TIDY) ftrace.cc $(TIDY_FLAGS)
check :
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --error-limit=no ./$(TRG)