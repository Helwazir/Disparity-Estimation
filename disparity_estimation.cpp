//
// Created by Hamzah Elwazir on 4/4/21.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
using namespace std;

/*
 * Requires: The filename of the csv file containing the image intensity values
 * Modifies: Nothing
 * Effects: Opens the specified csv file of pixel intensities and reads the values into a two dimensional vector and
 *          returns it.
 */
vector<vector<double>> parseCSV(string filename);

/*
 * Requires: A two dimensional vector of pixel intensities present in the left image, a two dimensional vector of pixel
 *           intensities present in the right image, and the window size to be used for SSD based block matching
 * Modifies: disp_output.csv
 * Effects: Uses SSD (sum of squared differences) based block matching with a user specified window size to estimate
 *          disparity between a stereo pair of rectified images. Uses maximum disparity threshold and directional
 *          search to minimize calculations. Minimum SSD values for each pixel coordinate are written to disp_output.csv.
 */
void estimateDisparity(vector<vector<double>> left_img, vector<vector<double>> right_img, int window_size);

int main(int argc, char* argv[]) {
    int window_size;

    // Get command line input it it exists, if not then default window_size to 3
    if (argc > 1) {
        window_size = stoi(argv[1]);
    } else {
        window_size = 3;
    }

    // Read pixel intensities from csv files into vectors
    vector<vector<double>> view1_vec = parseCSV("/Users/haz/Desktop/CS 120/Module 3/Open Ended Projects/M3OEP-MPL-helwazir/view_left");
    vector<vector<double>> view5_vec = parseCSV("/Users/haz/Desktop/CS 120/Module 3/Open Ended Projects/M3OEP-MPL-helwazir/view_right");

    // Estimate disparity between images
    estimateDisparity(view1_vec, view5_vec, window_size);

    return 0;
}

vector<vector<double>> parseCSV(string filename) {
    string line, val;
    vector<vector<double>> img_vec;

    ifstream inFile(filename);
    if(!inFile.is_open()) throw std::runtime_error("Could not open file");

    while (inFile.peek() != EOF) {
        vector<double> row;
        getline(inFile, line, '\n');
        stringstream s(line);
        while (getline(s, val, ',')) {
            row.push_back(stof(val));
        }
        img_vec.push_back(row);
    }
    inFile.close();

    return img_vec;
}


void estimateDisparity(vector<vector<double>> left_img, vector<vector<double>> right_img, int window_size) {
    ofstream outFile;
    outFile.open("disp_output.csv");

    vector<vector<double>> disp_map;
    int i, j, k, x, y;
    int ssd, idx;
    int min_col, max_col;
    int MAX_DISPARITY = 75;  // Max disparity previously calculated by finding the maximum disparity in the ground truth of left_img
    int rows = left_img.size();
    int cols = left_img[0].size();
    int upper = (window_size / 2);
    int lower = upper + 1;

    for (i = lower; i < rows - upper; ++i) {  // For each row in the image
        for (j = lower; j < cols - upper; ++j) {  // For each column in the image
            // Directional search and max disparity thresholding
            if (j > MAX_DISPARITY + upper) {
                min_col = j - MAX_DISPARITY - 1;
            } else {
                min_col = lower;
            }
            max_col = j - upper;

            int min_ssd = 1000000000;
            int index = 0;

            for (k = min_col; k < max_col; ++k) {  // For each pixel left of j
                ssd = 0;
                // Calculate the SSD between two windows (sub images) in left_img and right_img
                for (x = -(window_size/2); x <= window_size/2; ++x) {
                    for (y = -(window_size/2); y <= window_size/2; ++y) {
                        idx = left_img[i + x][j + y] - right_img[i + x][k + y];
                        ssd += idx * idx;
                    }
                }

                // If the current ssd is less than min_ssd update min_ssd and set index = k
                if (ssd < min_ssd) {
                    min_ssd = ssd;
                    index = k;
                }
            }

            // Calculate the disparity between the same object in the two images and write it to disp_output.csv
            int disparity = abs(j - index);
            outFile << disparity << ',';
        }
        outFile << '\n';
        cout << "Row " << i << " done" << endl;
    }
    outFile.close();
}
