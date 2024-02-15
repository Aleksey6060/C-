#include <iostream>

int main() {
    const int rows = 3;
    const int cols = 5;
    float matrix[rows][cols];

    std::cout << "Enter the elements of the matrix:\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    float row_averages[rows];

    for (int i = 0; i < rows; ++i) {
        float sum = 0;
        for (int j = 0; j < cols; ++j) {
            sum += matrix[i][j];
        }
        row_averages[i] = sum / cols;
    }

    std::cout << "Row averages:\n";

    for (int i = 0; i < rows; ++i) {
        std::cout << row_averages[i] << '\n';
    }

    return 0;
}