import csv

import torch
import torch.nn as nn
import onnx

class TinyMLP(nn.Module):
    def __init__(self):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(4, 5),
            nn.ReLU(),
            nn.Linear(5, 3),
            nn.Softmax(dim=1),
        )
    
    def forward(self, x):
        return self.net(x)

torch.manual_seed(0)
    
model = TinyMLP()
model.eval()

test_input = torch.tensor(
    [[0.5, -1.2, 0.8, 2.0]],
    dtype=torch.float32
)

#make a csv file with the test input so tinyinfer can read from it
with open("Models/model_input.csv", "w", newline="") as file:
    csv.writer(file).writerow(test_input.flatten().tolist())

with torch.no_grad():
    pytorch_output = model(test_input)

print(pytorch_output)

##save the pytorch model 
torch.save(model.state_dict(),
           "Models/tiny_ml.pt")

#exporting the model and it's weights in ONNX format!
torch.onnx.export(
    model,
    test_input,
    "Models/tiny_mlp.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=17
)

#open a csv file in write mode and store pytorch_output
with open("Models/pytorch_output.csv", "w", newline= "") as file:
    writer = csv.writer(file)
    writer.writerow(pytorch_output.flatten().tolist())

#validate exported ONNX file
onnx.model = onnx.load("Models/tiny_mlp.onnx")
onnx.checker.check_model(onnx.model)

print("Exported tiny_mlp.onnx Successfully")
print("Saved PyTorch output to pytorch_output.csv")