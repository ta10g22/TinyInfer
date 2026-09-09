#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <onnx/onnx_pb.h>

using namespace std;

// tensor class
class Tensor{
    private:
    vector<float> tensor_vector;
    vector<int64_t> tensor_dimension;

    public:
    //constructor to set tensor fields
    Tensor(vector<float> tensor_vector, vector<int64_t> tensor_dimension){
        this-> tensor_vector = tensor_vector;
        this-> tensor_dimension = tensor_dimension;
    }

    // there are 2 "const" first one ensure the caller doesn't change the vector while the last one ensure the 
    //function doesn't change any member variable of the object
    const vector<float>& get_const_tensor_vector() const{
        return this->tensor_vector;
    }

    //another getter function for kernels to modify in place
    vector<float>& get_tensor_vector(){
        return this->tensor_vector;
    }

    const vector<int64_t>& get_tensor_dimension() const{
        return this->tensor_dimension;
    }
};

// Implementation stays in helpers.cpp.
bool load_input(
    const onnx::ModelProto& modelproto,
    std::unordered_map<std::string, Tensor>& Tensor_map
);

void run_graph(
    const onnx::ModelProto& modelproto,
    std::unordered_map<std::string, Tensor>& Tensor_map
);
