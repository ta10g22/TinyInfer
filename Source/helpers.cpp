#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

#include <onnx/onnx_pb.h>

#include "helpers.h"
#include "Kernels.h"

using namespace std;


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
