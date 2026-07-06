import sys
from pathlib import Path 

import onnx
from onnx import numpy_helper


def get_tensor_shape(value_info):
    # Extract tensor shape.
    shape = []

    for dim in value_info.type.tensor_type.shape.dim:
        if dim.dim_value:
            shape.append(dim.dim_value)
        elif dim.dim_param:
            shape.append(dim.dim_param)
        else:
            shape.append("?")
    
    return shape


def get_tensor_dtype(value_info):
    # Convert ONNX dtype enum into readable text.
    elem_type = value_info.type.tensor_type.elem_type
    return onnx.TensorProto.DataType.Name(elem_type)


def print_model_info(model):
    # Print basic ONNX version info.
    print("MODEL INFO")
    print("----------")
    print(f"IR version: {model.ir_version}")

    for opset in model.opset_import:
        domain = opset.domain if opset.domain else "ai.onnx"
        print(f"Opset: {domain} version {opset.version}")

    print()


def print_inputs(graph):
    # Initializers are weights, not real runtime inputs.
    initializer_names = {init.name for init in graph.initializer}

    print("MODEL INPUTS")
    print("------------")

    for input_tensor in graph.input:
        if input_tensor.name in initializer_names:
            continue

        print(f"Name: {input_tensor.name}")
        print(f"Shape: {get_tensor_shape(input_tensor)}")
        print(f"Dtype: {get_tensor_dtype(input_tensor)}")
        print()


def print_outputs(graph):
    # Print final model outputs.
    print("MODEL OUTPUTS")
    print("-------------")

    for output_tensor in graph.output:
        print(f"Name: {output_tensor.name}")
        print(f"Shape: {get_tensor_shape(output_tensor)}")
        print(f"Dtype: {get_tensor_dtype(output_tensor)}")
        print()


def print_nodes(graph):
    # Nodes are the operations TinyInfer needs kernels for.
    print("GRAPH NODES")
    print("-----------")

    required_kernels = set()

    for index, node in enumerate(graph.node):
        required_kernels.add(node.op_type)

        print(f"Node {index}")
        print(f"Operator: {node.op_type}")
        print(f"Name: {node.name if node.name else '(no name)'}")
        print(f"Inputs: {list(node.input)}")
        print(f"Outputs: {list(node.output)}")

        if node.attribute:
            print("Attributes:")
            for attr in node.attribute:
                value = onnx.helper.get_attribute_value(attr)
                print(f"  {attr.name}: {value}")

        print()

    print("REQUIRED TINYINFER KERNELS")
    print("--------------------------")

    for op_type in sorted(required_kernels):
        print(f"- {op_type}")

    print()


def print_initializers(graph):
    # Initializers are stored weights and biases.
    print("WEIGHTS / INITIALIZERS")
    print("----------------------")

    for initializer in graph.initializer:
        array = numpy_helper.to_array(initializer)

        print(f"Name: {initializer.name}")
        print(f"Shape: {list(array.shape)}")
        print(f"Dtype: {array.dtype}")

        # Show only first few values.
        print(f"First values: {array.flatten()[:8]}")
        print()


def main():
    # Use path from command line, or default to tiny_mlp.onnx.
    if len(sys.argv) > 1:
        model_path = Path(sys.argv[1])
    else:
        model_path = Path("Models/tiny_mlp.onnx")

    # Load and validate ONNX model.
    model = onnx.load(model_path)
    onnx.checker.check_model(model)

    graph = model.graph

    print(f"Loaded ONNX model: {model_path}")
    print()

    print_model_info(model)
    print_inputs(graph)
    print_outputs(graph)
    print_nodes(graph)
    print_initializers(graph)


if __name__ == "__main__":
    main()