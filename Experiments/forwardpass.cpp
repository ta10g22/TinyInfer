#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// function for linear layer
vector<vector<float>> Linear_layer(vector<vector<float>> input, const vector<vector<float>> &weight_matrix)
{
    vector<vector<float>> output(3, vector<float>(3, 0));

    // time to multiply input with weight_matrix and return output
    for (int i = 0; i < size(input); i++)
    {
        for (int j = 0; j < size(input[0]); j++)
        {
            for (int m = 0; m < size(weight_matrix[0]); m++)
            {
                output[i][m] += input[i][j] * weight_matrix[j][m];
            }
        }
    }
    return output;
}

// function for relu layer
vector<vector<float>> &Relu(vector<vector<float>> &output1)
{
    // relu only let's output numbes over 0 stay the same if less (<) 0 they get clipped to zero
    for (int i = 0; i < size(output1); i++)
    {
        for (int m = 0; m < size(output1[0]); m++)
        {
            if (output1[i][m] < 0)
            {
                output1[i][m] = 0;
            }
        }
    }
    return output1;
}

// function for Softmax layer
// softmax
vector<vector<float>> Softmax(vector<vector<float>> softmax_input_mat)
{
    for (size_t i = 0; i < size(softmax_input_mat); i++)
    {
        /* code */
    }
}

// function for Argmax layer

// main function
int main()
{
    vector<vector<float>> output1;
    vector<vector<float>> input_matrix;
    vector<vector<float>> output2;
    vector<vector<float>> weights;

    // load weights into a vector of vectors
    ifstream weights_file("model_weights.csv");

    if (!weights_file.is_open())
    {
        cout << "could not open the weights_file";
        return 1;
    }

    // create a line to for each row of the weight object
    string line;
    vector<float> w_vector;
    // loop through each row to get line
    while (getline(weights_file, line))
    {
        w_vector = {};

        // loop through each char of line and extract floats into vector
        string integer;
        for (int j = 0; j < size(line); j++)
        {
            if (line[j] == ',')
            {
                w_vector.push_back(stof(integer));
                integer = "";
                continue;
            }
            integer += line[j];
        }
        weights.push_back(w_vector);
    }

    cout << "\n";
    // create input matrix
    input_matrix = {{10.1, -0.9, 5.1, 4.2, 8.2},
                    {9.2, 3.1, -10.1, 2.0, 1.1},
                    {11.3, -9.2, -3.1, -7, -1.1}};

    // Printing Input matrix
    cout << "Input matrix : \n";
    for (int i = 0; i < size(input_matrix); i++)
    {
        for (int m = 0; m < size(input_matrix[0]); m++)
        {
            cout << input_matrix[i][m] << ", ";
        }
        cout << "\n";
    }
    cout << "\n";

    // call function for linear layer on input
    output1 = Linear_layer(input_matrix, weights);

    // print matrix after linear layer applied
    cout << "Linear layer applied: \n";
    for (int i = 0; i < size(output1); i++)
    {
        for (int m = 0; m < size(output1[0]); m++)
        {
            cout << output1[i][m] << ", ";
        }
        cout << "\n";
    }
    cout << "\n";

    // call function for relu layer on output 1
    output2 = Relu(output1);

    // print final output
    cout << "Output Matrix: \n";
    for (int i = 0; i < size(output2); i++)
    {
        for (int m = 0; m < size(output2[0]); m++)
        {
            cout << output2[i][m] << ", ";
        }
        cout << "\n";
    }

    return 0;
}
