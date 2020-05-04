/*
  ==============================================================================

    PascalMatrix.h
    Created: 27 Apr 2020 11:03:44am
    Author:  Poonna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename T>
class PascalMatrix : dsp::Matrix<T>
{
public:
    static dsp::Matrix<T> create(int dimension)
    {
        dsp::Matrix<T> pascal(dimension, dimension);
        std::unique_ptr<T[]> col0A{ new T[dimension]{} };
        std::unique_ptr<T[]> col0B{ new T[dimension]{} };

        // Build up the first column
        col0A[0] = 1.0f;
        col0B[0] = 1.0f;
        for (auto i = 0; i < dimension; ++i)
        {
             for (auto j = 1; j < i + 1; ++j)
            {
                col0B[j] = col0A[j - 1] + col0A[j];
            }
            col0A.swap(col0B);
        }

        // Initialize the first row and column on the matrix
        for (auto i = 0; i < dimension; ++i)
        {
            pascal(0, i) = 1.0f;
            pascal(i, 0) = col0A[i];
        }

        // Build the remaining elements based on the first row and column
        for (auto j = 1; j < dimension; ++j)
        {
            for (auto i = 1; i < dimension; ++i)
            {
                pascal(i, j) = pascal(i, j - 1) - pascal(i - 1, j - 1) - pascal(i - 1, j);
            }
        }

        return pascal;
    }
};
