# Compiler settings
CXX = g++
CXXFLAGS = -Wall -O2

# Directories
DRIVER_DIR = driver
SRC_DIR = source
TEST_DIR = test
OUT_DIR = output

# Executable name
TARGET = $(DRIVER_DIR)/fw_app

# Default target
all: $(TARGET)

# Compile the executable (no CSR dependency - Floyd-Warshall is exempt, section 4.1)
$(TARGET): $(DRIVER_DIR)/floydWarshallMain.cpp $(SRC_DIR)/floydWarshall.cpp
	$(CXX) $(CXXFLAGS) $(DRIVER_DIR)/floydWarshallMain.cpp $(SRC_DIR)/floydWarshall.cpp -o $(TARGET)

# Spec example (section 6.2)
run-example: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_example.txt $(OUT_DIR)/output_example.txt
	@echo "Example test finished! Check $(OUT_DIR)/output_example.txt for results."

# Required sizes (section 4.2): 10, 100, 500, 1000, 2000
run1: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_10.txt $(OUT_DIR)/output_01.txt
	@echo "Test 1 (V=10) finished! Check $(OUT_DIR)/output_01.txt for results."

run2: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_100.txt $(OUT_DIR)/output_02.txt
	@echo "Test 2 (V=100) finished! Check $(OUT_DIR)/output_02.txt for results."

run3: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_500.txt $(OUT_DIR)/output_03.txt
	@echo "Test 3 (V=500) finished! Check $(OUT_DIR)/output_03.txt for results."

run4: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_1000.txt $(OUT_DIR)/output_04.txt
	@echo "Test 4 (V=1000) finished! Check $(OUT_DIR)/output_04.txt for results."

run5: $(TARGET)
	./$(TARGET) $(TEST_DIR)/fw_2000.txt $(OUT_DIR)/output_05.txt
	@echo "Test 5 (V=2000) finished! Check $(OUT_DIR)/output_05.txt for results."

# Run all tests sequentially
run-all: run-example run1 run2 run3 run4 run5

# Clean
clean:
	rm -f $(TARGET)
