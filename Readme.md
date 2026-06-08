# TinyInfer

**TinyInfer** is a small C++ machine learning inference runtime built from scratch.

The goal of this project is to understand and demonstrate how trained neural networks are executed outside of Python frameworks like PyTorch. TinyInfer focuses on the lower-level inference layer: loading model weights, running tensor operations in C++, validating outputs against PyTorch, and optimising performance-critical operations such as matrix multiplication.

This project is not intended to replace PyTorch, TensorRT, ONNX Runtime, or XLA. It is a focused engineering project for learning and demonstrating the core ideas behind ML inference runtimes, performance kernels, and compiler-style graph optimisation.

## Project summary

Most beginner machine learning projects focus on training a model and reporting accuracy.

TinyInfer focuses on what happens after training:

```text
Train model in PyTorch
        ↓
Export weights
        ↓
Load weights in C++
        ↓
Run inference
        ↓
Validate output against PyTorch
        ↓
Optimise the runtime
        ↓
Benchmark performance
```

Inference means using a trained model to make predictions.

For example:

```text
Input image → trained model → predicted digit
```

A runtime is the software that executes the trained model. In production systems, inference runtimes are responsible for running models quickly and correctly on CPUs, GPUs, or accelerators.

TinyInfer is a small version of that idea.

## Why I am building this

The purpose of TinyInfer is to build the layer underneath a machine learning framework.

Instead of only using:

```python
model(input)
```

this project explores what that call actually requires internally:

* tensor storage
* weight loading
* layer execution
* matrix multiplication
* activation functions
* numerical correctness checks
* benchmarking
* CPU performance optimisation
* graph-level optimisation

This makes the project relevant to ML systems, AI infrastructure, inference engineering, and hardware-aware software engineering.

## Engineering focus

TinyInfer is designed around three engineering goals:

### 1. Correctness

Before optimising performance, the runtime must produce correct outputs.

TinyInfer validates its C++ inference results against PyTorch using the same model weights and the same input data. The output difference is checked using a numerical tolerance.

A tolerance is a small allowed difference between two floating-point results. This is needed because different implementations can produce slightly different decimal values.

### 2. Performance

The project investigates why some operations are slow and how to make them faster.

The main performance focus is matrix multiplication, because it is one of the most important operations in neural networks.

TinyInfer will include:

* naive matrix multiplication
* cache-blocked matrix multiplication
* multithreaded matrix multiplication
* benchmark comparisons between implementations

Cache blocking means splitting a large matrix operation into smaller chunks so the CPU can reuse data from cache more efficiently. Cache is small, fast memory close to the processor.

### 3. Runtime and graph optimisation

TinyInfer will also include a simple computation graph.

A computation graph represents the model as a sequence of connected operations:

```text
Input → Linear → ReLU → Linear → Softmax → Output
```

This makes it possible to experiment with graph-level optimisations such as operator fusion.

Operator fusion means combining multiple operations into one operation.

Example:

```text
Before: Linear → ReLU
After:  FusedLinearReLU
```

This can improve performance by reducing unnecessary intermediate memory writes and reads.

## Planned features

### Version 1: Basic C++ inference runtime

* [ ] Tensor class
* [ ] Model weight loading
* [ ] Linear layer
* [ ] ReLU activation
* [ ] Softmax
* [ ] Forward pass for a small neural network
* [ ] MNIST or small classifier inference
* [ ] PyTorch output comparison
* [ ] Unit tests for core operations

### Version 2: Performance kernels

A kernel is a performance-critical function that implements one operation, such as matrix multiplication.

* [ ] Naive matrix multiplication
* [ ] Cache-blocked matrix multiplication
* [ ] Multithreaded matrix multiplication
* [ ] Benchmark suite
* [ ] Runtime comparison between implementations
* [ ] Performance tables and graphs

### Version 3: Graph optimisation

* [ ] Simple computation graph representation
* [ ] Graph execution engine
* [ ] Operator fusion
* [ ] Dead node elimination
* [ ] Constant folding
* [ ] Benchmark before and after graph optimisation

Dead node elimination means removing operations that do not affect the final output.

Constant folding means precomputing values that never change.

## Initial target model

The first target model is a small feedforward neural network trained in PyTorch.

Example:

```text
Input image
    ↓
Linear layer
    ↓
ReLU
    ↓
Linear layer
    ↓
Softmax
    ↓
Predicted class
```

The initial dataset will likely be MNIST or another small classification dataset.

