//XOR.H
// ----------------------------------------------------------------------------|
#ifndef XOR_H
#define XOR_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

struct neural_network
{
	int nbInputs;
	int nbHidden;
	int nbOutputs;
	double learning_rate;
	
    // Matrices of the neural network

    	/*Input of the neural network, it will be useful for the epoch
    	because it's thanks to this input that we will be able to train our
    	neural network on different combination of input*/
        struct Matrix matInput;
    	// Final Output after the feeding function
    	struct Matrix FinalOutput;
    
    	// This matrix will allow us to calculate the error after the feeding 
    	// function thanks to the final output
	struct Matrix matTarget;
    	// Matrix of weights of the Input-Hidden layer
	struct Matrix matWeightsIH;
    	// Matrix of the biases of hidden nodes
    	struct Matrix matBiasHidden;
    	// Matrix of the sum of weights * inputs
    	struct Matrix matWeightByInputIH;
    	// Matrix of the output of the hidden layer (after the sum)
    	struct Matrix matOutputHidden;
	// Matrix of weights in Hidden-output layer
	struct Matrix matWeightsHO;
	// Error values
	struct Matrix matErrors;
	// Bias of the output node
    	struct Matrix BiasOutputNode;
	// Sum of the weights * outputs of hidden node
	struct Matrix Mat_WO_HOutputs;

};


// Other Global variables
struct neural_network NetworkInit();
void reset(struct neural_network *network);
float * error_hidden(struct neural_network *network);
void update_weights(struct neural_network *network,float *error,
		float learning_rate);
void update_bias(struct neural_network *network,
				float *error,float learning_rate);
void backprobagating(struct neural_network *network);
void Feedforward(struct neural_network network);
void TrainNeuralNetwork(float *csv,struct neural_network network);
char Query(float *csv, struct neural_network network);
void save(struct neural_network network,char* file);
void load(struct neural_network network, char* file);

#endif
