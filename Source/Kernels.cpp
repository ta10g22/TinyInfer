#include <vector>
#include <cmath>
#include <algorithm>
#include <arm_neon.h>
#include <limits>


using namespace std;


// kernel for General Matrix Multiplication
vector<float> GEMM(vector<float>& input_matrix, const vector<int64_t>& input_dim,
    vector<float>& weight_matrix, const vector<int64_t>& weight_dim,
    vector<float>& bias){
    int batch_size = input_dim[0];
    int input_features = input_dim[1];
    int output_features = weight_dim[0];
    int64_t block_N = 8;
    int64_t block_K = 32;
    int64_t block_M = 32;

    vector<float> output1(batch_size * output_features, 0.0f);

    //keep the outer output tiles, but finish each output pair before moving on
    for(int64_t NN = 0; NN < batch_size; NN += block_N){
        int64_t end_N = min<int64_t>(NN + block_N, batch_size);
        for(int64_t KK = 0; KK < output_features; KK += block_K){
            int64_t end_K = min<int64_t>(KK + block_K, output_features);
            for(int64_t N = NN; N < end_N; N++){
                int64_t K = KK;
                for(; K + 1 < end_K; K += 2){
                    //initialize once, not again for every MM tile
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);

                    //more accumulator register so we can compute 2 outputs in parallel
                    float32x4_t acc4 = vdupq_n_f32(0.0f);
                    float32x4_t acc5 = vdupq_n_f32(0.0f);
                    float32x4_t acc6 = vdupq_n_f32(0.0f);
                    float32x4_t acc7 = vdupq_n_f32(0.0f);
                    float tail0 = 0.0f;
                    float tail1 = 0.0f;

                    //same input row but 2 different weight rows for the 2 outputs
                    const float* input = &input_matrix[N * input_features];
                    const float* weight0 = &weight_matrix[K * input_features];
                    const float* weight1 = &weight_matrix[(K + 1) * input_features];

                    //all input feature tiles contribute to the same output pair
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            //load input once and reuse it with both weight rows
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight0 + M + 4));
                            acc5 = vfmaq_f32(acc5, VA, vld1q_f32(weight1 + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight0 + M + 8));
                            acc6 = vfmaq_f32(acc6, VA, vld1q_f32(weight1 + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight0 + M + 12));
                            acc7 = vfmaq_f32(acc7, VA, vld1q_f32(weight1 + M + 12));
                        }

                        // Process remaining groups of four.
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));
                        }

                        //keep scalar leftovers across MM tiles too
                        for(; M < end_M; M++){
                            float value = input[M];
                            tail0 += value * weight0[M];
                            tail1 += value * weight1[M];
                        }
                    }

                    //all MM tiles finished: combine and reduce only once
                    float sum0 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    float sum1 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc4, acc5), vaddq_f32(acc6, acc7)));
                    output1[N * output_features + K] = sum0 + tail0;
                    output1[N * output_features + K + 1] = sum1 + tail1;
                }

                //calculate the last output separately if the tile has an odd number of columns
                for(; K < end_K; K++){
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);
                    float tail = 0.0f;
                    const float* input = &input_matrix[N * input_features];
                    const float* weight = &weight_matrix[K * input_features];

                    //the leftover output also keeps its sums across all MM tiles
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight + M + 12));
                        }
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));
                        }
                        for(; M < end_M; M++){
                            tail += input[M] * weight[M];
                        }
                    }

                    float sum = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    output1[N * output_features + K] = sum + tail;
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


