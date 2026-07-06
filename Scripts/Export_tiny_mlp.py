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
    

model = TinyMLP()
model.eval()
dummy_input = torch.randn(1, 4)

##save the pytorch model 
torch.save(model.state_dict(), "Models/tiny_ml.pt")

#exporting the model and it's weights in ONNX format!
torch.onnx.export(
    model,
    dummy_input,
    "Models/tiny_mlp.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=17
)

onnx.model = onnx.load("Models/tiny_mlp.onnx")
onnx.checker.check_model(onnx.model)

print("Exported tiny_mlp.onnx Successfully")