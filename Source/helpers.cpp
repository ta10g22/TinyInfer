#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>

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


vector<onnx_node> onnx_optimize(const onnx::ModelProto& modelproto){
    vector<onnx_node> optimized_graph;
    unordered_map<string, int> consumers;
    unordered_set<string> graph_outputs;

    // Count every use of each tensor across the graph.
    for(const auto& node : modelproto.graph().node()){
        for(const auto& input : node.input()){
            if(!input.empty()){
                consumers[input]++;
            }
        }
    }

    // Preserve tensors requested as model outputs.
    for(const auto& output : modelproto.graph().output()){
        graph_outputs.insert(output.name());
    }

    for(int i = 0; i < modelproto.graph().node_size(); i++){
        const auto& node = modelproto.graph().node(i);

        onnx_node operation;
        operation.op_type = node.op_type();
        operation.domain = node.domain();

        for(const auto& input : node.input()){
            operation.input_names.push_back(input);
        }
        for(const auto& output : node.output()){
            operation.output_names.push_back(output);
        }
        for(const auto& attribute : node.attribute()){
            operation.attributes.push_back(attribute);
        }

        if(node.domain().empty() && node.op_type() == "Gemm" &&
           node.output_size() == 1 &&
           i + 1 < modelproto.graph().node_size()){
            const auto& next = modelproto.graph().node(i + 1);
            const string& output = node.output(0);

            if(next.domain().empty() &&
               next.input_size() == 1 && next.output_size() == 1 &&
               !output.empty() && next.input(0) == output &&
               consumers[output] == 1 &&
               graph_outputs.count(output) == 0){

                bool fuse_relu = next.op_type() == "Relu";
                bool fuse_softmax = false;

                // Only fuse explicitly row-wise Softmax on our 2D output.
                if(next.op_type() == "Softmax"){
                    for(const auto& attribute : next.attribute()){
                        if(attribute.name() == "axis" &&
                           attribute.type() == onnx::AttributeProto::INT &&
                           (attribute.i() == 1 || attribute.i() == -1)){
                            fuse_softmax = true;
                        }
                    }
                }

                if(fuse_relu || fuse_softmax){
                    if(fuse_relu){
                        operation.op_type = "Fused_GEMM_ReLU";
                    }
                    else{
                        operation.op_type = "Fused_GEMM_Softmax";
                        for(const auto& attribute : next.attribute()){
                            operation.attributes.push_back(attribute);
                        }
                    }

                    operation.output_names.clear();
                    operation.output_names.push_back(next.output(0));
                    i++;
                }
            }
        }

        optimized_graph.push_back(operation);
    }

    return optimized_graph;
}


void run_graph(const vector<onnx_node>& optimized_graph,
               unordered_map<string, Tensor>& Tensor_map){
    for(const auto& node : optimized_graph){
        string input = node.input_names.at(0);
        string output = node.output_names.at(0);

        if(node.op_type == "Gemm" || node.op_type == "Fused_GEMM_ReLU" || node.op_type == "Fused_GEMM_Softmax" ){
            string weight = node.input_names.at(1);
            string bias = node.input_names.at(2);

            Tensor& input_tensor = Tensor_map.at(input);
            Tensor& weight_tensor = Tensor_map.at(weight);
            Tensor& bias_tensor = Tensor_map.at(bias);

            vector<int64_t> dim = {
                input_tensor.get_tensor_dimension()[0],
                weight_tensor.get_tensor_dimension()[0]
            };

            vector<float> output_vector;

            if(node.op_type == "Fused_GEMM_ReLU"){
                output_vector = Fused_GEMM_ReLU(
                    input_tensor.get_tensor_vector(),
                    input_tensor.get_tensor_dimension(),
                    weight_tensor.get_tensor_vector(),
                    weight_tensor.get_tensor_dimension(),
                    bias_tensor.get_tensor_vector());
            }

            else if(node.op_type == "Fused_GEMM_Softmax"){
                output_vector = Fused_GEMM_Softmax(
                    input_tensor.get_tensor_vector(),
                    input_tensor.get_tensor_dimension(),
                    weight_tensor.get_tensor_vector(),
                    weight_tensor.get_tensor_dimension(),
                    bias_tensor.get_tensor_vector());
            }

            else{
                output_vector = GEMM(
                    input_tensor.get_tensor_vector(),
                    input_tensor.get_tensor_dimension(),
                    weight_tensor.get_tensor_vector(),
                    weight_tensor.get_tensor_dimension(),
                    bias_tensor.get_tensor_vector());
            }

            Tensor_map.erase(output);
            Tensor_map.insert({output, Tensor(output_vector, dim)});
        }
        else if(node.op_type == "Relu" || node.op_type == "Softmax"){
            vector<float> output_vector =
                Tensor_map.at(input).get_const_tensor_vector();
            vector<int64_t> dim =
                Tensor_map.at(input).get_tensor_dimension();

            if(node.op_type == "Relu"){
                ReLU(output_vector);
            }
            else{
                Softmax(output_vector, dim);
            }

            Tensor_map.erase(output);
            Tensor_map.insert({output, Tensor(output_vector, dim)});
        }
        else{
            throw runtime_error("Unsupported operation: " + node.op_type);
        }
    }
}
