CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC
LDFLAGS = -shared

# Цели сборки
all: libLogger.so testApp

# Сборка динамической библиотеки
libLogger.so: src/Logger.cpp include/Logger.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

# Сборка тестового приложения
testApp: test/main.cpp libLogger.so
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lLogger -pthread

# Очистка
clean:
	rm -f libLogger.so testApp
