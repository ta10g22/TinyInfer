#pragma once

#include <string>
#include <vector>
#include <cstddef>

using namespace std;

bool load_csv(const string& path, vector<float>& values);

bool check_output(const float* actual, size_t output_size, const vector<float>& expected_output);
