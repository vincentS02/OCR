//XOR.C
// --------------------------------------------------------------------------
#include "xor.h"
#include "matrix.h"


//Creation of the structure neural_network 

struct neural_network NetworkInit()

{
	struct neural_network network;

	network.nbInputs = 784;
	network.nbHidden = 200;
	network.nbOutputs = 10;

    // Init of input matrix
	network.matInput = createMatrix(784, 1);

    // Init of the target matrix
	network.matTarget = createMatrix(10, 1);

	

    // Init error matrix
	network.matErrors = createMatrix(10, 1);

	//This mat can be initialised at 0 because
	//the numbers will be overwritten in FeedForward
	initMatrix_zero(network.matErrors);
	
    /*Here, we create the matrices of Weigths of the Input-Hidden layer
      and Hidden_Output layer. NB: for example (1;0) is the weight
      between the second hidden node and the first input node */

	network.matWeightsIH = createMatrix(network.nbHidden, 
			network.nbInputs);
	network.matWeightsHO = createMatrix(network.nbOutputs, 
			network.nbHidden);

	network.matOutputHidden = createMatrix(network.nbHidden, 1);
	network.matBiasHidden = createMatrix(network.nbHidden, 1);
	network.matWeightByInputIH = createMatrix(network.nbHidden,1);

	// Init of the last matrices and values
    	network.Mat_WO_HOutputs = createMatrix(network.nbOutputs, 1);

 	network.FinalOutput = createMatrix(network.nbOutputs, 1);
	network.BiasOutputNode = createMatrix(network.nbOutputs, 1);

	initMatrix(network.BiasOutputNode);
	initMatrix(network.matWeightsIH);

	initMatrix(network.matWeightsHO);
	initMatrix(network.matBiasHidden);
	initMatrix(network.Mat_WO_HOutputs);

	network.learning_rate = 0.1;
	return network;
}

void reset(struct neural_network *network)
{	
	initMatrix_zero(network->Mat_WO_HOutputs);
	initMatrix_zero(network->FinalOutput);
	initMatrix_zero(network->matWeightByInputIH);
	initMatrix_zero(network->matOutputHidden);

}

//Sigmoid function

float sigmoid(float x) 
{
    return (1.0 / (1.0 +   exp(-x)));
}

float softmax(int i,float sum,struct neural_network network)
{
	return exp(moveInMatrix(network.Mat_WO_HOutputs,i,0))/sum;
}


//Feedforward function
void Feedforward(struct neural_network network)
{
	/* Multiplication of the Weights by the input in the first layer :
	   the weights between the input and the hidden nodes */
	for (int hidden = 0; hidden < network.nbHidden; hidden++)
	{
		float TMPWI = 0;
		for (int input = 0; input < network.nbInputs; input++)
		{
		    //here I sum the Weights*Input
		    TMPWI += moveInMatrix(network.matWeightsIH,hidden, input) 
			    * moveInMatrix(network.matInput,input, 0);
		}
		//here I add the Biases of the Input-Hidden layer
		TMPWI += moveInMatrix(network.matBiasHidden, hidden, 0);

		// Here I store Weight*Input + Biases in the matrix 
		// 					matWeightByInputIH
		editMatrix(network.matWeightByInputIH, hidden, 0, TMPWI);

		//Finally, I use the sigmoid function on each value of the 
		//						previous matrix
		editMatrix(network.matOutputHidden, hidden, 0, sigmoid(TMPWI));
	}
	// For the hidden-output layer
	float sum=0;
	for(int output=0; output<network.nbOutputs; output++)
	{
		float TMPHO = 0;
		/*Multiplication of the weights by the output of the hidden node*/
        	for (int i = 0; i < network.nbHidden; i++)
        	{
		//here I sum the Weight*Input (input= hidden output)
        		TMPHO += moveInMatrix(network.matWeightsHO, output, i) * 
                	 	moveInMatrix(network.matOutputHidden, i, 0);
        	}
		//here I add the biases of the Hidden-Output layer
        	TMPHO += moveInMatrix(network.BiasOutputNode, output, 0);

		//Here I store the Weight*Input = Biases in the matrix mat_WO_HOutput
        	editMatrix(network.Mat_WO_HOutputs, output, 0, TMPHO);

		sum+=exp(TMPHO);

		//I apply the sigmoid function on each value of the previous matrix
        	editMatrix(network.FinalOutput, output, 0, sigmoid(TMPHO));	

	}
	
	//for(int i=0;i<network.nbOutputs;i++)
	//{
	//	editMatrix(network.FinalOutput,i , 0, softmax(i,sum,network));
	//}

	
}


