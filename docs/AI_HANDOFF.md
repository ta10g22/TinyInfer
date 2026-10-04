# AI Handoff

## Shared Benchmark Helpers (2026-10-04)
- Added Headers/benchmark_helpers.h and Source/benchmark_helpers.cpp in the existing beginner-readable style: bool load_csv(path, values) fills a vector using getline/stof; bool check_output(actual, count, expected) checks length, finite values and absolute tolerance 0.001, and reports the result.
- TinyInfer's load_input delegates CSV reading to load_csv while retaining its own shape/map handling. Both mains use load_csv for references and check_output for comparison. ORT tensor setup and both inference loops are unchanged.
- Updated both make targets to link the shared source. CSV helper intentionally supports the single-row numeric files produced by the existing scripts, not general quoted/multiline CSV.
- Built both programs; only the pre-existing ReLU signed/unsigned loop warning remains. TinyInfer ran successfully with its current tiny-model CSVs. ORT passed using a temporary large-model fixture from saved PyTorch weights; project models/CSVs and benchmark results were untouched.
- Temporary helper tests passed for CRLF CSV parsing, repeated loading without appending, missing file, tolerance match/mismatch, output-count mismatch, NaN and infinity. Existing tracked Tinyinfer executable was preserved by building the verification binary under /tmp.

## Simplified ORT Benchmark (2026-10-04)
- User requested minimal additions in their original coding style. Removed load_csv helper, stringstream parsing, try/catch, generic type/dimension checks and allocated I/O name lookup from Source/onnx_benchmark.cpp.
- CSVs are read directly in main with getline/stof like TinyInfer (single-row numeric CSVs from the existing export scripts). Model-derived shape and fixed exported names input/output feed the ORT tensor and Run calls.
- Retained one CPU thread, 100 warmups, 10,000 timed runs and a separate output check after timing. Only basic file-open, output-count and finite/tolerance checks remain; input must match the selected model. Other session options use defaults.
- Rebuilt with make onnx_benchmark successfully without warnings. Did not rerun inference in this revision or change the CSVs; user will regenerate matching large-model data. Prior revision's temporary-fixture runtime tests are documented below.

## ONNX Runtime Benchmark (2026-10-04)
- Implemented Source/onnx_benchmark.cpp at the user's request: numeric CSV loading, model-derived static float32 input shape and I/O names, borrowed input tensor, output comparison (absolute tolerance 0.001 with size and finite-value checks), 100 warmups and 10,000 timed Run calls.
- Uses CPU execution with one intra-op thread, sequential execution and all graph optimizations. Loading, tensor setup and validation are outside timing; timed calls include returned output allocation/destruction.
- Added separate `make onnx_benchmark` target linked against native Homebrew ORT under /opt/homebrew/opt/onnxruntime (ORT_PREFIX can override). Run `./onnx_benchmark` from the project root.
- Builds without warnings. Passed inference against a temporary PyTorch fixture using the saved large-model weights; rejected a NaN reference, wrong output count and the currently mismatched input CSV. Temporary fixtures were removed; project CSVs and model files were not changed.
- Default benchmark model is Models/large_mlp.onnx. User plans to regenerate matching CSVs; current project CSVs contain the tiny-model inputs. No official timing added to results.csv.

## Tiny MLP NEON Result (2026-10-03)
- Confirmed Source/Tinyinfer.cpp selects Models/tiny_mlp.onnx and averages 10,000 iterations.
- Recorded user-reported tiny_mlp ARM NEON average 2.49099 us with reported correctness pass; speedup versus 13.4596-us baseline is 5.40331.
- This measurement is approximately 4.46% slower than the earlier 2.3847-us cache-blocked result; repeat measurements before concluding a regression.
- Preserved other CSV results. No source changes or independent benchmark rerun.

