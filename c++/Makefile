CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread

SOURCES = producer-consumer.cpp future.cpp packaged_task.cpp async.cpp barrier.cpp shared_future.cpp load-balance.cpp

BUILD_DIR = build

OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))

BINARIES = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=))

all: $(BINARIES)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BINARIES)
