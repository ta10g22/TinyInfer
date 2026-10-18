#pragma once

#include <cstdint>
#include <vector>
using namespace std;

vector<float> GEMM(
    vector<float> &input_matrix, const vector<int64_t> &input_dim, vector<float> &weight_matrix, const vector<int64_t> &weight_dim, vector<float> &bias);

void ReLU(
    vector<float> &input_matrix2);

void Softmax(
    vector<float> &input, const vector<int64_t> &dim);

vector<float> Fused_GEMM_ReLU(vector<float>& input_matrix,const vector<int64_t> &input_dim,
    vector<float>& weight_matrix,const vector<int64_t>& weight_dim, vector<float>& bias);

vector<float> Fused_GEMM_Softmax(vector<float>& input_matrix,
    const vector<int64_t>& input_dim, vector<float>& weight_matrix,
    const vector<int64_t>& weight_dim, vector<float>& bias);