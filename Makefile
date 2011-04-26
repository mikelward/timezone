PROGRAMS=dstinfo
all: tags $(PROGRAMS)

tags:
	ctags -R

clean:
	-rm *.o

clobber: clean
	-rm $(PROGRAMS)

dstinfo: timezone.o dstinfo.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

