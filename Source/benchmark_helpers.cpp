#include <fstream>
#include <iostream>
#include <cmath>

#include "benchmark_helpers.h"

using namespace std;

bool load_csv(const string& path, vector<float>& values){
    ifstream file(path);
    if(!file.is_open()){
        cerr << "CSV file failed to open: " << path << '\n';
        return false;
    }

    //read each comma separated value from the exported CSV row
    values.clear();
    string value;
    while(getline(file, value, ',')){
        values.push_back(stof(value));
    }
    return true;
}

bool check_output(const float* actual, size_t output_size, const vector<float>& expected_output){
    if(output_size != expected_output.size()){
        cerr << "Output size does not match the expected output\n";
        return false;
    }

    for(size_t i = 0; i < output_size; i++){
        if(!isfinite(actual[i]) || !isfinite(expected_output[i]) ||
           abs(actual[i] - expected_output[i]) > 0.001f){
            cerr << "Final output does not match the expected output\n";
            return false;
        }
    }
    cout << "Final Output and model output match successfully\n";
    return true;
}
