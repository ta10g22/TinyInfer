#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;


// kernel for General Matrix Multiplication
vector<float> GEMM(vector<float>& input_matrix,const vector<int64_t> &input_dim, vector<float>& weight_matrix,const vector<int64_t>& weight_dim, vector<float>& bias){
    int batch_size = input_dim[0];
    int input_features = input_dim[1];
    int output_features = weight_dim[0];
    int64_t block_N = 8;
    int64_t block_K = 32;
    int64_t block_M = 32;

    vector<float> output1(batch_size * output_features, 0.0f);

    for(int64_t NN = 0; NN < batch_size; NN += block_N){
        for(int64_t KK = 0; KK < output_features; KK += block_K){
            for(int64_t MM = 0; MM < input_features; MM += block_M){
                int64_t end_N = min<int64_t>(NN + block_N, batch_size);
                int64_t end_K = min<int64_t>(KK + block_K, output_features);
                int64_t end_M = min<int64_t>(MM + block_M, input_features);

                // Accumulate this block into the output.
                for(int64_t N = NN; N < end_N; N++){
                    for(int64_t K = KK; K < end_K; K++){
                        for(int64_t M = MM; M < end_M; M++){
                            output1[N * output_features + K] +=
                                input_matrix[N * input_features + M] *
                                weight_matrix[K * input_features + M];
                        }
                    }
                }
            }
        }
    }

    // Add bias once, after all blocks have contributed.
    for(int64_t N = 0; N < batch_size; N++){
        for(int64_t K = 0; K < output_features; K++){
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