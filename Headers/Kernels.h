#pragma once

#include <cstdint>
#include <vector>
using namespace std;

void GEMM(
    vector<float> &input_matrix, const vector<int64_t> &input_dim, vector<float> &weight_matrix, const vector<int64_t> &weight_dim, vector<float> &bias,
    vector<float>& output1);

void ReLU(
    vector<float> &input_matrix2);

void Softmax(
    vector<float> &input, const vector<int64_t> &dim);

void Fused_GEMM_ReLU(vector<float>& input_matrix,const vector<int64_t> &input_dim,
    vector<float>& weight_matrix,const vector<int64_t>& weight_dim, vector<float>& bias,
    vector<float>& output1);

void Fused_GEMM_Softmax(vector<float>& input_matrix,
    const vector<int64_t>& input_dim, vector<float>& weight_matrix,
    const vector<int64_t>& weight_dim, vector<float>& bias,
    vector<float>& output);
