srcdir=./src
libdir=./lib
incdir=-I./include -I./src

target=main.out
src=$(shell find $(srcdir) -name "*.cpp")
obj=$(patsubst %.cpp, %.o, $(src))
dir=$(patsubst %.cpp, %.d, $(src))
lib=$(notdir $(shell find $(libdir) -name "*.so"))
lib:=$(patsubst lib%.so,-l%, $(lib)) -lpthread
staticlib=$(shell find $(libdir) -name "*.a")

CXX=g++
CXXFLAGS=-std=c++17 -g -O3 -Wall 
CXXLIB=$(incdir) -L$(libdir) $(lib)

$(target):$(obj)
	$(CXX) $(CXXFLAGS) $(CXXLIB) -o $@ $^ $(staticlib)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP -o $@ $<

-include $(dir)

.PHONY:echo clean

echo:
	@echo $(target)
	@echo $(obj)
	@echo $(src)
	@echo $(dir)
	@echo $(CXXLIB)

clean:
	rm -f $(obj) $(target) $(dir)
