import csv
import torch
import torch.nn as nn
import onnx


class LargeMLP(nn.Module):
    def __init__(self):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(256, 512),
            nn.ReLU(),
            nn.Linear(512, 128),
            nn.Softmax(dim=1),
        )

    def forward(self, x):
        return self.net(x)

torch.manual_seed(0)
    
model = LargeMLP()
model.eval()

test_input = torch.randn(32, 256, dtype=torch.float32) 

with open("Models/model_input.csv", "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(test_input.flatten().tolist())
    
with torch.no_grad():
    pytorch_output = model(test_input)

print(pytorch_output)

##save the pytorch model 
torch.save(model.state_dict(),
           "Models/large_ml.pt")

#exporting the model and it's weights in ONNX format!
torch.onnx.export(
    model,
    test_input,
    "Models/large_mlp.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=17
)

#open a csv file in write mode and store pytorch_output
with open("Models/pytorch_output.csv", "w", newline= "") as file:
    writer = csv.writer(file)
    writer.writerow(pytorch_output.flatten().tolist())

#validate exported ONNX file
onnx.model = onnx.load("Models/large_mlp.onnx")
onnx.checker.check_model(onnx.model)

print("Exported Large_mlp.onnx Successfully")
print("Saved PyTorch output to pytorch_output.csv")