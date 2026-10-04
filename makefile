# C++ compiler and language version.
CXX := g++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra

# Location where vcpkg installed ONNX and Protobuf.
VCPKG_ROOT ?= $(HOME)/vcpkg
VCPKG_PREFIX := $(VCPKG_ROOT)/installed/arm64-osx

# Ask pkg-config for every library required by Protobuf.
PKG_CONFIG_PATH := $(VCPKG_PREFIX)/lib/pkgconfig
PROTOBUF_CFLAGS := $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" pkg-config --cflags protobuf)
PROTOBUF_LIBS := $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" pkg-config --libs protobuf)

# TinyInfer source files and the executable to create.
SOURCES := Source/Tinyinfer.cpp Source/Kernels.cpp Source/helpers.cpp Source/benchmark_helpers.cpp
TARGET := Tinyinfer
ORT_PREFIX ?= /opt/homebrew/opt/onnxruntime

# Build TinyInfer when make is run without a target.
all: $(TARGET)

$(TARGET): $(SOURCES) Headers/Kernels.h Headers/helpers.h Headers/benchmark_helpers.h
	$(CXX) $(CXXFLAGS) $(PROTOBUF_CFLAGS) -IHeaders \
		-DONNX_NAMESPACE=onnx -DONNX_ML=1 \
		$(SOURCES) -L$(VCPKG_PREFIX)/lib \
		-lonnx -lonnx_proto $(PROTOBUF_LIBS) -o $(TARGET)

# Build the separate ONNX Runtime benchmark using native Homebrew.
onnx_benchmark: Source/onnx_benchmark.cpp Source/benchmark_helpers.cpp Headers/benchmark_helpers.h makefile
	$(CXX) $(CXXFLAGS) -IHeaders -I$(ORT_PREFIX)/include/onnxruntime \
		Source/onnx_benchmark.cpp Source/benchmark_helpers.cpp \
		-L$(ORT_PREFIX)/lib -Wl,-rpath,$(ORT_PREFIX)/lib -lonnxruntime -o $@

# Build TinyInfer if needed, then run it from the project root.
run: $(TARGET)
	./$(TARGET)

# Delete the generated executable.
clean:
	rm -f $(TARGET) onnx_benchmark

.PHONY: all run clean
