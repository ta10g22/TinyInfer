#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <iostream>

#include <onnx/onnx_pb.h>

#include "helpers.h"
#include "benchmark_helpers.h"
#include "Kernels.h"

using namespace std;


bool load_weights(const onnx::ModelProto& modelproto, unordered_map<string, Tensor>& Tensor_map){
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
            return false;
        }

        //find number of weights in initializer and the tensor dimension vector 
        for(int j = 0; j < modelproto.graph().initializer(i).dims_size(); j++){
            number_of_weights *= modelproto.graph().initializer(i).dims(j);
            Tensor_dimension.push_back(modelproto.graph().initializer(i).dims(j));
        }   

        //saftey check to ensure model has the right number of data per tensor
        if(raw_data.size() != number_of_weights * sizeof(float)){
            cerr << "Number of raw datapoints != number of weights " ;
            return false;
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

    return true;
}


bool load_input(const onnx::ModelProto& modelproto, unordered_map<string, Tensor>& Tensor_map){
    vector<float> input_values;
    if(!load_csv("Models/model_input.csv", input_values)){
        return false;
    }

    vector<int64_t> input_dim;
    for(int i = 0; i < modelproto.graph().input(0).type().tensor_type().shape().dim_size(); i++){
        input_dim.push_back(
            modelproto.graph().input(0).type().tensor_type().shape().dim(i).dim_value()
        );
    }

    Tensor input_tensor(input_values, input_dim);
    Tensor_map.insert({modelproto.graph().input(0).name(), input_tensor});
    return true;
}

void run_graph(const onnx::ModelProto& modelproto,  unordered_map<string, Tensor>& Tensor_map ){
    for(int i =0; i < modelproto.graph().node_size(); i++){ 
        if (modelproto.graph().node(i).op_type() == "Gemm") {
            string input = modelproto.graph().node(i).input(0);
            string weight = modelproto.graph().node(i).input(1);
            string bias = modelproto.graph().node(i).input(2);
            string output = modelproto.graph().node(i).output(0);
            
            vector<int64_t> dim = {Tensor_map.at(input).get_tensor_dimension()[0], Tensor_map.at(weight).get_tensor_dimension()[0]};
            
            vector<float> output_vector = GEMM(Tensor_map.at(input).get_tensor_vector(), Tensor_map.at(input).get_tensor_dimension(),
            Tensor_map.at(weight).get_tensor_vector(),Tensor_map.at(weight).get_tensor_dimension(), Tensor_map.at(bias).get_tensor_vector());
            
            Tensor_map.erase(output);
            Tensor_map.insert({output, Tensor(output_vector, dim)}); ;
        }

        else if (modelproto.graph().node(i).op_type() == "Relu"){
            string input = modelproto.graph().node(i).input(0);
            string output = modelproto.graph().node(i).output(0);

            vector<float> output_vector = Tensor_map.at(input).get_const_tensor_vector();

            ReLU(output_vector);

            vector<int64_t> dim = Tensor_map.at(input).get_tensor_dimension();

            Tensor_map.erase(output);
            Tensor_map.insert({output, Tensor(output_vector, dim)});
         }

        else if (modelproto.graph().node(i).op_type() == "Softmax"){
            string input = modelproto.graph().node(i).input(0);
            string output = modelproto.graph().node(i).output(0);

            vector<float> output_vector = Tensor_map.at(input).get_const_tensor_vector();

            Softmax(output_vector, Tensor_map.at(input).get_tensor_dimension());

            vector<int64_t> dim = Tensor_map.at(input).get_tensor_dimension();

            Tensor_map.erase(output);
            Tensor_map.insert({output, Tensor(output_vector, dim)});
        }
    }
}
