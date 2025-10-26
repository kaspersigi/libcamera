###########################################
#Makefile for my C++ programs
###########################################

TRG = main
OBJ = main.o
SRC = main.cc
VPATH = /home/miku/learn/Learning_C++
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
LIBS = -lpthread
LLVMLIBC = /usr/lib/llvm$(LLVM)/lib/
CLANG_FLAGS = -nostdlib
CLANG_LINK_FLAGS = /usr/lib/llvm$(LLVM)/lib/libllvmlibc.a -static
CLANGXX_FLAGS = -stdlib=libc++
CLANGXX_LINK_FLAGS = -nostdlib++ -lc++
TIDY_FLAGS = -- --target=$(ARCH) $(DEBUG)

.PHONY : all
all : template open
template :
	$(MAKE) -C 00.template
open :
	$(MAKE) -C 01.open

format :
	$(MAKE) -C 00.template format
	$(MAKE) -C 01.open format

clean :
	$(MAKE) -C 00.template clean
	$(MAKE) -C 01.open clean

scan :
	$(MAKE) -C 00.template scan
	$(MAKE) -C 01.open scan

tidy :
	$(MAKE) -C 00.template tidy
	$(MAKE) -C 01.open tidy

style :
	clang-format -style=webkit -dump-config > .clang-format

bak :
	git archive --format=tar.gz --prefix=libcamera/ --output ../libcamera.tar.gz master