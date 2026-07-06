# AI Handoff

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
