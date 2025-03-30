PROJECT_NAME := Gelly
BUILD_DIR := build
SRC_DIR := src
DEBUG_EXECUTABLE := $(BUILD_DIR)/Debug/$(PROJECT_NAME)
RELEASE_EXECUTABLE := $(BUILD_DIR)/Release/$(PROJECT_NAME)
CMAKE := cmake
CMAKE_GENERATOR := Ninja Multi-Config
COMMON_CMAKE_ARGS := -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)" -DCMAKE_EXPORT_COMPILE_COMMANDS=1

.PHONY: run clean build-dir install install-release build build-release debug

run: build
	./$(BUILD_DIR)/Debug/$(PROJECT_NAME)

clean:
	rm -rf $(BUILD_DIR)

build-dir:
	mkdir -p $(BUILD_DIR)

install: clean build-dir
	$(CMAKE) $(COMMON_CMAKE_ARGS) .
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

build:
	$(CMAKE) --build $(BUILD_DIR) --config Debug

build-release:
	$(CMAKE) --build $(BUILD_DIR) --config Release

release: build-release
	./$(BUILD_DIR)/Release/$(PROJECT_NAME)

debug: build
	lldb $(BUILD_DIR)/Debug/$(PROJECT_NAME)