//propagation the error
float* error_hidden(struct neural_network *network)
{
	for(int i=0;i<network->nbOutputs;i++)
	{
		editMatrix(network->matErrors,i,0,
				moveInMatrix(network->FinalOutput,i,0)
				-moveInMatrix(network->matTarget, i, 0));
	}

    float* hidden = calloc(network->nbHidden,sizeof(float));
    for(int i=0;i<network->nbHidden;i++)
    {
        float new_error=0;
        for(int j=0;j<network->nbOutputs;j++)
        {
            new_error+=moveInMatrix(network->matWeightsHO,j,i) * 
		    moveInMatrix(network->matErrors,j,0);
		    
        }
        hidden[i]=new_error;
    }
    


    return hidden;
}

//update the weights of the neural network
void update_weights(struct neural_network *network,float *error,
		float learning_rate)
{
	float delta;
    for(int i=0;i<network->nbOutputs;i++)
    {
        for (int j = 0; j < network->nbHidden; j++)
        {
		
		delta=moveInMatrix(network->matWeightsHO,i,j)-learning_rate*
			moveInMatrix(network->matErrors,i,0)*
			moveInMatrix(network->matOutputHidden,j,0)*
			moveInMatrix(network->FinalOutput,i,0)*
			(1-moveInMatrix(network->FinalOutput,i,0));
            	editMatrix(network->matWeightsHO,i,j,delta);
        }
    }
    for(int i=0;i<network->nbHidden;i++)
    {
        for (int j = 0; j < network->nbInputs;j++)
        {
		delta=moveInMatrix(network->matWeightsIH,i,j)-learning_rate*error[i]*
				moveInMatrix(network->matInput,j,0)*
				moveInMatrix(network->matOutputHidden,i,0)*
				(1-moveInMatrix(network->matOutputHidden,i,0));
            	editMatrix(network->matWeightsIH,i,j,delta);
        }
    }

}

//Update the bias of the neural network
void update_bias(struct neural_network *network,float *error,
				float learning_rate)
{
	for(int i=0;i<network->nbOutputs;i++)
	{
		editMatrix(network->BiasOutputNode,i,0,
					moveInMatrix(network->BiasOutputNode,0,0)-learning_rate*
					moveInMatrix(network->matErrors,i,0)*
					moveInMatrix(network->FinalOutput,i,0)*
					(1-moveInMatrix(network->FinalOutput,i,0)));

	}
		
	float delta;
	for( int i=0;i<network->nbHidden;i++)
	{
		delta=moveInMatrix(network->matBiasHidden,i,0)-learning_rate*error[i]*
				moveInMatrix(network->matOutputHidden,i,0)*
				(1-moveInMatrix(network->matOutputHidden,i,0));
		editMatrix(network->matBiasHidden,i,0,delta);

	}

}
//update the neural network to improve it
void backprobagating(struct neural_network *network)
{
	float* error1= error_hidden(network);
	update_weights(network,error1, network->learning_rate);
	update_bias(network, error1,network->learning_rate);
	free(error1);
}


void TrainNeuralNetwork(float *csv,struct neural_network network)
{
	int label=csv[0];

	reset(&network);

	for(int i=1;i<785;i++)
	{
		editMatrix(network.matInput,i-1,0,csv[i]);
	}

	for(int i=0;i<10;i++)
	{
		if(label!=i)
		{
			editMatrix(network.matTarget,i,0,0.01f);

		}
		else
		{
			editMatrix(network.matTarget,i,0,0.99f);
		}
	}

	Feedforward(network);

	backprobagating(&network);

	
}