## Large MLP NEON Result (2026-10-03)
- Confirmed Source/Tinyinfer.cpp selects Models/large_mlp.onnx and averages 10,000 run_graph calls.
- Recorded user-reported ARM NEON average 624.824 us; user's output reports the correctness check passed. No independent benchmark or validation rerun.
- Speedup versus original large-model baseline 21689.5 us: 34.71298. Versus previous cache-blocked result 1125.13 us: approximately 1.80072.
- Updated only the large_mlp ARM NEON measurement in Benchmarks/results.csv; previous measurements preserved. No source edits.

## Simplified Cumulative Benchmark Rows (2026-09-18)
- User requested one Memory reuse row instead of Reduced tensor copies, Preallocated output buffers, and Tensor lifetime buffer reuse for each model.
- Removed All optimizations rows: retained improvements accumulate, so the final TinyInfer variant already represents the combined configuration. Precomputed execution plan remains last.
- Preserved all existing timing and speedup values. Only CSV/checklist documentation changed; no benchmarks rerun.

## Profiling and Remaining Benchmark Work (2026-09-18)
- Next: profile current optimized execution on both models before further optimization; CSV timings alone do not explain hotspots. Preserve old variants to reproduce and profile earlier experiments.
- Retained fusion rows; replaced blank Memory reuse rows with Preallocated output buffers. Added Reduced tensor copies, Precomputed execution plan, and Tensor lifetime buffer reuse for both models. Existing measurements preserved.
- These stages target redundant copies, repeated allocations, per-inference string/map dispatch, and reusing storage after a tensor's last consumer, respectively.
- User intends sequential optimization. Record exact enabled variants and threads for each run; CSV speedup remains relative to each model's original unoptimized baseline, not the preceding row. Do not assume a multithreaded cumulative variant has one thread.
- Deferred constant folding and dead-code elimination to dedicated test graphs with constant-only operations and unused branches; the current MLPs do not exercise them. Keep correctness tests and CI on the project checklist, not as timed inference variants.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. Read current CSV/handoff and applied targeted patch; no source changes or benchmarks run.

## Tiny MLP NMK Speedup (2026-09-10)
- Filled the missing tiny_mlp NMK speedup with 5.18081, calculated as 13.4596 / 2.59797.
- Read the CSV and patched the single missing cell; no timings changed or benchmarks rerun.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md.

## Cache-Blocked Tiny MLP Result (2026-09-10)
- Recorded user-reported tiny_mlp cache-blocked average 2.3847 us, with their correctness comparison passing.
- Speedup versus 13.4596-us baseline: 5.64415. Time reduction versus 2.59797-us NMK measurement: approximately 8.2%; repeated measurements needed to distinguish small differences from noise.
- Read results.csv and patched the tiny-model cache-blocking row. Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. No benchmark rerun or kernel changes.

## Cache-Blocked Large MLP Result (2026-09-10)
- Recorded user-reported cache-blocked large_mlp average: 1125.13 us, with their PyTorch comparison passing.
- Speedup against the 21689.5-us unoptimized baseline is 19.27733; against the earlier 3788.15-us NMK result it is 3.36686.
- Read results.csv and patched its large-model cache-blocking row. Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. No benchmark rerun or kernel edits.

## Large MLP Loop-Ordering Result (2026-09-09)
- Recorded user-reported N-M-K Gemm with -O3: 3788.15 us average over 10,000 runs; PyTorch comparison passed according to user output.
- Speedup versus the unoptimized large-model baseline is 5.72562; versus the original -O3 result it is 1.27887.
- Read results.csv and patched its loop-ordering row. Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. No benchmark rerun or kernel changes.

## Large MLP Unoptimized Baseline (2026-09-09)
- Recorded user-reported unoptimized large_mlp average of 21689.5 microseconds over 10,000 iterations; their correctness check passed.
- Baseline speedup is 1.0. Updated existing O3 result (4844.56 microseconds) to 4.47708 speedup against this same-model baseline.
- Read Benchmarks/results.csv and applied the two-row update; no benchmark rerun. Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md.

