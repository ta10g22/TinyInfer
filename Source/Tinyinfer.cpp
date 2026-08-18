#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <cstring>

#include <onnx/onnx_pb.h>

#include "Kernels.h"


using namespace std;

//Creating a tensor class
class Tensor{
    private:
    string name;
    vector<float> tensor_vector;
    vector<int64_t> tensor_dimension;

    public:
    //constructor to set tensor fields
    Tensor(string name, vector<float> tensor_vector, vector<int64_t> tensor_dimension){
        this-> name = name;
        this-> tensor_vector = tensor_vector;
        this-> tensor_dimension = tensor_dimension;
    }

    // 2 "const" first one ensure the caller doesn't change the vector while the last one ensure the function doesn't change any member variable of the object
    const vector<float>& get_tensor_vector() const{
        return this->tensor_vector;
    }
};

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
        for(int j = 0; j < number_of_weights + 0 ; j++){
            float weight ;
            memcpy(&weight, raw_data.data() + (j * sizeof(float)), sizeof(float));
            Tensor_vector.push_back(weight);
        }

        //Add the Name and Tensor to the Map (names are unique so no need to check if already in)
        Tensor_map.insert({Name, Tensor(Name, Tensor_vector, Tensor_dimension)});
    }   

    // maybe print to see what values i get out (loop through map we map)
    for (const auto &tensor_pair : Tensor_map){
        cout << "\n" << tensor_pair.first << ":" ;

        const vector<float>& Tensor_vector = tensor_pair.second.get_tensor_vector();

        for(int x = 0; x < Tensor_vector.size(); x++){
            cout << Tensor_vector[x] << ", " ;
        }
    }
    
    // call kernels from Kernel.h and time output

    // validate correctness

    //benchmark performance against maybe ONNX runtime

    return 0;
}