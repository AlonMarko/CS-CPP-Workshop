//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

// Insert MlpNetwork class here...
/**
 * @brief mlp class - combines everything and is responsible for taking the input throught the
 * whole proccess
 */
class MlpNetwork
{
private:
    Matrix *_weights;
    Matrix *_biases;
public:
    /**
     * @brief constructor
     * @param weights array of 4 matrices
     * @param baises array of 4 matrices
     */
    MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);

    /**
     * @brief operator () - does the whole job of the network on
     * the given matrix
     * @param m input matrix
     * @return digit Struct - has result and probability
     */
    Digit operator()(const Matrix &m);
};

#endif // MLPNETWORK_H
