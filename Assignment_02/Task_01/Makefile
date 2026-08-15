# Compiler settings
CXX = g++
CXXFLAGS = -Wall -O2

# Directories
DRIVER_DIR = driver
SRC_DIR = source
TEST_DIR = test
OUT_DIR = output

# CSR conversion helper is reused from Assignment_01 / Task_03 (per the
# Assignment_02 instructions: call it, do not copy it).
CSR_DIR = ../../Assignment_01/Task_03/source

# Executable name
TARGET = $(DRIVER_DIR)/bf_app

# Default target: compile the code
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(DRIVER_DIR)/bellmanFordMain.cpp $(SRC_DIR)/bellmanFord.cpp $(CSR_DIR)/csr.cpp
	$(CXX) $(CXXFLAGS) $(DRIVER_DIR)/bellmanFordMain.cpp $(SRC_DIR)/bellmanFord.cpp $(CSR_DIR)/csr.cpp -o $(TARGET)

# Spec example (section 5.2)
run-example: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_example.txt $(OUT_DIR)/output_example.txt
	@echo "Example test finished! Check $(OUT_DIR)/output_example.txt for results."

# Required sizes (section 4.2): 10, 100, 10000, 50000, 100000
run1: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_10.txt $(OUT_DIR)/output_01.txt
	@echo "Test 1 (V=10) finished! Check $(OUT_DIR)/output_01.txt for results."

run2: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_100.txt $(OUT_DIR)/output_02.txt
	@echo "Test 2 (V=100) finished! Check $(OUT_DIR)/output_02.txt for results."

run3: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_10000.txt $(OUT_DIR)/output_03.txt
	@echo "Test 3 (V=10000) finished! Check $(OUT_DIR)/output_03.txt for results."

run4: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_50000.txt $(OUT_DIR)/output_04.txt
	@echo "Test 4 (V=50000) finished! Check $(OUT_DIR)/output_04.txt for results."

run5: $(TARGET)
	./$(TARGET) $(TEST_DIR)/bf_100000.txt $(OUT_DIR)/output_05.txt
	@echo "Test 5 (V=100000) finished! Check $(OUT_DIR)/output_05.txt for results."

# Run all tests sequentially
run-all: run-example run1 run2 run3 run4 run5

# Clean up compiled files
clean:
	rm -f $(TARGET)
