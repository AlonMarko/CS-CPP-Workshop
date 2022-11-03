//
// Created by alonn on 6/9/2020.
//
#include "Activation.h"

#ifndef EX4_CPP_DENSE_H
#define EX4_CPP_DENSE_H
/**
 * @brief dense class - representsa layer of the "network"
 * performs the layers functions on the input
 */
class Dense
{
private:
    Matrix _weight;
    Matrix _bias;
    ActivationType _type;
public:
    /**
     * @brief constructor
     * @param weight matrix
     * @param bias matrix
     * @param aType sofr or relu
     */
    Dense(Matrix &weight, Matrix &bias, ActivationType aType);

    /**
     * @brief returns the weight matrix - forbids mods
     * @return matrix
     */
    Matrix getWeights() const;

    /**
     * @brief returns the bias matrix - forbids mods
     * @return matrix
     */
    Matrix getBias() const;

    /**
     * @brief returns activation type - relu or softMax
     * @return relu of softMax - in struct
     */
    ActivationType getActivation() const;

    /**
     * @brief applies the layer on the input and returns the output matrix from
     * that layer
     * @param m input matrix
     * @return output matrix
     */
    Matrix operator()(const Matrix &m) const;
};

#endif //EX4_CPP_DENSE_H
