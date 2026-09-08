#include <vector>
#include <cmath>
using namespace std;


// kernel for General Matrix Multiplication
vector<float> GEMM(vector<float>& input_matrix,const vector<int64_t> &input_dim, vector<float>& weight_matrix,const vector<int64_t>& weight_dim, vector<float>& bias){

    int batch_size = input_dim[0];
    int input_features = input_dim[1];
    int output_features = weight_dim[0];

    vector<float> output1(batch_size * output_features, 0.0f);

    for(int64_t N = 0; N < batch_size; N++){
        for(int64_t K = 0; K < output_features; K++){
            for(int64_t M = 0; M < input_features; M++){
                output1[N * output_features + K] +=
                    input_matrix[N * input_features + M] *
                    weight_matrix[K * input_features + M];
            }

            output1[N * output_features + K] += bias[K];
        }
    }

    return output1;
}

// Kernel for Relu activation layer
void ReLU(vector<float>& input_matrix2){
    for(int i = 0; i < input_matrix2.size(); i++){
            if(input_matrix2[i] < 0){
                input_matrix2[i]= 0;
            }
    }
}

// Kernel for Softmax layer
void Softmax(vector<float>& input, const vector<int64_t>& dim){
    int64_t rows = dim[0];
    int64_t columns = dim[1];

    for(int64_t i = 0; i < rows; i++){
        float max_value = input[i * columns];

        for(int64_t j = 0; j < columns; j++){
            if(input[i * columns + j] > max_value){
                max_value = input[i * columns + j];
            }
        }

        float sum_exp = 0.0f;
        for(int64_t j = 0; j < columns; j++){
            input[i * columns + j] =
                exp(input[i * columns + j] - max_value);

            sum_exp += input[i * columns + j];
        }

        for(int64_t j = 0; j < columns; j++){
            input[i * columns + j] /= sum_exp;
        }
    }
}