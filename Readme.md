# TinyInfer

**TinyInfer** is my ML compiler backend i've built from scratch.

The goal of this project is to understand how neural networks written in ML frameworks like pytorch and tensorflow are aggressively optmized to run quicky on our different hardware. 

## Project summary

Most ML projects focus on training a model and reporting accuracy.

TinyInfer focuses on what happens after training:

```text
Train model in PyTorch
        ↓
Export model as ONNX
        ↓
Load ONNX graph and weights in C++
        ↓
Optimize the computation graph
        ↓
Run inference using optimized kernels
        ↓
   Validate output 
        ↓
Benchmark performance
```


Instead of only using:

```python
torch.compile()
```

this project explores what that call actually requires internally.


## Implemented features

### Basic C++ inference runtime

* [ ] ONNX model loading
* [ ] Read model graph metadata from ONNX
* [ ] Linear layer
* [ ] ReLU activation
* [ ] Softmax
* [ ] PyTorch output comparison

### Performance kernels

* [ ] Naive matrix multiplication
* [ ] Cache-blocked matrix multiplication
* [ ] Arm Neon (Simd optimization)
* [ ] Operator fusion
* [ ] Running SIMD accumulator in parallel
* [ ] Register blocking 
* [ ] partial sum reuse across tiles

### Graph optimisations

* [ ] Operator fusion
