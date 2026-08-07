#include <iostream> 
#include <fstream>
#include <kernel.h>

using namespace std;

int main() {
    //store the path of the model tinyinfer will use
    string model_path = "Models/tiny_mlp.onnx";

    //open ONNX file for reading in binary model
    ifstream model_file(model_path, ios::binary);

    // check if all kernels are supported

    //store weights in data structure on stack

    // call kernels from Kernel.h and time output

    // validate correctness

    //benchmark performance against maybe ONNX runtime

    return 0;
}