char Query(float* csv,struct neural_network network)
{	

	reset(&network);

	for(int i=0;i<784;i++)
	{
		editMatrix(network.matInput,i,0,csv[i]);
	}

	Feedforward(network);

	float max=0;
	int ind=0;
	for (int i=0; i<network.nbOutputs;i++)
	{
		if(moveInMatrix(network.FinalOutput,i,0)>max)
		{
			max=moveInMatrix(network.FinalOutput,i,0);
			ind=i;
		}

	}
	return '0'+ind;

}

void save(struct neural_network network,char *file)
{
	FILE *fichier=fopen(file,"w+");
	for(int i=0;i<network.nbHidden;i++)
	{
		for(int j=0;j<network.nbInputs;j++)
		{
			fprintf(fichier,"%f ",moveInMatrix(network.matWeightsIH,i,j));
		}
		fputc('\n',fichier);

	}
	fputc('\n',fichier);

	for(int i=0;i<network.nbHidden;i++)
	{
		fprintf(fichier,"%f ",moveInMatrix(network.matBiasHidden,i,0));
	
	}
	fputc('\n',fichier);
	fputc('\n',fichier);

	for(int i=0;i<network.nbOutputs;i++)
	{
		for(int j=0;j<network.nbHidden;j++)
		{
			fprintf(fichier,"%f ",moveInMatrix(network.matWeightsHO,i,j));
		}
		fputc('\n',fichier);

	}
	fputc('\n',fichier);

	for(int i=0;i<network.nbOutputs;i++)
	{
		fprintf(fichier,"%f ",moveInMatrix(network.BiasOutputNode,i,0));
	
	}

	fclose(fichier);


}

void load(struct neural_network network, char* file)
{
	FILE *fichier=fopen(file,"r");
	for(int i=0;i<network.nbHidden;i++)
	{
		for(int j=0;j<network.nbInputs;j++)
		{
			char c=fgetc(fichier);
			int n=1;
			if (c=='-')
			{
				n=-1;
				c=fgetc(fichier);
			}
			float res=0;
			while(c!=' ')
			{
				if(c<='9' && c>='0')
				{
					res=res*10+atoi(&c);
				}
				c=fgetc(fichier);
			}
			editMatrix(network.matWeightsIH,i,j,n*res/1000000);	
		}
		fgetc(fichier);
	}
	fgetc(fichier);

	for(int i=0;i<network.nbHidden;i++)
	{
		char c=fgetc(fichier);
		int n=1;
		if (c=='-')
		{
			n=-1;
			c=fgetc(fichier);
		}
		float res=0;
		while(c!=' ')
		{
			if(c<='9' && c>='0')
			{
				res=res*10+atoi(&c);
			}
			c=fgetc(fichier);
		}
		editMatrix(network.matBiasHidden,i,0,n*res/1000000);	
	}
	fgetc(fichier);
	fgetc(fichier);

	for(int i=0;i<network.nbOutputs;i++)
	{
		for(int j=0;j<network.nbHidden;j++)
		{
			char c=fgetc(fichier);
			int n=1;
			if (c=='-')
			{
				n=-1;
				c=fgetc(fichier);
			}
			float res=0;
			while(c!=' ')
			{
				if(c<='9' && c>='0')
				{
					res=res*10+atoi(&c);
				}
				c=fgetc(fichier);
			}
			editMatrix(network.matWeightsHO,i,j,n*res/1000000);	
		}
		fgetc(fichier);
	}
	fgetc(fichier);

	for(int i=0;i<network.nbOutputs;i++)
	{
		char c=fgetc(fichier);
		int n=1;
		if (c=='-')
		{
			n=-1;
			c=fgetc(fichier);
		}
		float res=0;
		while(c!=' ')
		{
			if(c<='9' && c>='0')
			{
				res=res*10+atoi(&c);
			}
			c=fgetc(fichier);
		}
		editMatrix(network.BiasOutputNode,i,0,n*res/1000000);	
	}

}

