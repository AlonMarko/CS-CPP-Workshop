//
// Created by alonn on 6/9/2020.
//
#define LAST_RELU 2
#include "MlpNetwork.h"

/**
     * @brief constructor
     * @param weights array of 4 matrices
     * @param baises array of 4 matrices
     */
MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases) : _weights(weights), _biases(biases)
{
}

/**
    * @brief operator () - does the whole job of the network on
    * the given matrix
    * @param m input matrix
    * @return digit Struct - has result and probability
    */
Digit MlpNetwork::operator()(const Matrix &m)
{
    Digit finalRes;
    finalRes.probability = 0;
    finalRes.value = 0;
    float cur = 0;
    Matrix walkThroughMat = m;
    for (int i = 0; i < MLP_SIZE; i++)
    {
        if (i <= LAST_RELU)
        {
            Dense layer(_weights[i], _biases[i], Relu);
            walkThroughMat = layer(walkThroughMat);
        }
        else
        {
            Dense layer(_weights[i], _biases[i], Softmax);
            walkThroughMat = layer(walkThroughMat);
        }
    }
    for (int j = 0; j < walkThroughMat.getRows()*walkThroughMat.getCols(); j++)
    {
        cur = walkThroughMat[j];
        if (cur > finalRes.probability)
        {
            finalRes.probability = cur;
            finalRes.value = j;
        }
    }
    return finalRes;
}
