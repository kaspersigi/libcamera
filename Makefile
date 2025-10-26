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
TIDY_FLAGS = -- --target=$(ARCH) $(CXX_FLAGS) $(CLANGXX_FLAGS) $(DEBUG)

.PHONY : all
all : template macro regex constructor interface shared_ptr weak_ptr const function_template class_template threadpool iterator class_link class_export class_thread class_async class_promise class_pthread async_callback print_format print path filesystem ifstream ofstream boost_regex static_local_variable singleton ftrace halrequest framecapture spdlog
template :
	$(MAKE) -C 00.template
macro :
	$(MAKE) -C 01.macro
regex :
	$(MAKE) -C 02.regex
constructor :
	$(MAKE) -C 03.constructor
interface :
	$(MAKE) -C 04.interface
shared_ptr :
	$(MAKE) -C 05.shared_ptr
weak_ptr :
	$(MAKE) -C 06.weak_ptr
const :
	$(MAKE) -C 07.const
function_template :
	$(MAKE) -C 08.function_template
class_template :
	$(MAKE) -C 09.class_template
threadpool :
	$(MAKE) -C 10.threadpool
iterator :
	$(MAKE) -C 11.iterator
class_link :
	$(MAKE) -C 12.class_link
class_export :
	$(MAKE) -C 13.class_export
class_thread :
	$(MAKE) -C 14.class_thread
class_async :
	$(MAKE) -C 15.class_async
class_promise :
	$(MAKE) -C 16.class_promise
class_pthread :
	$(MAKE) -C 17.class_pthread
async_callback :
	$(MAKE) -C 18.async_callback
print_format :
	$(MAKE) -C 19.format
print :
	$(MAKE) -C 20.print
path :
	$(MAKE) -C 21.path
filesystem :
	$(MAKE) -C 22.filesystem
ifstream :
	$(MAKE) -C 23.ifstream
ofstream :
	$(MAKE) -C 24.ofstream
boost_regex :
	$(MAKE) -C 25.boost_regex
static_local_variable :
	$(MAKE) -C 26.static_local_variable
singleton :
	$(MAKE) -C 27.singleton
ftrace :
	$(MAKE) -C 28.ftrace
halrequest :
	$(MAKE) -C 29.halrequest
framecapture :
	$(MAKE) -C 30.framecapture
spdlog :
	$(MAKE) -C 31.spdlog

format :
	$(MAKE) -C 00.template format
	$(MAKE) -C 01.macro format
	$(MAKE) -C 02.regex format
	$(MAKE) -C 03.constructor format
	$(MAKE) -C 04.interface format
	$(MAKE) -C 05.shared_ptr format
	$(MAKE) -C 06.weak_ptr format
	$(MAKE) -C 07.const format
	$(MAKE) -C 08.function_template format
	$(MAKE) -C 09.class_template format
	$(MAKE) -C 10.threadpool format
	$(MAKE) -C 11.iterator format
	$(MAKE) -C 12.class_link format
	$(MAKE) -C 13.class_export format
	$(MAKE) -C 14.class_thread format
	$(MAKE) -C 15.class_async format
	$(MAKE) -C 16.class_promise format
	$(MAKE) -C 17.class_pthread format
	$(MAKE) -C 18.async_callback format
	$(MAKE) -C 19.format format
	$(MAKE) -C 20.print format
	$(MAKE) -C 21.path format
	$(MAKE) -C 22.filesystem format
	$(MAKE) -C 23.ifstream format
	$(MAKE) -C 24.ofstream format
	$(MAKE) -C 25.boost_regex format
	$(MAKE) -C 26.static_local_variable format
	$(MAKE) -C 27.singleton format
	$(MAKE) -C 28.ftrace format
	$(MAKE) -C 29.halrequest format
	$(MAKE) -C 30.framecapture format
	$(MAKE) -C 31.spdlog format

