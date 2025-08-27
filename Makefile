BUILD_DIR  ?= build
BUILD_TYPE ?= Release
TARGET     ?= curry_vision

.PHONY: run
run:
	rm -rf $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -j
	./$(BUILD_DIR)/$(TARGET)