vector<float> Fused_GEMM_ReLU(vector<float>& input_matrix,const vector<int64_t> &input_dim, vector<float>& weight_matrix,const vector<int64_t>& weight_dim, vector<float>& bias){
    int batch_size = input_dim[0];
    int input_features = input_dim[1];
    int output_features = weight_dim[0];
    int64_t block_N = 8;
    int64_t block_K = 32;
    int64_t block_M = 32;

    vector<float> output1(batch_size * output_features, 0.0f);

    //keep the outer output tiles, but finish each output pair before moving on
    for(int64_t NN = 0; NN < batch_size; NN += block_N){
        int64_t end_N = min<int64_t>(NN + block_N, batch_size);
        for(int64_t KK = 0; KK < output_features; KK += block_K){
            int64_t end_K = min<int64_t>(KK + block_K, output_features);
            for(int64_t N = NN; N < end_N; N++){
                int64_t K = KK;
                for(; K + 1 < end_K; K += 2){
                    //initialize once, not again for every MM tile
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);

                    //more accumulator register so we can compute 2 outputs in parallel
                    float32x4_t acc4 = vdupq_n_f32(0.0f);
                    float32x4_t acc5 = vdupq_n_f32(0.0f);
                    float32x4_t acc6 = vdupq_n_f32(0.0f);
                    float32x4_t acc7 = vdupq_n_f32(0.0f);
                    float tail0 = 0.0f;
                    float tail1 = 0.0f;

                    //same input row but 2 different weight rows for the 2 outputs
                    const float* input = &input_matrix[N * input_features];
                    const float* weight0 = &weight_matrix[K * input_features];
                    const float* weight1 = &weight_matrix[(K + 1) * input_features];

                    //all input feature tiles contribute to the same output pair
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            //load input once and reuse it with both weight rows
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight0 + M + 4));
                            acc5 = vfmaq_f32(acc5, VA, vld1q_f32(weight1 + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight0 + M + 8));
                            acc6 = vfmaq_f32(acc6, VA, vld1q_f32(weight1 + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight0 + M + 12));
                            acc7 = vfmaq_f32(acc7, VA, vld1q_f32(weight1 + M + 12));
                        }

                        // Process remaining groups of four.
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));
                        }

                        //keep scalar leftovers across MM tiles too
                        for(; M < end_M; M++){
                            float value = input[M];
                            tail0 += value * weight0[M];
                            tail1 += value * weight1[M];
                        }
                    }

                    //all MM tiles finished: combine and reduce only once
                    float sum0 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    float sum1 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc4, acc5), vaddq_f32(acc6, acc7)));
                    output1[N * output_features + K] = sum0 + tail0;
                    output1[N * output_features + K + 1] = sum1 + tail1;
                }

                //calculate the last output separately if the tile has an odd number of columns
                for(; K < end_K; K++){
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);
                    float tail = 0.0f;
                    const float* input = &input_matrix[N * input_features];
                    const float* weight = &weight_matrix[K * input_features];

                    //the leftover output also keeps its sums across all MM tiles
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight + M + 12));
                        }
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));
                        }
                        for(; M < end_M; M++){
                            tail += input[M] * weight[M];
                        }
                    }

                    float sum = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    output1[N * output_features + K] = sum + tail;
                }
            }
        }
    }

    // Add bias once, after all blocks have contributed.
    for(int64_t N = 0; N < batch_size; N++){
        for(int64_t K = 0; K < output_features; K++){
            output1[N * output_features + K] += bias[K];
            if(output1[N * output_features + K] < 0){
            output1[N * output_features + K]= 0;
        }
        }
    }
    return output1;  
}

