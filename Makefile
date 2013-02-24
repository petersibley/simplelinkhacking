SOURCES=$(wildcard *.cpp)

CXX=c++
LD=c++
IFLAGS= -I/usr/include/ -I/usr/local/include
LIBS= -L/usr/lib -L/usr/local/lib -lm -lstdc++  -lboost_graph-mt

#CFLAGS=  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64  -g -D_DEBUG
CFLAGS=  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64  -O3
PROGRAM=process



OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

all: $(PROGRAM)

$(PROGRAM) : $(OBJECTS)
	$(LD) $(LIBS) -o $(PROGRAM) $(OBJECTS)

%.o: %.cpp %.h

clean:
	rm -f $(OBJECTS) $(PROGRAM) 

#-include $(OBJECTALL:=.d)
#%.d : %.cpp
#	set -e; $(CXX) -MM $(CFLAGS) $< \
#		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
#		[ -s $@ ] || rm -f $@
#
.SUFFIXES:  .o .cpp
.cpp.o:
	$(CXX) -c $(IFLAGS) $(CFLAGS) -o $@ $<