## Large MLP Measurement (2026-09-09)
- Recorded user-reported large_mlp average 4844.56 microseconds, with their PyTorch correctness check passing.
- Current makefile enables -O3, so recorded in Compiler O3 row. Unoptimized large-model baseline is not measured; speedup left blank.
- Read CSV and checked CXXFLAGS, then patched Benchmarks/results.csv and this handoff. No benchmark rerun.

## Restored Tiny-Model Measurements (2026-09-09)
- Restored previously recorded tiny_mlp results after the user noticed blank cells: baseline 13.4596 us / 1.0 speedup; O3 2.62761 us / 5.12237 speedup.
- These cells were already blank when the missing large_mlp rows were restored earlier; the cause of their removal is unknown.
- Read Benchmarks/results.csv with sed and restored only the two measurement rows using apply_patch. No benchmarks rerun. Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md.

## Readiness Recheck (2026-09-09)
- Confirmed load_weights declaration, false returns on failure, tiny-model input CSV export, and corrected Scripts/large_mlp.py filename.
- `make TARGET=/tmp/tinyinfer-review-fixed` passed with two signed/unsigned comparison warnings.
- Both export scripts passed py_compile with PYTHONPYCACHEPREFIX=/tmp/tinyinfer-review-pycache.
- Large model and input CSV are not generated yet, so large-model inference and numerical correctness remain untested. User can run the large export, make, and ./Tinyinfer from the project root.
- No implementation changes; only this handoff was updated.

## Large-Model Readiness Review (2026-09-09)
- Reviewed Source/Tinyinfer.cpp, Source/helpers.cpp, Source/Kernels.cpp, both headers, Scripts/tiny_mlp.py and Scripts/ large_mlp.py without modifying implementation files.
- `make TARGET=/tmp/tinyinfer-review` fails: load_weights is not declared in helpers.h.
- load_weights returns 1 on both failure paths; because it returns bool this incorrectly reports success. Change these to false.
- tiny_mlp.py does not export Models/model_input.csv, so switching back can leave large-model inputs in use. Large script exports matching model/input/reference correctly by inspection.
- Large script filename has a leading space: Scripts/ large_mlp.py.
- Both Python scripts passed syntax checking with PYTHONPYCACHEPREFIX=/tmp/tinyinfer-review-pycache .venv/bin/python3.12 -m py_compile. No model exports or inference tests were run.
- Current main selects large_mlp.onnx. Kernel signatures, transposed-weight indexing, row-wise softmax, and repeated output replacement match these MLPs by inspection. No numerical validation of the large model yet.
- Only docs/AI_HANDOFF.md changed for this review; user requested findings, not fixes or additional protection.

## Input Loader Helper (2026-09-09)
- Added bool load_input(modelproto, Tensor_map) to Source/helpers.cpp and declared it in Headers/helpers.h.
- Reads Models/model_input.csv into a Tensor using the model's single fixed input shape and name; checks file opening, positive dimensions and value count before inserting.
- User requested call-site code separately; Source/Tinyinfer.cpp still has an incomplete input block referencing an undeclared input_tensor. Replace that block with if(!load_input(modelproto, Tensor_map)){ return 1; } before warm-ups.
- Syntax check passed: g++ -std=c++20 -IHeaders -I/Users/admin/vcpkg/installed/arm64-osx/include -DONNX_NAMESPACE=onnx -DONNX_ML=1 -fsyntax-only Source/helpers.cpp.
- Full executable was not built or run. Malformed CSV conversion still uses stof exceptions; dynamic shapes and multiple model inputs are unsupported.
- Files touched: Source/helpers.cpp, Headers/helpers.h, docs/AI_HANDOFF.md.

## Restored Large MLP Rows (2026-09-09)
- Restored the 12 missing large_mlp benchmark rows at the user's request (32x256, 10,000 iterations, no measurements).
- Existing tiny_mlp rows were left as found; their earlier baseline and O3 measurements are now blank in the CSV.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. Read CSV with sed, then applied the restoration patch. No model or benchmark execution changes.

