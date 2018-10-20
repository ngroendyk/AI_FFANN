/*****************************************************************************
 * File: Neuron.cpp
 * Written By: Nate Groendyk
 * Date: 2 June 2018
 * 
 * Purpose: Implement a Neural Network Neuron
 ****************************************************************************/
 #include "Neuron.h"
 #include <stdint.h>
 #include <iostream>

//globals
NeuronId Neuron::mzNumOfNeurons = 0; //default startup #


Neuron::Neuron(enum ActivationFunc PhoType) : mBias(),
                                              mWeightedInput(),
                                              mDentrites(),
                                              mAxon(),
                                              mError(),
                                              mPhoType(PhoType)
{
	mzNumOfNeurons++;
	mNeuronId = mzNumOfNeurons;
}

Neuron::~Neuron()
{

}

void Neuron::CalcActivation()
{

	//Go through current Dentrites

	//For each of their inputs Mult by their weights

	//After An action does the pointial decrease? if so, clear
	//input after calculation of Activation.thoughts to be more temporal...but
    //we are implementing standard FF ANN.

	double Sum = 0.0;
	for (VecDentrite::iterator Iter = mDentrites.begin();
		  Iter != mDentrites.end(); ++Iter)
	{
		Sum += Iter->mWeighting * Iter->mInputValue;
	}

    mWeightedInput = Sum + mBias; //this is Z.
    
	//Now apply the Activation Function, 
    switch (mPhoType)
    {
        case eSigmoid: //Apply sigmoid func: pho(z) = (1 / (1 + e^(-z)))
            mAxon = 1.0 / (1.0 + exp(-1.0 * mWeightedInput));
            break;
        default:
            std::cout << "Neuron:: Activation Func Type unknown: " << mPhoType
                      << std::endl;
            assert(false);
            break;
    } //end switch, apply specific activation func
}


bool Neuron::SetError(double BackPropWeightedSumError)
{

    //use the selected activation function's derivative.
    //use the currently calculated Z (forward weighted input) to calc PhoDeriv
    // Multiply BackPropWeightedSumError by pho(z) --> Store as mError.

    switch (mPhoType)
    {
      case eSigmoid:
        //use the sigmoid derivative for calcing this error
        // dPho(z) = (e^(-z)) / ((1 + e^(-z))^2)
        mError = BackPropWeightedSumError * 
            (               (exp(-1.0 * mWeightedInput)) /
            ((1.0+exp(-1.0*mWeightedInput))*(1.0+exp(-1.0*mWeightedInput))) );
        break;    
      default:
        std::cout << "Neuron:: Activation Func Type unknown: " << mPhoType 
                  << std::endl;
        assert(false);
        return(false);
        break;
    }
    return true;
}

double Neuron::GetError()
{
    return mError;
}

bool Neuron::UpdateBias(double NumOfTrainingSamples, double LearningRate)
{
    //Notes about this calc:
    // 1) We assume the LearningRate is between 0 and 1.
    // 2) We dont mult by 0.5 (1/2) b/c the cost function may not be least
    // squares, which may scale our learning rate accordingly. (Eventually,
    // we can detect the Cost func perhaps, and then do the scaling, but need
    // access to Layer class, so bail for now, since can be dealt with by
    // passedin Learning Rate).
    // we use a negative delta (subtract) of the computed derivative so that
    // the result, will cause Newtonian Root Minimization (always moving to 0).

    // Perhaps later we can figure out bettwe automatic delta's rather than
    // using learning rate and # of samples

    mBias = mBias - (mError * (LearningRate / NumOfTrainingSamples));
    return true;
}

bool Neuron::Dentrite::UpdateWeight(double l_minus1KthNeuron_AxonOutput,
                                    double l_jthNeuron_Error,
                                    double NumOfTrainingSamples,
                                    double LearningRate)
{
    //The A^(l-1)k * Error^(l)j = the dCost/Dweight (computed, so this is a
    //slope), so we just multiply by the delta we want (1/#ofTrainingSamples) *
    //        by the learningrate (the delta), we need to move to move towards
    //        the Cost minimum, since the detla ends up being the negative of
    //        the computed derivative.
    mWeighting = mWeighting - ((l_minus1KthNeuron_AxonOutput *
                    l_jthNeuron_Error) * (LearningRate / NumOfTrainingSamples));
    return true;
}

AxonOutput Neuron::GetAxonResult()
{
	return mAxon;
}

void Neuron::Synapse_UpdateDentriteInput(uint64_t Dentrite, 
													  double NewValue)
{
	//Does the element exist? assert if not.

	//update the pair value.
}

void Neuron::Plastisity_UpdateDentriteWeight(
		uint64_t Dentrite,
		double NewCoef)
{

	//Does the element exist? assert if not.

	//update the weighting.
}

Neuron::VecDentrite& Neuron::GetDentritesRef()
{
	return mDentrites;
}


NeuronId Neuron::GetId()
{
	return mNeuronId;
}


Neuron::Dentrite::Dentrite():
		mWeighting(),
		mInputValue()
{
}

Neuron::Dentrite::~Dentrite()
{
}


Neuron::Dentrite& Neuron::AddDentrite()
{
	Dentrite NewDentrite; //Gets Copied in.
	mDentrites.push_back(NewDentrite); //copy in the new one.
	return mDentrites.back(); //return a reference to the copy.
}

double Neuron::ReadAxon()
{
	return mAxon;
}

