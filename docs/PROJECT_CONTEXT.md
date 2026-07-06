# TinyInfer Project Context

## Goal

TinyInfer is a C++ ML inference engine built for learning ML Systems, AI Infrastructure, and inference engineering.

The goal is to understand how model inference works internally: loading weights, running forward passes, handling matrix operations, testing correctness, and later improving performance.

## Current Stage

Currently building the basic runtime.

Current focus:

* Linear layer
* ReLU
* Softmax
* CSV/text weight loading
* simple demo programs in `Experiments/`

Main current demo:

```text
Experiments/Demo_forwardpass.cpp
```

## Build / Run Command

Current command:

```bash
g++ -std=c++17 Experiments/Demo_forwardpass.cpp -o main
./main
```

Update this if the project structure changes.

## Coding Style

Use simple, beginner-readable C++.

Prefer:

* clear variable names
* small functions
* simple loops
* small testable changes

Avoid:

* large rewrites
* over-engineering
* complex C++ tricks
* changing many files at once without a clear reason

## AI Assistant Rules

The user wants guidance, debugging help, hints, and explanations so they can build the project themselves.

Do not write full code solutions unless explicitly asked.

When helping:

1. Explain the issue clearly.
2. Suggest the next small step.
3. Keep changes small and testable.
4. Explain technical terms before relying on them.

## Project Direction

Build TinyInfer in this order:

1. Get basic forward-pass operations working.
2. Clean up experiment code into reusable files.
3. Add simple correctness tests.
4. Add benchmarking.
5. Optimise only after correctness is proven.

## Shared Context Files

Use:

```text
docs/PROJECT_CONTEXT.md
docs/AI_HANDOFF.md
```

`PROJECT_CONTEXT.md` is for stable long-term project direction.

`AI_HANDOFF.md` is for the latest current state, bugs, commands run, files changed, and next steps.

After making changes, update `docs/AI_HANDOFF.md`.
