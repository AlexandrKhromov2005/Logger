CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC
LDFLAGS = -shared

all: libLogger.so testApp

libLogger.so: src/Logger.cpp include/Logger.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

testApp: test/main.cpp libLogger.so
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lLogger

clean:
	rm -f libLogger.so testApp