vector<float> Fused_GEMM_Softmax(vector<float>& input_matrix,
    const vector<int64_t>& input_dim, vector<float>& weight_matrix,
    const vector<int64_t>& weight_dim, vector<float>& bias){

    int64_t batch_size = input_dim[0];
    int64_t input_features = input_dim[1];
    int64_t output_features = weight_dim[0];
    int64_t block_N = 8;
    int64_t block_K = 32;
    int64_t block_M = 32;

    vector<float> output(batch_size * output_features, 0.0f);

    //keep the outer output tiles, but finish each output pair before moving on
    for(int64_t NN = 0; NN < batch_size; NN += block_N){
        int64_t end_N = min<int64_t>(NN + block_N, batch_size);
        for(int64_t KK = 0; KK < output_features; KK += block_K){
            int64_t end_K = min<int64_t>(KK + block_K, output_features);
            for(int64_t N = NN; N < end_N; N++){
                int64_t K = KK;
                for(; K + 1 < end_K; K += 2){
                    //initialize once, not again for every MM tile
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);

                    //more accumulator register so we can compute 2 outputs in parallel
                    float32x4_t acc4 = vdupq_n_f32(0.0f);
                    float32x4_t acc5 = vdupq_n_f32(0.0f);
                    float32x4_t acc6 = vdupq_n_f32(0.0f);
                    float32x4_t acc7 = vdupq_n_f32(0.0f);
                    float tail0 = 0.0f;
                    float tail1 = 0.0f;

                    //same input row but 2 different weight rows for the 2 outputs
                    const float* input = &input_matrix[N * input_features];
                    const float* weight0 = &weight_matrix[K * input_features];
                    const float* weight1 = &weight_matrix[(K + 1) * input_features];

                    //all input feature tiles contribute to the same output pair
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            //load input once and reuse it with both weight rows
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight0 + M + 4));
                            acc5 = vfmaq_f32(acc5, VA, vld1q_f32(weight1 + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight0 + M + 8));
                            acc6 = vfmaq_f32(acc6, VA, vld1q_f32(weight1 + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight0 + M + 12));
                            acc7 = vfmaq_f32(acc7, VA, vld1q_f32(weight1 + M + 12));
                        }

                        // Process remaining groups of four.
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight0 + M));
                            acc4 = vfmaq_f32(acc4, VA, vld1q_f32(weight1 + M));
                        }

                        //keep scalar leftovers across MM tiles too
                        for(; M < end_M; M++){
                            float value = input[M];
                            tail0 += value * weight0[M];
                            tail1 += value * weight1[M];
                        }
                    }

                    //all MM tiles finished: combine and reduce only once
                    float sum0 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    float sum1 = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc4, acc5), vaddq_f32(acc6, acc7)));
                    output[N * output_features + K] = sum0 + tail0;
                    output[N * output_features + K + 1] = sum1 + tail1;
                }

                //calculate the last output separately if the tile has an odd number of columns
                for(; K < end_K; K++){
                    float32x4_t acc0 = vdupq_n_f32(0.0f);
                    float32x4_t acc1 = vdupq_n_f32(0.0f);
                    float32x4_t acc2 = vdupq_n_f32(0.0f);
                    float32x4_t acc3 = vdupq_n_f32(0.0f);
                    float tail = 0.0f;
                    const float* input = &input_matrix[N * input_features];
                    const float* weight = &weight_matrix[K * input_features];

                    //the leftover output also keeps its sums across all MM tiles
                    for(int64_t MM = 0; MM < input_features; MM += block_M){
                        int64_t end_M = min<int64_t>(MM + block_M, input_features);
                        int64_t M = MM;
                        for(; M + 16 <= end_M; M += 16){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));

                            VA = vld1q_f32(input + M + 4);
                            acc1 = vfmaq_f32(acc1, VA, vld1q_f32(weight + M + 4));

                            VA = vld1q_f32(input + M + 8);
                            acc2 = vfmaq_f32(acc2, VA, vld1q_f32(weight + M + 8));

                            VA = vld1q_f32(input + M + 12);
                            acc3 = vfmaq_f32(acc3, VA, vld1q_f32(weight + M + 12));
                        }
                        for(; M + 4 <= end_M; M += 4){
                            float32x4_t VA = vld1q_f32(input + M);
                            acc0 = vfmaq_f32(acc0, VA, vld1q_f32(weight + M));
                        }
                        for(; M < end_M; M++){
                            tail += input[M] * weight[M];
                        }
                    }

                    float sum = vaddvq_f32(
                        vaddq_f32(vaddq_f32(acc0, acc1), vaddq_f32(acc2, acc3)));
                    output[N * output_features + K] = sum + tail;
                }
            }
        }

        // All output columns for these rows are now complete.
        for(int64_t N = NN; N < end_N; N++){
            int64_t row = N * output_features;
            float max_value = -numeric_limits<float>::infinity();
            for(int64_t K = 0; K < output_features; K++){
                output[row + K] += bias[K];
                max_value = max(max_value, output[row + K]);
            }

            float sum_exp = 0.0f;
            for(int64_t K = 0; K < output_features; K++){
                output[row + K] = exp(output[row + K] - max_value);
                sum_exp += output[row + K];
            }
            for(int64_t K = 0; K < output_features; K++){
                output[row + K] /= sum_exp;
            }
        }
    }
    return output;
}