MNIST itself is not the main achievement. The important part is building the runtime system around the model.

## Repository structure

Planned structure:

```text
TinyInfer/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── tensor.hpp
│   ├── layers.hpp
│   ├── model.hpp
│   ├── matmul.hpp
│   └── graph.hpp
├── src/
│   ├── tensor.cpp
│   ├── layers.cpp
│   ├── model.cpp
│   ├── matmul.cpp
│   └── graph.cpp
├── tests/
│   ├── test_tensor.cpp
│   ├── test_layers.cpp
│   └── test_matmul.cpp
├── benchmarks/
│   └── benchmark_matmul.cpp
├── python/
│   ├── train_model.py
│   ├── export_weights.py
│   └── compare_outputs.py
└── models/
    └── weights/
```

## Build instructions

Requirements:

* C++17 or newer
* CMake
* Python 3
* PyTorch, for training and validation scripts

Build:

```bash
cmake -S . -B build
cmake --build build
```

Run tests:

```bash
./build/tests/tinyinfer_tests
```

Run benchmarks:

```bash
./build/benchmarks/benchmark_matmul
```

Run inference:

```bash
./build/tinyinfer models/weights/model.bin data/sample_input.bin
```

## Correctness validation

TinyInfer compares its C++ outputs against PyTorch.

Validation process:

1. Train a small model in PyTorch
2. Export the model weights
3. Load the same weights in TinyInfer
4. Run the same input through PyTorch and TinyInfer
5. Compare the outputs
6. Check that the maximum error is within an acceptable tolerance

Example expected output:

```text
PyTorch logits:   [0.02, 0.04, 0.91, ...]
TinyInfer logits: [0.02, 0.04, 0.91, ...]

Max error: 0.00003
Result: PASS
```

## Benchmarking plan

TinyInfer will benchmark both individual operations and full model inference.

Example benchmark table:

| Operation      | Implementation | Time | Speedup |
| -------------- | -------------: | ---: | ------: |
| MatMul 512x512 |          Naive |  TBD |   1.00x |
| MatMul 512x512 |  Cache-blocked |  TBD |     TBD |
| MatMul 512x512 |  Multithreaded |  TBD |     TBD |
| Linear + ReLU  |   Separate ops |  TBD |   1.00x |
| Linear + ReLU  |       Fused op |  TBD |     TBD |

The final project will include benchmark results, implementation notes, and analysis of where the performance improvements came from.

## What this project demonstrates

TinyInfer is intended to demonstrate:

* C++ systems programming
* understanding of neural network inference
* tensor and memory layout design
* matrix multiplication implementation
* CPU cache-aware optimisation
* multithreading
* benchmarking methodology
* numerical correctness testing
* graph-based model execution
* compiler-style optimisation concepts
* ability to connect software performance to hardware behaviour

## What this project is not

TinyInfer is not:

* a full deep learning framework
* a training library
* a replacement for PyTorch
* a production inference engine
* a complete ONNX Runtime clone
* a GPU runtime

The goal is depth over breadth: implement a small set of features clearly, correctly, and with strong performance analysis.

## Long-term extensions

Possible future extensions:

* ONNX subset importer
* SIMD vectorisation
* INT8 quantised inference
* convolution layer support
* Apple Silicon performance analysis
* ARM NEON backend
* simple memory planner
* additional graph optimisations

SIMD means Single Instruction, Multiple Data. It allows a CPU to apply one instruction to multiple numbers at the same time, which can make numerical code faster.

INT8 quantisation means using 8-bit integers instead of 32-bit floating-point numbers to reduce memory usage and improve inference speed.

## Example CV description

**TinyInfer — C++ ML Inference Runtime**
Built a C++ inference runtime from scratch for small neural networks, including tensor operations, model weight loading, Linear/ReLU/Softmax layers, PyTorch correctness validation, and performance benchmarking. Implemented naive, cache-blocked, and multithreaded matrix multiplication kernels, with planned graph-level optimisations including operator fusion, dead node elimination, and constant folding.

## Final aim

The final aim is to produce a clean, well-tested, and well-documented ML systems project showing how a trained model can be executed and optimised outside of a high-level framework:

```text
PyTorch model
    ↓
Exported weights
    ↓
C++ inference runtime
    ↓
Optimised kernels
    ↓
Graph optimisation
    ↓
Correctness tests + benchmarks
```

TinyInfer is a small project, but it is designed to demonstrate the same kind of thinking used in real ML inference runtimes, ML compilers, and hardware-aware AI systems.
