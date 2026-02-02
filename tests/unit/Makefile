.PHONY: build test clean help install-deps

all: build

install-deps:
	@echo "Installing unit test dependencies..."
	@sudo apt-get update && sudo apt-get install -y cmake libcmocka-dev pkg-config

build:
	@echo "Building unit tests..."
	@mkdir -p build
	@cd build && cmake .. && make

test: build
	@echo "Running unit tests..."
	@cd build && CTEST_OUTPUT_ON_FAILURE=1 make test

clean:
	@echo "Cleaning unit tests..."
	@rm -rf build

help:
	@echo "Available targets: build, test, clean, install-deps, help"