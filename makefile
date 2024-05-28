CXX ?= g++
BUILD_FLAGS=


build:
	mkdir -p out
	$(CXX) $(BUILD_FLAGS) \
		./src/*.cpp \
		./app/*.cpp \
		-I ./include/ \
		-o ./out/server \
		-lpqxx -lpq \
		-lmosquittopp \
		-std=c++23 \

