#include <vector>
#include <cmath>
using namespace std;


// kernel for General Matrix Multiplication
vector<vector<float>> GEMM(vector<vector<float>> input_matrix, vector<vector<float>> weight_matrix, vector<float> bias){

    int batch_size = input_matrix.size();
    int input_features = input_matrix[0].size();
    int output_features = weight_matrix.size();

    vector<vector<float>> output1(batch_size, vector<float>(output_features, 0));

    for(int N = 0; N < batch_size; N++){
        for(int K = 0; K < output_features; K++){
            for(int M = 0; M < input_features; M++){
                output1[N][K] += input_matrix[N][M] * weight_matrix[K][M];
            }

            output1[N][K] += bias[K];
        }
    }

    return output1;
}

// Kernel for Relu activation layer
vector<vector<float>> ReLU(vector<vector<float>>& input_matrix2){

    for(int i = 0; i < input_matrix2.size(); i++){
        for(int j = 0; j < input_matrix2[0].size(); j++){
            if(input_matrix2[i][j] < 0){
                input_matrix2[i][j] = 0;
            }
        }
    }

    return input_matrix2;
}

// Kernel for Softmax layer
vector<vector<float>> Softmax(vector<vector<float>>& input_matrix3){

    for(int i = 0; i < input_matrix3.size(); i++){

        float max_value = input_matrix3[i][0];

        for(int j = 0; j < input_matrix3[0].size(); j++){
            if(input_matrix3[i][j] > max_value){
                max_value = input_matrix3[i][j];
            }
        }
        
        //while we subtract the max_value from all elements to make them <=0 , We can also add the exp to get 
        //the sum at the same time

        float sum_exp = 0;
        for(int j = 0; j < input_matrix3[0].size(); j++){
            input_matrix3[i][j] = exp(input_matrix3[i][j] - max_value);
            sum_exp += input_matrix3[i][j];
        }
        
        for(int j = 0; j < input_matrix3[0].size(); j++){
            input_matrix3[i][j] = input_matrix3[i][j] / sum_exp;
        }
    }

    return input_matrix3;
}