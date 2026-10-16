import argparse
import csv
import time

import torch
import torch.nn as nn


class MLP(nn.Module):
    def __init__(self, input_features, hidden_features, output_features):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_features, hidden_features),
            nn.ReLU(),
            nn.Linear(hidden_features, output_features),
            nn.Softmax(dim=1),
        )

    def forward(self, x):
        return self.net(x)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", choices=["tiny", "large"], required=True)
    parser.add_argument("--threads", type=int, default=1,
                        help="CPU thread count; 0 keeps PyTorch's default")
    args = parser.parse_args()

    if args.threads < 0:
        parser.error("--threads must be zero or positive")
    if args.threads > 0:
        torch.set_num_threads(args.threads)

    # Select the architecture and weights saved by the export scripts.
    if args.model == "tiny":
        model = MLP(4, 5, 3)
        weights_path = "Models/tiny_ml.pt"
        input_shape = (1, 4)
    else:
        model = MLP(256, 512, 128)
        weights_path = "Models/large_ml.pt"
        input_shape = (32, 256)

    model.load_state_dict(torch.load(weights_path, map_location="cpu", weights_only=True))
    model.eval()

    # Read the same input and reference CSVs used by the C++ benchmarks.
    with open("Models/model_input.csv", newline="") as file:
        input_values = [float(value) for value in next(csv.reader(file))]
    with open("Models/pytorch_output.csv", newline="") as file:
        expected_values = [float(value) for value in next(csv.reader(file))]

    if len(input_values) != input_shape[0] * input_shape[1]:
        parser.error("Input CSV does not match the selected model. Run its export script first.")

    input_tensor = torch.tensor(input_values, dtype=torch.float32).reshape(input_shape)
    expected_output = torch.tensor(expected_values, dtype=torch.float32)

    # fullgraph=True reports unsupported compilation instead of splitting the graph.
    compiled_model = torch.compile(model, backend="inductor", fullgraph=True)

    with torch.inference_mode():
        print("Compiling and checking the model...", flush=True)
        output = compiled_model(input_tensor)
        if not torch.isfinite(output).all() or not torch.isfinite(expected_output).all():
            raise ValueError("Output or reference contains non-finite values")
        torch.testing.assert_close(output.flatten(), expected_output, rtol=0, atol=0.001)
        print("Final output and model output match successfully", flush=True)

        # Compilation and warmup are outside the timed loop.
        for i in range(100):
            compiled_model(input_tensor)

        inference_start = time.perf_counter()
        for i in range(10000):
            compiled_model(input_tensor)
        inference_duration = time.perf_counter() - inference_start

    average_time = inference_duration * 1_000_000 / 10000
    print(f"PyTorch {torch.__version__}, compiled CPU, Python API")
    print(f"model: {args.model}, threads: {torch.get_num_threads()}, iterations: 10000")
    print(f"average inference time :{average_time:.5f} microseconds")


if __name__ == "__main__":
    main()
