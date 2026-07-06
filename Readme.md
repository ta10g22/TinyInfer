# TinyInfer

**TinyInfer** is a small C++ machine learning inference runtime built from scratch.

The goal of this project is to understand and demonstrate how trained neural networks are executed outside of Python frameworks like PyTorch. 

## Project summary

Most beginner machine learning projects focus on training a model and reporting accuracy.

TinyInfer focuses on what happens after training:

```text
Train model in PyTorch
        ↓
Export model as ONNX
        ↓
Load ONNX graph and weights in C++
        ↓
Run inference
        ↓
Validate output against PyTorch
        ↓
Optimise the runtime
        ↓
Benchmark performance
```


## Why I am building this

The purpose of TinyInfer is to build the layer underneath a machine learning framework.

Instead of only using:

```python
model(input)
```

this project explores what that call actually requires internally.

Which makes the project relevant to ML systems, AI infrastructure, inference engineering, and hardware-aware software engineering.

## Engineering focus

TinyInfer is designed around three engineering goals:

### 1. Correctness

### 2. Performance

### 3. Runtime and graph optimisation




## Planned features

### Version 1: Basic C++ inference runtime

* [ ] Tensor class
* [ ] ONNX model loading
* [ ] Read model graph metadata from ONNX
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
* [ ] Benchmark before and after graph optimisation.