## Large MLP Benchmark Skeleton (2026-09-09)
- Added large_mlp rows for each existing benchmark variant with planned input shape 32x256 and 10,000 iterations. Measurement fields remain blank.
- Preserved all tiny_mlp rows and measurements. Large-model speedups must use the large-model baseline.
- User will create the model themselves; no model or runtime files changed.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. Read CSV with sed and applied additions using apply_patch.

## Latest O3 Result
- Recorded the user's latest reported O3 average: 2.62761 microseconds over 10,000 iterations; the user reported the PyTorch comparison passed.
- Speedup versus the 13.4596-microsecond unoptimized baseline is 5.12237.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md. Read the CSV with sed and applied the result patch; no benchmark rerun performed.

## Latest Benchmark Record
- Recorded the user's reported 10,000-run baseline: 13.4596 microseconds per inference, speedup 1.0, in Benchmarks/results.csv.
- Current Makefile has no optimization flag. Provided CXXFLAGS with -O3 for the user to apply; did not change the Makefile or run another benchmark.
- Read the CSV with sed; applied a one-row patch. Next: force rebuild with make -B after changing flags, then run ./Tinyinfer and verify correctness.
- Files touched: Benchmarks/results.csv and docs/AI_HANDOFF.md.

## Latest Build Update
- Added Source/helpers.cpp to makefile SOURCES and Headers/helpers.h to target dependencies.
- Read makefile, Source/helpers.cpp, Headers/helpers.h, and relevant Tinyinfer.cpp calls.
- Ran `make TARGET=/tmp/tinyinfer-helpers-check`; compilation failed on existing source issues.
- Next: pass modelproto and Tensor_map to run_graph() in main; qualify vector as std::vector in helpers.h; match the lowercase helpers.h include in helpers.cpp.
- Files changed for this update: makefile and docs/AI_HANDOFF.md. No source fixes or benchmark changes made.

## Current State
TinyInfer has a compilable demo forward pass in `Experiments/forwardpass.cpp`, but its current CSV loading bug makes the third output column incorrect. The tracked `Experiments/Demo_forwardpass.cpp` file is currently deleted in the working tree and appears to have been replaced by the untracked `Experiments/forwardpass.cpp`.

## Last Completed Work
- Parsed model weights from CSV/text file.
- Created input matrix manually.
- Applied linear layer.
- Applied ReLU.
- Printed output matrix.

## Known Issues
- CSV parsing drops the final field in every row.
- Linear-layer output dimensions are hard-coded and input dimensions are not validated.
- The documented root-level run command cannot find the weights file.
- `Softmax` is currently stubbed in `Experiments/forwardpass.cpp` and does not return a value.
- Need cleaner separation between experiment code and reusable engine code.

## Next Suggested Steps
1. Fix CSV parsing and add a small loader correctness test.
2. Derive and validate matrix dimensions in the linear layer.
3. Make weight loading independent of the process working directory.
4. Move layer functions into reusable source/header files after the forward pass is correct.

## Latest Readiness Scan (2026-07-02)
- Read `docs/PROJECT_CONTEXT.md`, `docs/AI_HANDOFF.md`, the complete `README.md`/`Readme.md`, `AGENTS.md`, and `CLAUDE.md`.
- Scanned the repository layout and current git state without changing implementation code.
- Confirmed the current source file is `Experiments/forwardpass.cpp`, which is untracked. The tracked `Experiments/Demo_forwardpass.cpp`, root `main`, and root `main.cpp` are deleted in the working tree.
- Confirmed `Experiments/forwardpass.cpp` compiles with C++17 from `Experiments/`, but warning-enabled compilation reports 13 warnings: signed/unsigned loop comparisons plus a missing return from the stubbed `Softmax` function.
- Ran the current `Experiments/forward` binary from `Experiments/`. It prints the incorrect all-zero third output column caused by the CSV parsing bug.
- No implementation files were changed during this scan.

