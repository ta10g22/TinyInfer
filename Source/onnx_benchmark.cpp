#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <cstdint>
#include <onnxruntime_cxx_api.h>

#include "benchmark_helpers.h"

using namespace std;

int main(){
    //onnx model location
    string modelpath = "Models/large_mlp.onnx";

    // Runtime Env
    Ort::Env env;

    // Configure
    Ort::SessionOptions options;
    options.SetIntraOpNumThreads(1);

    //load and prepare the model once
    Ort::Session session(env, modelpath.c_str() ,options);

    //load input values and expected results
    vector<float> input_values;
    vector<float> expected_output;
    if(!load_csv("Models/model_input.csv", input_values)){
        return 1;
    }
    if(!load_csv("Models/pytorch_output.csv", expected_output)){
        return 1;
    }

    //get the input dimensions and wrap the values in an ORT tensor
    auto input_type = session.GetInputTypeInfo(0);
    vector<int64_t> input_shape = input_type.GetTensorTypeAndShapeInfo().GetShape();
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    auto input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, input_values.data(), input_values.size(),
        input_shape.data(), input_shape.size());

    //names used when exporting our models
    const char* input_names[] = {"input"};
    const char* output_names[] = {"output"};
    Ort::RunOptions run_options;

    //warm up before timing
    for(int i = 0; i < 100; i++){
        session.Run(run_options, input_names, &input_tensor, 1, output_names, 1);
    }

    //time the runtime output also for 10000 iterations
    auto inference_start = chrono::steady_clock::now();
    for(int i =0 ; i < 10000; i++){
        session.Run(run_options, input_names, &input_tensor, 1, output_names, 1);
    }
    auto inference_duration = chrono::duration<double, micro>(chrono::steady_clock::now() - inference_start).count();

    auto average_time = inference_duration/10000;

    //run once outside timing to check correctness
    auto outputs = session.Run(run_options, input_names, &input_tensor, 1, output_names, 1);
    const float* actual = outputs[0].GetTensorData<float>();
    size_t output_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();
    if(!check_output(actual, output_size, expected_output)){
        return 1;
    }

    //print results
    cout << "average inference time :" << average_time << " microseconds" <<'\n';
    return 0;
}
