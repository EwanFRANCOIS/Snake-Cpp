CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
nom ?= main

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: build clean
build: $(nom)

clean:
	rm -f $(nom)
