# TinyInfer

**TinyInfer** is a small C++ inference runtime with ONNX graph optimizations and hand-written ARM NEON kernels.

I built it to understand how data layout, SIMD execution, graph fusion, and memory allocation affect neural-network inference. The project focuses on the work underneath a machine-learning framework, rather than training accuracy. It executes an optimized sequence of operations using existing C++ kernels; it is not a machine-code-generating compiler backend.

On the larger MLP benchmark, the final implementation reduced recorded inference time from **21.690 ms to 0.262 ms**, approximately **82.68x faster than the original baseline**, including compiler optimization. ONNX Runtime and TorchInductor remain faster on this workload.

## Project workflow

```text
Create a fixed-seed PyTorch model and input
    -> Export the graph and weights as ONNX
    -> Load the graph, weights, and input in C++
    -> Fuse supported adjacent operations
    -> Allocate output tensors once
    -> Execute optimized kernels
    -> Validate against PyTorch and benchmark
```

The included models use randomly initialized weights. They are synthetic inference benchmarks, not trained classifiers.

## Large-model results

The main comparison uses `large_mlp`:

```text
Input [32, 256]
    -> Linear(256, 512)
    -> ReLU
    -> Linear(512, 128)
    -> Softmax along each output row
Output [32, 128]
```

Each reported inference processes the **whole batch of 32**, not one sample. Lower latency is better.

| Implementation | Recorded latency per batch (microseconds) | Speedup over original TinyInfer |
| --- | ---: | ---: |
| TinyInfer original baseline | 21,689.500 | 1.00x |
| TinyInfer final, including tensor buffer reuse | 262.322 | 82.68x |
| ONNX Runtime, C++ API | 118.977 | 182.30x |
| TorchInductor, compiled Python API | 52.310 | 414.64x |

ONNX Runtime is **2.20x faster** and TorchInductor is **5.01x faster** than final TinyInfer in these recorded results. The purpose of the comparison is to measure progress and the remaining gap, not to claim that a small custom runtime replaces either system.

The larger model makes kernel work more visible than the tiny example, but it is still a small fixed-shape MLP. These results do not establish a general ranking across models, batch sizes, or hardware.

### Optimization history

These are successive recorded stages, not independently controlled experiments. Speedups are relative to the original TinyInfer baseline, not the preceding row.

| TinyInfer stage | Recorded latency (microseconds) | Speedup |
| --- | ---: | ---: |
| Original baseline | 21,689.500 | 1.00x |
| Compiler `-O3` | 4,844.560 | 4.48x |
| GEMM loop ordering (NMK) | 3,788.150 | 5.73x |
| Cache-blocked GEMM | 1,125.130 | 19.28x |
| ARM NEON SIMD | 624.824 | 34.71x |
| GEMM + bias + ReLU fusion and operation-record plan | 601.372 | 36.07x |
| GEMM + bias + Softmax fusion | 592.140 | 36.63x |
| Multiple independent SIMD accumulators | 433.081 | 50.08x |
| Register blocking: two outputs together | 313.202 | 69.25x |
| Partial-sum retention across input-feature tiles | 257.434 | 84.25x |
| Tensor buffer reuse, final implementation | 262.322 | 82.68x |

The NMK label describes an earlier experiment, not the final loop order. The ReLU-fusion stage also introduced the operation-record execution plan, so its improvement cannot be attributed to fusion alone.

Tensor buffer reuse did **not demonstrate a clear large-model latency improvement**: the final value is the mean of two runs, 256.491 and 268.153 microseconds, while the preceding stage records one run at 257.434 microseconds. The final comparison uses 262.322 rather than selecting the lowest historical measurement.

Exact recorded values for both model sizes are in [Benchmarks/results.csv](Benchmarks/results.csv). Its `average_us` column contains per-run average latency or a summary of repeated run averages; historical entries mix single runs, means, and medians. The CSV does not retain every raw run or its software versions.

### Benchmark conditions

- Hardware: Apple M3 Pro MacBook Pro, CPU execution, float32.
- Threading: single-threaded TinyInfer; ONNX Runtime and PyTorch configured for one intra-op thread.
- Timing: 100 warmup calls followed by 10,000 timed calls using the same model and input.
- Exclusions: model export, loading, graph preparation, initial buffer allocation, TorchInductor compilation, warmup, and correctness checks are outside the timed loops.
- Interfaces: TinyInfer and ONNX Runtime use C++; TorchInductor uses `torch.compile(..., backend="inductor", fullgraph=True)` through Python under `torch.inference_mode()`.
- Correctness: compare output size and finite values against the PyTorch reference with absolute tolerance `0.001`; this is numerical agreement, not bitwise equality.
- Scope: timings include each benchmark's inference-call overhead, not just matrix multiplication. They are not an identical-API comparison or cold-start measurements.

Measurements were collected incrementally, not as one controlled statistical study. Background activity, CPU frequency, thermal conditions, and cache state can affect timings; these results do not isolate those effects. Small changes between stages should not be treated as conclusive wins or regressions.

## Implemented features

### Runtime and graph execution

