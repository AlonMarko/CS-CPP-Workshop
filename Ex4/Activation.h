//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * @brief class responsible for recognizing which function needed to apply and applies it
 */
class Activation // activation class with all the methods - see description of each method
{
private: // the type of activation
    ActivationType _type;
public:
    /**
     * @brief activation type constructor
     * @param actType enum representing the activation type
     */
    Activation(ActivationType actType);


    /**
     * @brief returns the act type to whoever asks for it - const ver
     * @return act type
     */
    ActivationType getActivationType() const;

    /**
     * @brief applies activation func - relu or softMax on the input matrix we recieved
     * @param m input matrix
     * @return matrix after activation - does not change the original matrix
     */
    Matrix operator()(const Matrix &m) const;

};

// Insert Activation class here...

#endif //ACTIVATION_H
