main: timezone.h timezone.cpp main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o dstinfo main.cpp timezone.cpp