- [x] ONNX graph and float32 weight loading.
- [x] Tensor storage and name-based graph execution.
- [x] Linear layers through GEMM, ReLU, and row-wise Softmax for the included models.
- [x] Adjacent GEMM-ReLU and GEMM-Softmax fusion with consumer and graph-output checks.
- [x] Operation records prepared before inference.
- [x] Output tensor buffers allocated once and reused across calls.
- [x] Output validation against PyTorch and separate runtime benchmarks.

### Kernel optimizations

- [x] Cache blocking and loop-order experiments.
- [x] ARM NEON four-lane float32 SIMD.
- [x] Independent accumulators to reduce dependency chains within dot products.
- [x] Two-output register blocking to reuse each loaded input vector across two weight rows.
- [x] Accumulator retention across input-feature tiles, reducing only after the complete dot products.
- [x] Fused GEMM, bias, and activation kernels.

The naive implementation and intermediate variants are historical benchmark stages. The current executable runs the final kernels; it does not offer a switch for every earlier variant.

## Build and run

### Prerequisites

The current build targets Apple Silicon macOS and requires:

- A C++20 compiler with ARM NEON support, `make`, and `pkg-config`.
- ONNX and Protobuf C++ libraries installed through vcpkg for `arm64-osx`.
- The native ONNX Runtime C++ library for its comparison executable.
- Python dependencies listed in [requirements.txt](requirements.txt), with a PyTorch/Python combination that supports the export script and CPU TorchInductor benchmark.

The [makefile](makefile) defaults to vcpkg at `$HOME/vcpkg` and native ONNX Runtime at `/opt/homebrew/opt/onnxruntime`. Override `VCPKG_ROOT`, `VCPKG_PREFIX`, or `ORT_PREFIX` when your installations differ. Python packages alone do not supply the C++ libraries expected by this makefile.

Dependency versions are not pinned, so a fresh environment is not an exact reproduction of the historical benchmark environment.

### 1. Export the large model

Run from the repository root, using your Python environment with the export dependencies:

```sh
python3 Scripts/large_mlp.py
```

This generates the ONNX model, saved PyTorch weights, input CSV, and expected-output CSV.

**Both model export scripts overwrite `Models/model_input.csv` and `Models/pytorch_output.csv`.** Keep these files paired with the model selected in every benchmark. The output CSV contains reference predictions, not weights.

### 2. Select the model in both C++ programs

The programs currently select the tiny model through hardcoded paths; they do not accept a model command-line argument. For the large comparison, set:

In [Source/Tinyinfer.cpp](Source/Tinyinfer.cpp):

```cpp
string model_path = "Models/large_mlp.onnx";
```

In [Source/onnx_benchmark.cpp](Source/onnx_benchmark.cpp):

```cpp
string modelpath = "Models/large_mlp.onnx";
```

### 3. Build and benchmark

```sh
make -B
./Tinyinfer

make -B onnx_benchmark
./onnx_benchmark

python3 Scripts/Pytorch_benchmark.py --model large --threads 1
```

Use the Python environment with working CPU TorchInductor support for the last command; it can be separate from the export environment. The makefile builds C++ with `-O3`; `-B` forces a rebuild so stale binaries or changed compiler flags are not overlooked.

Confirm that each benchmark reports a successful output comparison. Repeat measurements under similar conditions and retain the raw run averages when evaluating small changes.

To run the tiny model instead, export with `Scripts/tiny_mlp.py`, select `Models/tiny_mlp.onnx` in both C++ programs, rebuild, and pass `--model tiny` to the Python benchmark.

## Repository layout

| Path | Purpose |
| --- | --- |
| [Source/Tinyinfer.cpp](Source/Tinyinfer.cpp) | Model loading, preparation, warmup, timing, and validation |
| [Source/Kernels.cpp](Source/Kernels.cpp) | GEMM, activation, and fused NEON kernels |
| [Source/helpers.cpp](Source/helpers.cpp) | Tensor loading, graph fusion, output-buffer preparation, and execution |
| [Source/benchmark_helpers.cpp](Source/benchmark_helpers.cpp) | CSV loading and numerical output checks |
| [Source/onnx_benchmark.cpp](Source/onnx_benchmark.cpp) | ONNX Runtime C++ comparison |
| [Headers](Headers) | Tensor and operation definitions and function declarations |
| [Scripts](Scripts) | Model export, ONNX inspection, and TorchInductor benchmarking |
| [Models](Models) | Model files, input data, and reference outputs |
| [Benchmarks/results.csv](Benchmarks/results.csv) | Recorded results for both model sizes |
| [Assembly](Assembly) | Saved assembly from loop-order experiments |

## Scope and limitations

TinyInfer supports the conventions used by the included float32, two-dimensional MLPs. GEMM computes `input * weights^T + bias`, with weights stored as `[output_features, input_features]` and one bias per output feature. Arbitrary ONNX GEMM attributes, broadcasting, and Softmax axes are not fully implemented or validated.

This is not a general ONNX runtime. It does not implement GPU execution, multithreaded kernels, arbitrary operator coverage, or portable non-NEON kernels. Model paths and benchmark shapes are selected manually. The repository includes end-to-end output checks, but no checked-in standalone kernel unit-test suite.

The main lesson is that SIMD alone is not enough: data reuse, independent work, accumulation order, and allocation overhead all matter. The recorded progression shows substantial improvement over the initial implementation while keeping the remaining gap to established runtimes visible.
