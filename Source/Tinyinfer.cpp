#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <cmath>

#include <onnx/onnx_pb.h>

#include "Kernels.h"
#include "helpers.h"
using namespace std;


int main() {
    //store all suported kernels by tinyinfer in a set
    unordered_set<string> setofkernels;

    //add tiny infer functions into setofkernels SET
    setofkernels.insert("Gemm");
    setofkernels.insert("Relu");
    setofkernels.insert("Softmax");

    //store the path of the model tinyinfer will use
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

    //loop through the initializer collection of tensors(weights) and store them in a map
    for(int i = 0; i < modelproto.graph().initializer_size(); i++){

        string Name = modelproto.graph().initializer(i).name();
        int64_t number_of_weights = 1;
        vector<int64_t> Tensor_dimension;
        vector<float> Tensor_vector ={};
        string raw_data = modelproto.graph().initializer(i).raw_data();

        //check that the weight data type is a float
        if(modelproto.graph().initializer(i).data_type() != onnx::TensorProto::FLOAT){
            cerr << "TinyInfer only supports FLOAT tensors\n";
            return 1;
        }

        //find number of weights in initializer and the tensor dimension vector 
        for(int j = 0; j < modelproto.graph().initializer(i).dims_size(); j++){
            number_of_weights *= modelproto.graph().initializer(i).dims(j);
            Tensor_dimension.push_back(modelproto.graph().initializer(i).dims(j));
        }   

        //saftey check to ensure model has the right number of data per tensor
        if(raw_data.size() != number_of_weights * sizeof(float)){
            cerr << "Number of raw datapoints != number of weights " ;
            return 1;
        }

        //copy content of address "weight" from raw_data start, size to copy "float"
        for(int j = 0; j < number_of_weights; j++){
            float weight ;
            memcpy(&weight, raw_data.data() + (j * sizeof(float)), sizeof(float));
            Tensor_vector.push_back(weight);
        }

        //Add the Name and Tensor to the Map (names are unique so no need to check if already in)
        Tensor_map.insert({Name, Tensor(Tensor_vector, Tensor_dimension)});
    }   

    // maybe print to see what values i get out (loop through map we map)
    for (const auto &tensor_pair : Tensor_map){
        cout << "\n" << tensor_pair.first << ":" ;

        const vector<float>& Tensor_vector = tensor_pair.second.get_const_tensor_vector();

        for(int x = 0; x < Tensor_vector.size(); x++){
            cout << Tensor_vector[x] << ", " ;
        }
    }
    
    cout << '\n' ;

    // add input tensor for model (hardcoded for now)(mayber parsed from a CSV in near future)
    Tensor input_tensor({0.5f, -1.2f, 0.8f, 2.0f}, {1, 4});
    Tensor_map.insert({"input",input_tensor});

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

    //open csv file of model results
    ifstream pytorch_output("Models/pytorch_output.csv");

    if (!pytorch_output.is_open()){
        cerr << "pytorch_output file failed to open" << '\n' ;
        return 1;
    }
    
    //read each comma separated value
    vector<float> expected_output;
    string value;

    while(getline(pytorch_output, value, ',')){
        expected_output.push_back(stof(value));
    }

    // check if it passed 
    for(int i = 0; i < final_output.size(); i++){
        if (abs(expected_output[i] - final_output[i]) > 0.001f){
            cerr << "Final output doesn't match the expected output, sorry!" << "\n";
            return 1;
        }
    }
    //passed successfully
    cout << "Final Output and model output match successfully" <<'\n';

    //print results
    cout << "average inference time :" << average_time << " microseconds" <<'\n';

    return 0;
}