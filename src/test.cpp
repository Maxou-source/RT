#include <iostream>
#include <stdexcept>

class DoubleMatrix {
private:
    double* data;   // Pointer to store the matrix as a 1D array
    size_t rows;    // Number of rows
    size_t cols;    // Number of columns

public:
    // Constructor
    DoubleMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        data = new double[rows * cols]; // Allocate memory for 2D matrix in a 1D array
        for (size_t i = 0; i < rows * cols; ++i) {
            data[i] = 0.0; // Initialize all elements to 0
        }
    }

    // Destructor
    ~DoubleMatrix() {
        delete[] data; // Free allocated memory
    }

    // Access to a row, which enables m[row][col] access
    class RowAccessor {
    private:
        double* rowData;
        size_t colCount;

    public:
        RowAccessor(double* rowData, size_t colCount) : rowData(rowData), colCount(colCount) {}

        double& operator[](size_t col) {
            if (col >= colCount) {
                throw std::out_of_range("Column index out of range");
            }
            return rowData[col];
        }
    };

    // Overload `operator[]` for row access
    RowAccessor operator[](size_t row) {
        if (row >= rows) {
            throw std::out_of_range("Row index out of range");
        }
        return RowAccessor(data + row * cols, cols);
    }

    // Const version for read-only access
    class ConstRowAccessor {
    private:
        const double* rowData;
        size_t colCount;

    public:
        ConstRowAccessor(const double* rowData, size_t colCount) : rowData(rowData), colCount(colCount) {}

        const double& operator[](size_t col) const {
            if (col >= colCount) {
                throw std::out_of_range("Column index out of range");
            }
            return rowData[col];
        }
    };

    ConstRowAccessor operator[](size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of range");
        }
        return ConstRowAccessor(data + row * cols, cols);
    }

    // Getters for dimensions
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
};
