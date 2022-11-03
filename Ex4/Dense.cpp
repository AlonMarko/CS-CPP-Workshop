//
// Created by alonn on 6/9/2020.
//

#include "Dense.h"

/**
     * @brief constructor
     * @param weight matrix
     * @param bias matrix
     * @param aType sofr or relu
     */
Dense::Dense(Matrix &weight, Matrix &bias, ActivationType aType) : _weight(weight), _bias(bias),
                                                                   _type(aType)
{
}

/**
    * @brief returns the weight matrix - forbids mods
    * @return matrix
    */
Matrix Dense::getWeights() const
{
    return _weight;
}

/**
    * @brief returns the bias matrix - forbids mods
    * @return matrix
    */
Matrix Dense::getBias() const
{
    return _bias;
}

/**
    * @brief returns activation type - relu or softMax
    * @return relu of softMax - in struct
    */
ActivationType Dense::getActivation() const
{
    return _type;
}

/**
  * @brief applies the layer on the input and returns the output matrix from
  * that layer
  * @param m input matrix
  * @return output matrix
  */
Matrix Dense::operator()(const Matrix &m) const
{
    Matrix copy = m;
    copy = _weight * m + _bias;
    Activation activation = _type;
    return activation(copy);
}


