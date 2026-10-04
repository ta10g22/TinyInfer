#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <chrono>

#include <onnx/onnx_pb.h>

#include "Kernels.h"
#include "helpers.h"
#include "benchmark_helpers.h"
using namespace std;


int main() {
    //store all suported kernels by tinyinfer in a set
    unordered_set<string> setofkernels;

    //add tiny infer functions into setofkernels SET
    setofkernels.insert("Gemm");
    setofkernels.insert("Relu");
    setofkernels.insert("Softmax");

    //store the path of the model tinyinfer will use.  <----- LOAD MODEL
    string model_path = "Models/tiny_mlp.onnx";

    //open ONNX file for reading in binary mode
    ifstream model_file(model_path, ios::binary);

    if(!model_file.is_open()){
        cerr << "ONNX model file didn't load properly\n" ;
        return 1;
    }

    //put the binary stream file through onnx modelproto parser
    onnx::ModelProto modelproto;

    if(!modelproto.ParseFromIstream(&model_file)){
        cerr << "failed to parse ONNX model\n";

        return 1;
    }

    // check if all kernels are supported
    for(int i = 0; i < modelproto.graph().node().size(); i++){
        if(!setofkernels.contains(modelproto.graph().node(i).op_type())){
            cerr << "this runtime doesn't support all the kernels used by the model" ;
            return 1;
        }
    }

    //map weight names to weight tensors
    unordered_map<string,Tensor> Tensor_map;

    //load weights for onnx model into the  tensor map so we can run inference using our optimized kernels
    if(!load_weights(modelproto, Tensor_map)){
        return 1;
    }
                        
    // Load the input tensor before running inference.   <------parse input for the model you're doing inference on!
    if(!load_input(modelproto, Tensor_map)){
    return 1;
    }

    //warm up (to ensure code and data are already in CPU caches
    for(int i = 0; i < 100; i++){
        run_graph(modelproto, Tensor_map);
    }

    // run inference on model 10,000 times 
    auto inference_start = chrono::steady_clock::now();
    for(int i = 0; i < 10000 ; i++){
        run_graph(modelproto, Tensor_map);
    }
    auto inference_duration = chrono::duration<double, micro>(chrono::steady_clock::now() - inference_start).count() ;
     
    auto average_time = inference_duration/10000;

    // validate correctness
    string final_output_name = modelproto.graph().output(0).name();
    const vector<float> final_output  = Tensor_map.at(final_output_name).get_const_tensor_vector();

    //load expected results and check correctness
    vector<float> expected_output;
    if(!load_csv("Models/pytorch_output.csv", expected_output)){
        return 1;
    }
    if(!check_output(final_output.data(), final_output.size(), expected_output)){
        return 1;
    }

    //print results
    cout << "average inference time :" << average_time << " microseconds" <<'\n';

    return 0;
}