## Latest Review (2026-06-24)
- Read `docs/PROJECT_CONTEXT.md`, `docs/AI_HANDOFF.md`, and the complete README before reviewing the repository.
- Reviewed the current C++ experiment without changing implementation code.
- Confirmed that `Experiments/Demo_forwardpass.cpp` compiles with C++17 and runs when launched from `Experiments/`.
- Confirmed a correctness bug in the CSV parser: it drops the final value in every row because a value is only appended when a comma is encountered. The demo therefore prints an incorrect all-zero third output column.
- Confirmed that the linear layer output shape is hard-coded to `3 x 3` and that matrix dimensions are not validated before indexing. Other valid matrix sizes can produce a wrong shape or out-of-bounds access.
- Confirmed that the documented root-level run command fails because the program resolves `model_weights.csv` relative to the process working directory.
- The warning-enabled build succeeds with 12 signed/unsigned comparison warnings.
- The README's CMake/test/inference commands describe the planned repository rather than the current files; no `CMakeLists.txt`, test target, or runtime target exists yet.
- The worktree already contains deleted tracked files (`main`, `main.cpp`) and untracked instruction/context files. These were preserved.

## Files Touched
- `docs/AI_HANDOFF.md`

## Commands Run
- `pwd`
- `git status --short`
- `sed -n '1,220p' docs/PROJECT_CONTEXT.md`
- `sed -n '1,260p' docs/AI_HANDOFF.md`
- `sed -n '1,240p' README.md`
- `sed -n '241,520p' README.md`
- `sed -n '1,220p' AGENTS.md`
- `sed -n '1,220p' CLAUDE.md`
- `rg --files -g '!build/**' -g '!node_modules/**'`
- `sed -n '1,260p' Experiments/Demo_forwardpass.cpp` (failed because the file is currently deleted)
- `sed -n '1,120p' Experiments/model_weights.csv`
- `sed -n '1,280p' Experiments/forwardpass.cpp`
- `git ls-files`
- `git diff --stat`
- `git diff -- Experiments/Demo_forwardpass.cpp main.cpp Readme.md README.md`
- `ls -la`
- `ls -la Experiments`
- `g++ -std=c++17 -Wall -Wextra -Wpedantic forwardpass.cpp -o /tmp/tinyinfer-forwardpass-check` (from `Experiments/`)
- `./forward` (from `Experiments/`)
- `sed`/`nl` reads of `docs/PROJECT_CONTEXT.md`, `docs/AI_HANDOFF.md`, `README.md`, `AGENTS.md`, `CLAUDE.md`, `Experiments/Demo_forwardpass.cpp`, and `Experiments/model_weights.csv`
- `rg --files -g '!build/**' -g '!node_modules/**'`
- `git status --short`
- `git diff --stat`
- `git diff -- main.cpp main README.md Readme.md`
- `git branch --show-current`
- `git log -5 --oneline --decorate`
- `git ls-files`
- `git show HEAD:main.cpp`
- `g++ -std=c++17 -Wall -Wextra -Wpedantic Experiments/Demo_forwardpass.cpp -o /tmp/tinyinfer-review`
- `g++ -std=c++17 -Wall -Wextra -Wpedantic Demo_forwardpass.cpp -o /tmp/tinyinfer-review` (from `Experiments/`)
- `/tmp/tinyinfer-review`
- `cmake -S . -B /tmp/tinyinfer-cmake-review` (failed because CMake is not installed in the current environment)

## Current Errors / Next Steps
- Fix CSV parsing so the last field of each row is retained, then add a small loader correctness test. For the current data, the linear output's third column should be approximately `[-92.20, 27.64, 124.94]`, not all zeros.
- Either restore/rename the tracked demo path or intentionally stage the rename from `Experiments/Demo_forwardpass.cpp` to `Experiments/forwardpass.cpp`.
- Complete or remove the stubbed `Softmax` function before treating warning-enabled builds as clean.
- Derive output dimensions from the input and weight matrices and reject incompatible or ragged matrices.
- Make weight-file resolution independent of the process working directory, or update the documented command accordingly.
- Decide whether the README should clearly label its CMake commands as future/planned until that structure exists.
- Confirm whether the existing deletions of tracked `main` and `main.cpp` are intentional before committing future work.
