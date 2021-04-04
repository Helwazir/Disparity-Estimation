//
// Created by Hamzah Elwazir on 4/4/21.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

vector<vector<double>> parseCSV(string filename);
void estimateDisparity(vector<vector<double>> left_img, vector<vector<double>> right_img);

int main() {
    vector<vector<double>> view1_vec = parseCSV("view1_csv");
    vector<vector<double>> view5_vec = parseCSV("view5_csv");

    cout << "Size: " << view1_vec.size() << " x " << view1_vec[0].size() << endl;
    cout << "Size: " << view5_vec.size() << " x " << view5_vec[0].size() << endl;

    estimateDisparity(view1_vec, view5_vec);

    return 0;
}

vector<vector<double>> parseCSV(string filename) {
//    ifstream myFile("/Users/haz/Desktop/CS 120/Misc/MLP_maybe/" + filename);
    ifstream myFile("./" + filename);
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    string line, val;
    vector<vector<double>> img_vec;

    while (myFile.peek() != EOF) {
        vector<double> row;

        getline(myFile, line, '\n');
        stringstream s(line);

        while (getline(s, val, ',')) {
            row.push_back(stof(val));
        }
        img_vec.push_back(row);
    }

    myFile.close();
    return img_vec;
}


void estimateDisparity(vector<vector<double>> left_img, vector<vector<double>> right_img) {
    ofstream myFile;
    myFile.open("disp_output.csv");

    vector<vector<double>> disp_map;
    long i, j, k;
    long idx00, idx01, idx02, idx10, idx11, idx12, idx20, idx21, idx22;
    long sqr_idx00, sqr_idx01, sqr_idx02, sqr_idx10, sqr_idx11, sqr_idx12, sqr_idx20, sqr_idx21, sqr_idx22;
    long ssd;
    long rows = left_img.size();
    long cols = left_img[0].size();
    int MAX_DISPARITY = 75;
    int min_col, max_col;


    for (i = 2; i < rows - 1; ++i) {
        vector<double> disp_row;
        for (j = 2; j < cols - 1; ++j) {

            if (j > MAX_DISPARITY + 1) {
                min_col = j - MAX_DISPARITY - 1;
            } else {
                min_col = 2;
            }
            max_col = j - 1;

            long min_ssd = 1000000000;
            long index = 0;

            for (k = min_col; k < max_col; ++k) {

                idx00 = left_img[i - 1][j - 1] - right_img[i - 1][k - 1];
                idx01 = left_img[i - 1][j] - right_img[i - 1][k];
                idx02 = left_img[i - 1][j + 1] - right_img[i - 1][k + 1];

                idx10 = left_img[i][j - 1] - right_img[i][k - 1];
                idx11 = left_img[i][j] - right_img[i][k];
                idx12 = left_img[i][j + 1] - right_img[i][k + 1];

                idx20 = left_img[i + 1][j - 1] - right_img[i + 1][k - 1];
                idx21 = left_img[i + 1][j] - right_img[i - 1][k];
                idx22 = left_img[i + 1][j + 1] - right_img[i + 1][k + 1];

                sqr_idx00 = idx00 * idx00;
                sqr_idx01 = idx01 * idx01;
                sqr_idx02 = idx02 * idx02;

                sqr_idx10 = idx10 * idx10;
                sqr_idx11 = idx11 * idx11;
                sqr_idx12 = idx12 * idx12;

                sqr_idx20 = idx20 * idx20;
                sqr_idx21 = idx21 * idx21;
                sqr_idx22 = idx22 * idx22;

                ssd = sqr_idx00 + sqr_idx01 + sqr_idx02
                      + sqr_idx10 + sqr_idx11 + sqr_idx12
                      + sqr_idx20 + sqr_idx21 + sqr_idx22;

                if (ssd < min_ssd) {
                    min_ssd = ssd;
                    index = k;
                }
            }
            long diff = abs(j - index);
            myFile << diff << ',';
        }
        myFile << '\n';
        cout << "Row " << i << "done" << endl;
    }
}