clean :
	$(MAKE) -C 00.template clean
	$(MAKE) -C 01.macro clean
	$(MAKE) -C 02.regex clean
	$(MAKE) -C 03.constructor clean
	$(MAKE) -C 04.interface clean
	$(MAKE) -C 05.shared_ptr clean
	$(MAKE) -C 06.weak_ptr clean
	$(MAKE) -C 07.const clean
	$(MAKE) -C 08.function_template clean
	$(MAKE) -C 09.class_template clean
	$(MAKE) -C 10.threadpool clean
	$(MAKE) -C 11.iterator clean
	$(MAKE) -C 12.class_link clean
	$(MAKE) -C 13.class_export clean
	$(MAKE) -C 14.class_thread clean
	$(MAKE) -C 15.class_async clean
	$(MAKE) -C 16.class_promise clean
	$(MAKE) -C 17.class_pthread clean
	$(MAKE) -C 18.async_callback clean
	$(MAKE) -C 19.format clean
	$(MAKE) -C 20.print clean
	$(MAKE) -C 21.path clean
	$(MAKE) -C 22.filesystem clean
	$(MAKE) -C 23.ifstream clean
	$(MAKE) -C 24.ofstream clean
	$(MAKE) -C 25.boost_regex clean
	$(MAKE) -C 26.static_local_variable clean
	$(MAKE) -C 27.singleton clean
	$(MAKE) -C 28.ftrace clean
	$(MAKE) -C 29.halrequest clean
	$(MAKE) -C 30.framecapture clean
	$(MAKE) -C 31.spdlog clean

scan :
	$(MAKE) -C 00.template scan
	$(MAKE) -C 01.macro scan
	$(MAKE) -C 02.regex scan
	$(MAKE) -C 03.constructor scan
	$(MAKE) -C 04.interface scan
	$(MAKE) -C 05.shared_ptr scan
	$(MAKE) -C 06.weak_ptr scan
	$(MAKE) -C 07.const scan
	$(MAKE) -C 08.function_template scan
	$(MAKE) -C 09.class_template scan
	$(MAKE) -C 10.threadpool scan
	$(MAKE) -C 11.iterator scan
	$(MAKE) -C 12.class_link scan
	$(MAKE) -C 13.class_export scan
	$(MAKE) -C 14.class_thread scan
	$(MAKE) -C 15.class_async scan
	$(MAKE) -C 16.class_promise scan
	$(MAKE) -C 17.class_pthread scan
	$(MAKE) -C 18.async_callback scan
	$(MAKE) -C 19.format scan
	$(MAKE) -C 20.print scan
	$(MAKE) -C 21.path scan
	$(MAKE) -C 22.filesystem scan
	$(MAKE) -C 23.ifstream scan
	$(MAKE) -C 24.ofstream scan
	$(MAKE) -C 25.boost_regex scan
	$(MAKE) -C 26.static_local_variable scan
	$(MAKE) -C 27.singleton scan
	$(MAKE) -C 28.ftrace scan
	$(MAKE) -C 29.halrequest scan
	$(MAKE) -C 30.framecapture scan
	$(MAKE) -C 31.spdlog scan

tidy :
	$(MAKE) -C 00.template tidy
	$(MAKE) -C 01.macro tidy
	$(MAKE) -C 02.regex tidy
	$(MAKE) -C 03.constructor tidy
	$(MAKE) -C 04.interface tidy
	$(MAKE) -C 05.shared_ptr tidy
	$(MAKE) -C 06.weak_ptr tidy
	$(MAKE) -C 07.const tidy
	$(MAKE) -C 08.function_template tidy
	$(MAKE) -C 09.class_template tidy
	$(MAKE) -C 10.threadpool tidy
	$(MAKE) -C 11.iterator tidy
	$(MAKE) -C 12.class_link tidy
	$(MAKE) -C 13.class_export tidy
	$(MAKE) -C 14.class_thread tidy
	$(MAKE) -C 15.class_async tidy
	$(MAKE) -C 16.class_promise tidy
	$(MAKE) -C 17.class_pthread tidy
	$(MAKE) -C 18.async_callback tidy
	$(MAKE) -C 19.format tidy
	$(MAKE) -C 20.print tidy
	$(MAKE) -C 21.path tidy
	$(MAKE) -C 22.filesystem tidy
	$(MAKE) -C 23.ifstream tidy
	$(MAKE) -C 24.ofstream tidy
	$(MAKE) -C 25.boost_regex tidy
	$(MAKE) -C 26.static_local_variable tidy
	$(MAKE) -C 27.singleton tidy
	$(MAKE) -C 28.ftrace tidy
	$(MAKE) -C 29.halrequest tidy
	$(MAKE) -C 30.framecapture tidy
	$(MAKE) -C 31.spdlog tidy