#include <iostream> 
#include <fstream>
#include <kernel.h>
#include <onnx/onnx_pb.h>
#include <cstdint>


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
};

int main() {
    //store all suported kernels by tinyinfer in a set
    unordered_set<string> setofkernels;

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

    if(!modelproto.ParseFromIsStream(&model_file)){
        cerr << "failed to parse ONNX model\n"
        return 1;
    }

    // check if all kernels are supported
    for(int i = 0; i < modelproto.graph().node().size(); i++){
        if(!setofkernels.contains(model.graph().node(i).op_type())){
            cerr << "this runtime doesn't support all the kernels used by the model" ;
            return 1;
        }
    }

    //map to store weight tensors to their respective functions
    unordered_map<string,Tensor> Tensor_map;

    //loop through the initializer collection of tensors and store them in a map
    for(int i = 0; i < modelproto.graph().initializer_size(); i++){
        int Name = modelproto.graph().initializer(i).Name();
        vector<int64_t> Tensor_dimension = modelproto.graph().initializer(i).dims();

        //loop through tensorproto and store the Tensor_vector
        for(int j = 0; j < modelproto.graph().initializer_size(i); j++){
            vector<float> Tensor_vector.push_back(modelproto.graph().initializer(i).raw_data(j));
        }
        if (!Tensor_map.contains(Name)) {
            Tensor_map.insert(Name);
            Tensor_map[Name] = Tensor(Name, Tensor_vector, Tensor_dimension);
        }
        else{
        Tensor_map[Name] = Tensor(Name, Tensor_vector, Tensor_dimension);
        }
    }   

    // call kernels from Kernel.h and time output

    // validate correctness

    //benchmark performance against maybe ONNX runtime

    return 0;
}