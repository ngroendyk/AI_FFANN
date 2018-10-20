/*****************************************************************************
 * File: FeedForwardNeuralNet.cpp
 * Written By: Nate Groendyk
 * Date: 26 June 2018
 *
 * Purpose: Implement a neural network using the neuron class
 *****************************************************************************/

#include "FeedForwardNeuralNet.h"
#include "StringTools.h"
#include "Neuron.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

//Static to keep track of when we srand'd everything.
bool FeedForwardNeuralNet::mzRandSet = false;



///////////////////////////////////////
/// FeedForwardNeuralNet Class Funcs
///////////////////////////////////////

FeedForwardNeuralNet::FeedForwardNeuralNet(
    uint64_t NumOfInputs, 
    uint64_t NumOfOutputs,
    enum CostFuncType ChosenCostFunction) : mFF_Layers(),
                                            mInputVec(NumOfInputs),
                                            mOutputVec(NumOfOutputs),
                                            mIsNetworkLockedIn(false),
                                            mNetCostFunc(ChosenCostFunction),
                                            mStaleCompute(true)
{
    //Initialze the Random number generator with a seed.
    if (!mzRandSet)
    {
        mzRandSet = true;
        srand(time(NULL));
    }
}

FeedForwardNeuralNet::~FeedForwardNeuralNet()
{
}

FeedForwardNeuralNet::HiddenLayers& FeedForwardNeuralNet::GetFF_Layers()
{
    return mFF_Layers;
}

bool FeedForwardNeuralNet::LoadInputs(
		const std::vector<double>& Inputs)
{
    //Simply pass the incoming values into the member vector
	uint64_t NumOfElements = mInputVec.size();
	if (Inputs.size() < NumOfElements)
	{
		NumOfElements = Inputs.size();
	}
	//iterate through inputs, and get into input.
    for (uint64_t i = 0; i< NumOfElements; ++i)
    {
        mInputVec[i] = Inputs[i];
    }
    mStaleCompute = true;
	return true;
}

bool FeedForwardNeuralNet::ReadOutputs(
		std::vector<double>& Outputs)
{
	uint64_t NumOfElements = mOutputVec.size();
	if (Outputs.size() < NumOfElements)
	{
		NumOfElements = Outputs.size();
	}

	//iterate through outputs get pointer and read the Axon
	for (uint64_t i = 0; i < NumOfElements; i++)
	{
        Outputs[i] = mOutputVec[i];
	}

    return true;
}

bool FeedForwardNeuralNet::EZCreateFFNN(std::string CreationStr)
{

    return true;
}

bool FeedForwardNeuralNet::LockNetwork()
{
    if (mIsNetworkLockedIn)
    {
        return true;
    }

    //We now assume the network has its neurons and hidden layers,
    //lets "lock-it-in" and create subsequent neuron dentrites.

    if (mFF_Layers.empty())
    {
        //need hidden layers in our network...at least 1
        return false;
    }

    //Make sure the last hidden layer has the same number of outputs as our
    //output vector
    if (!mFF_Layers.back().get())
    {
        assert(false);
    }
    if (mFF_Layers.back()->GetLayerNeurons().size() != mOutputVec.size())
    {
        //need last hidden layer to have as many neurons as the output vector.
        std::cout << "Last Hidden Layer needs same number of Neurons as output"
                     "Vector " << std::endl;
        return false;
    }

    //Iterate through the hidden layers, and add in Neuron Dentrites.
    uint64_t NumOfDents = 0;
    uint64_t PrevLayerSize = 0;

    for (HiddenLayers::iterator LayerIter = mFF_Layers.begin();
         LayerIter != mFF_Layers.end(); ++LayerIter)
    {
        if (!LayerIter->get())
        {
            assert(false);
        }

        //if first layer we add the dentrites based on i/p
        if (LayerIter == mFF_Layers.begin())
        {
            NumOfDents = mInputVec.size();
        }
        else
        { //not the first layer. we use prev layer to get the dentrite cnt
            NumOfDents = PrevLayerSize;
        }
        //Iterate through the Neurons in this layer
        for (NeuronPtrVect::iterator NeuronIt =
            (*LayerIter)->GetLayerNeurons().begin(); NeuronIt !=
            (*LayerIter)->GetLayerNeurons().end(); ++NeuronIt)
        {

            //Make sure we have a valid Neuron
            if (!(*NeuronIt).get())
            {   
                std::cout << "LockIn() Error: Invalid Neuron!" << std::endl;
                continue;
            }
            Neuron& CurNeuron(**NeuronIt);
        
            //Add in the dentrites
            for (uint64_t i = 0; i < NumOfDents; ++i)
            {
                //Add the dentrite, and get a ref to it.
                Neuron::Dentrite& CurDent = CurNeuron.AddDentrite();
                //Init the weight
                switch ((*LayerIter)->GetDentWeightType())
                {
                    case eZero: //means to default to 0
                        CurDent.mWeighting = 0.0;
                        CurNeuron.mBias = 0.0;
                        break;
                    case eRandom: //means tp default to random cnt between range.
                        if (((*LayerIter)->mWeightUpperRange == 0.0) &&
                            ((*LayerIter)->mWeightLowerRange == 0.0))
                        {
                            CurNeuron.mBias = 0.0;
                            CurDent.mWeighting = 0.0; //no range specified. use
                                                      //thus use zero
                            break;
                        }
                        {
                        //Get a random # within range
                        double OldRange = RAND_MAX;
                        double NewRange = (*LayerIter)->mWeightUpperRange -
                                          (*LayerIter)->mWeightLowerRange;
                        double HalfRange = static_cast<double>(RAND_MAX) / 2.0;
                        double RandNum = static_cast<double>(rand()) - HalfRange;
                        double NormalRandNum = (NewRange / OldRange) * RandNum;
                        CurDent.mWeighting = NormalRandNum;
                        //double RandNm2 = static_cast<double>(rand()) - HalfRange;
                        //double NormalRandNm2 = (NewRange / OldRange) * RandNm2;
                        CurNeuron.mBias = 0.0; //NormalRandNm2; //Get a random bias
                                                        //eventhough overwritten
                                                        //but do the extra ops,
                                                        //b/c it saves code.
                        }
                        break;
                    case eUserSpec: //means to default to user specified #
                            CurDent.mWeighting = (*LayerIter)->mUserWeight;
                            //CurNeuron.mBias = (*LayerIter)->mUserWeight;
                        CurNeuron.mBias = 0.0; //(*LayerIter)->mUserWeight;


                        break;
                    default:    //unknown type
                        std::cout << "Bad Dentrite initial weight type" 
                                  << std::endl;
                        assert (0);
                        break;
                } //end switch
            
            } //end for: iterate through dentrite adding.
        } //end for: NeuronIt
        PrevLayerSize = (*LayerIter)->GetLayerNeurons().size(); //backup size
    } //end for: LayerIter

    mIsNetworkLockedIn = true;
    return true;
}

bool FeedForwardNeuralNet::DoCompute()
{
	if (!mIsNetworkLockedIn)
	{
		return false;
	}

/*
/////
uint32_t l = 0;
for (uint16_t r = 0; r < 28; ++r)
{
    for (uint16_t c = 0; c < 28; ++c)
    {
        if (mInputVec[l] > 10.0)
        {
            std::cout << "1 ";
        }
        else
        {
            std::cout << "  ";
        }
        ++l;
    }
    std::cout << std::endl;
}

/////
*/

    //Iterate through all the hidden layers, load each neuron with its
    //input values and call its ComputeActivation func.
    // The ComputeActivation, should also maintain a new Weighted input.
    HiddenLayers::iterator PrevLayerIt;
    for (HiddenLayers::iterator LayerIt = mFF_Layers.begin(); 
         LayerIt != mFF_Layers.end(); ++LayerIt)
    {
        if (!(*LayerIt).get())
        { //error, all layers must contain a filled pointer.
            assert(false);
        }
        
        //For current Layer, go through its Neurons Dentrites and load the
        //Dentrites with new values.
        for (NeuronPtrVect::iterator NeuronIter = 
            (*LayerIt)->GetLayerNeurons().begin(); NeuronIter !=
            (*LayerIt)->GetLayerNeurons().end(); ++NeuronIter)
        {
            if (!(*NeuronIter).get())
            { //Must have an actual Neuron populated in shared ptr.
                assert(false);
            }

            uint64_t CurDentPos = 0;
            //Now for current Neuron iterate through its Dentrites, and load
            //the dentrite with the input vector/last layer's values.
            for (std::vector<Neuron::Dentrite>::iterator CurDent = 
                (*NeuronIter)->GetDentritesRef().begin(); CurDent !=
                (*NeuronIter)->GetDentritesRef().end(); ++CurDent)
            {
                if (LayerIt == mFF_Layers.begin())
                {
                    //first layer, we must grab from input vect
                    CurDent->mInputValue = mInputVec[CurDentPos];
                }
                else
                {  
                    //Not first layer, we grab from the previously layer Neurons
                    CurDent->mInputValue = (*PrevLayerIt)->
                        GetLayerNeurons()[CurDentPos]->ReadAxon();  
                }
                ++CurDentPos;
            } //end for: dentrite iter

            //Compute Neural Activation
            (*NeuronIter)->CalcActivation();

        } //end for iterating through layer Neurons
        PrevLayerIt = LayerIt;
    } //end for: Iterate through layers.

    //Write the last Neuron Layer Axons to our output vector
    uint64_t i=0;
    for (NeuronPtrVect::iterator NeuronIter = 
        mFF_Layers.back()->GetLayerNeurons().begin(); NeuronIter !=
        mFF_Layers.back()->GetLayerNeurons().end(); ++NeuronIter)
    {
        mOutputVec[i] = (*NeuronIter)->ReadAxon();
        ++i;
    }
    
    mStaleCompute = false; //we are up to date.
    return true;
}

double FeedForwardNeuralNet::DoLearningCycle(
    const std::vector<double>& CorrectOutputs,
    double NumOfTrainingSamples,
    double LearningRate)
{

    if (mStaleCompute)
    {
        std::cout << "Compute Stale, forcing compute. " <<std::endl;
        //Force the compute
        if (!DoCompute())
        {
            std::cout << "Compute Failed. Bailing on Learning Cycle" 
                      << std::endl;
            return (0.0);
        }
    }


/*
//print debug
std::cout << "Fed Answer Vec |  Output Network Vec" << std::endl;
for (uint16_t i = 0; i < 10; ++i)
{
    std::cout << " SkyNet.ReadOutputs[" << i << "] = "
              << mOutputVec[i] << "  |  AnswerVec[" << i 
              << "] = " << CorrectOutputs[i] << std::endl;
}
*/



    //Does the Training Answer Vector match the size of our output array
    //Normally we dont care, but for training purps, we care.
    if (CorrectOutputs.size() != mOutputVec.size())
    {
        std::cout << "Error in DoLearningCycle: Need Training Results to be "
                  << "same size as Output vector" << std::endl;
        return 0.0;
    }

    //Now, we know the forward pass is done. Lets calculate the Error in
    //Network. First the O/P layers: BPEq 1-3
    HiddenLayers::reverse_iterator Prev_rLayerIt;
    for (HiddenLayers::reverse_iterator rLayerIt = mFF_Layers.rbegin();
         rLayerIt != mFF_Layers.rend(); ++rLayerIt)
    {
        
        if (rLayerIt == mFF_Layers.rbegin())
        { //we are on the last Layer. Do BP1.
        
            //iterate through Output Layer's Neuron's
            uint64_t CurJthNeuronIter = 0;
            for (NeuronPtrVect::iterator J_NeuronIter = 
                (*rLayerIt)->GetLayerNeurons().begin(); J_NeuronIter !=
                (*rLayerIt)->GetLayerNeurons().end(); ++J_NeuronIter)
            {   //Iterating through "jth Neuron in Lth (last/output) layer"
                //Whats our cost function:             
                switch (mNetCostFunc)
                {
                  case eLeastSquares:
                    //This means our Cost func is: C = sum (y(x)j - A^(L)j)^2
                    //Hence our derivative is:
                    //-2(y(x)j - A^(L)j)dA^(L)j
                    double OutputCostDerivCalcd = -2.0 *
                        ( CorrectOutputs[CurJthNeuronIter] -
                          (*J_NeuronIter)->ReadAxon() );
                    //Use the SetError as a trick to apply the phoType @
                    //weighted i/p for current error deriv (of calcd cost func)
                    (*J_NeuronIter)->SetError(OutputCostDerivCalcd);
                    break;

                  //case <someother cost func>:
                    //apply derivative given o/p trained samples. Dont apply pho
                    //then call (J_NeuronIter)->SetError(Your calc'd deriv).
               
                } //end switch: pick Cost Func.
                ++CurJthNeuronIter; 
            } //end for: done iterating through output layer
        } //end if BP1 application
        else
        { //we are on some other layer than last. DO BP2.

            //Apply elementwise version of BP2, as per my derivation.
            //Iterate through the current layer's Neurons, to calc the error
            //in each Neuron.
            uint64_t KthNeuronOffset = 0;
            for (NeuronPtrVect::iterator K_NeuronIter = 
                (*rLayerIt)->GetLayerNeurons().begin(); K_NeuronIter !=
                (*rLayerIt)->GetLayerNeurons().end(); ++K_NeuronIter)
            {   //Iterating through "kth Neuron in lth layer"
            
                //For each kth neuron, iterate through the jth Neurons in the
                //l+1 layer (l+1 layer = Prev_rLayerIt)
                double WeightedErrorSum = 0.0;
                for (NeuronPtrVect::iterator J_NeuronIter = 
                    (*Prev_rLayerIt)->GetLayerNeurons().begin(); J_NeuronIter !=
                    (*Prev_rLayerIt)->GetLayerNeurons().end(); ++J_NeuronIter)
                {
                    WeightedErrorSum += (*J_NeuronIter)->
                        GetDentritesRef()[KthNeuronOffset].mWeighting *
                        (*J_NeuronIter)->GetError(); 
                } //end for: Done calcing the sum for E^(l)k=W^(l+1)jk*E^(l+1)j                                    
                //Now calc the Error E(sum)*pho_deriv(Z^(l)k)
                (*K_NeuronIter)->SetError(WeightedErrorSum); //the SetError func
                    //will use the known activation func's deriv, to calc.

                ++KthNeuronOffset; //track which Kth Neuron we are on.
            } //end for: backprop: Error calc for kth Neuron in lth layer
        } //end if: BP2 application

        Prev_rLayerIt = rLayerIt; //backup previous reverse layer
    } //end for: iterate through all FF NN layers.

    //Note, we dont calc the backprop equation BP2 on the inputs b/c they dont 
    //have errors, but BP4 (weight deriv calc) still works for i/p layer.

    //At this point, all the errors are known. Now Lets calculate the delta 
    //Weight (or bias) to move, to implement Minimum optimization (Newton Root 
    //Finding). Applying BP3 & 4
    
    //Iterate through the Layers, start at first layer.
    HiddenLayers::iterator PrevLayerIt;
    for (HiddenLayers::iterator LayerIt = mFF_Layers.begin(); 
         LayerIt != mFF_Layers.end(); ++LayerIt)
    {
       
        //iterate through the jth Neurons in this lth layer 
        for (NeuronPtrVect::iterator J_NeuronIter = 
            (*LayerIt)->GetLayerNeurons().begin(); J_NeuronIter !=
            (*LayerIt)->GetLayerNeurons().end(); ++J_NeuronIter)
        {
            //Iterate through the Dentrites of the current jth Neuron.
            uint64_t CurDentIter = 0;
            for (std::vector<Neuron::Dentrite>::iterator CurDent = 
                (*J_NeuronIter)->GetDentritesRef().begin(); CurDent !=
                (*J_NeuronIter)->GetDentritesRef().end(); ++CurDent)
            {

                //Know where to grab the dentrite i/ps from. (1st layer
                //depenant). This applies BP4 and calc/applies delta -n
                if (LayerIt == mFF_Layers.begin())
                { //Means we are on the first layer, need to use i/p vector
                    //calc dC/Dwjkl
                    CurDent->UpdateWeight(mInputVec[CurDentIter],
                                          (*J_NeuronIter)->GetError(),
                                          NumOfTrainingSamples,
                                          LearningRate); //Apply BP4 and update
                }
                else
                {   //Means we are on the > first layer.can use prev layer
                    //calc dC/Dwjkl
                    CurDent->UpdateWeight(
                        (*PrevLayerIt)->GetLayerNeurons()[CurDentIter]->
                            ReadAxon(),
                        (*J_NeuronIter)->GetError(),
                        NumOfTrainingSamples,
                        LearningRate); //apply BP4 and update Weight.
                } //end if: what layer are we on.
                ++CurDentIter;
            } //done iterating through Neuron J(lth layer) dentrites 

            //update the bias calcs, for this Jth Neuron. BP3 & apply our deltas
            (*J_NeuronIter)->UpdateBias(NumOfTrainingSamples,
                                        LearningRate);
        } //end for: Iterate through Jth Neurons in lth Layer
        PrevLayerIt = LayerIt; //backup this layer for next iteration 
                               //(yields l-1)
    } //Iterate through the Layers

    return 0.0;
}

void FeedForwardNeuralNet::DebugPrint(bool PrintWeights,
                                      bool PrintBiases,
                                      bool PrintError,
                                      bool PrintDentriteInputs,
                                      bool PrintAxonOutputs,
                                      bool PrintInputVect,
                                      bool PrintOutputVect)
{
    
    if (PrintInputVect)
    {
        for (uint64_t i = 0; i < mInputVec.size(); ++i)
        {
            std::cout << " InputVect[" << i << "] = " << mInputVec[i] <<
                std::endl;
        } //end for
    } //end if

    if (PrintOutputVect)
    {
        for (uint64_t i = 0; i < mOutputVec.size(); ++i)
        {
            std::cout << " OutputVect[" << i << "] = " << mOutputVec[i] <<
                std::endl;
        } //end for
    } //end if

    ///Iterate through all layers, Neurons and Dentrites.
    std::cout << " --Network Details-- " << std::endl;
    
    uint64_t CurLayerCnt = 0;
    for (HiddenLayers::iterator LayerIt = mFF_Layers.begin(); 
        LayerIt != mFF_Layers.end(); ++LayerIt)
    {
        std::cout <<"   " << "Layer: " << CurLayerCnt << std::endl;
        //iterate through the Neurons in this lth layer 
        uint64_t CurNeuronCnt = 0;
        for (NeuronPtrVect::iterator NeuronIter = 
            (*LayerIt)->GetLayerNeurons().begin(); NeuronIter !=
            (*LayerIt)->GetLayerNeurons().end(); ++NeuronIter)
        {
            std::cout <<"   " << "  " << "Neuron: " << CurNeuronCnt << std::endl;
  
            std::cout <<"   " << "  " << "  " << "Error: " 
                      << (*NeuronIter)->GetError() <<  std::endl;
            std::cout <<"   " << "  " << "  " << "mBias: " 
                      << (*NeuronIter)->mBias <<  std::endl;
            std::cout <<"   " << "  " << "  " << "WeightedInput: " 
                      << (*NeuronIter)->mWeightedInput <<  std::endl;
             std::cout <<"   " << "  " << "  " << "AxonOut: " 
                      << (*NeuronIter)->ReadAxon() <<  std::endl;
    
            //Iterate through the Dentrites of the current Neuron.
            uint64_t CurDentCnt = 0;
            for (std::vector<Neuron::Dentrite>::iterator CurDent = 
                (*NeuronIter)->GetDentritesRef().begin(); CurDent !=
                (*NeuronIter)->GetDentritesRef().end(); ++CurDent)
            {
                std::cout <<"   " << "  " << "  " << "Dentrite: " 
                          <<CurDentCnt <<std::endl;
                std::cout <<"   " << "  " << "    " << "mWeighting: " 
                          <<CurDent->mWeighting << std::endl;
                std::cout <<"   " << "  " << "    " << "mInputValue: " 
                          <<CurDent->mInputValue << std::endl;

                ++CurDentCnt;
            } //end iter: Dentrite
            ++CurNeuronCnt;
        } //end iter: NeuronIt
        ++CurLayerCnt;
    } //end for: LayerIt



}

bool FeedForwardNeuralNet::SaveNetworkConfig(std::string FileName,
                                             bool OverwriteExisting)
{

#ifdef dfdfgdfg
    ifstream IdxFile(RawDataFileName, ios::in | ios::binary);
    if (!IdxFile)
    {
        std::cout << "idx_rdr: Error opening file." << std::endl;
        //error!
        return;
    }

    SysTools::FileGuard IdxGaurd(IdxFile); //make sure we close it.

 
 
    //Write a 4-byte number to indicate its a feed forward network

    //Try and read the Magic Number
    memset(TempBuf, '\0', sizeof(TempBuf));
    IdxFile.read(TempBuf, sizeof(uint32_t));
    if (!IdxFile)
    {
        std::cout << "idx_rdr: Error reading file." << std::endl;
        //error!
        return;
    }
    mMagicNumber = SysTools::ToMachineEndianess<uint32_t>(&TempBuf[0],
        SysTools::eBigEndian);

#endif
    return true;
}

bool FeedForwardNeuralNet::LoadNetworkConfig(std::string FileName)
{

    //can we open the file

    //Read a 4-byte number to indicate its a feed forward network

    return true;
}



///////////////////////////////////////
/// Hidden Layer Class Funcs
///////////////////////////////////////

FeedForwardNeuralNet::HiddenLayer::HiddenLayer() :
    mWeightUpperRange(0.0),
    mWeightLowerRange(0.0),
    mUserWeight(0.0), 
    mpNeuronsInLayer(),
    mDentInitWeightType(FeedForwardNeuralNet::DentriteInitWeightType::eZero)
{
}

FeedForwardNeuralNet::HiddenLayer::~HiddenLayer()
{
}

NeuronPtrVect& FeedForwardNeuralNet::HiddenLayer::
	GetLayerNeurons()
{
	return mpNeuronsInLayer;
}

void FeedForwardNeuralNet::HiddenLayer::SetDentWeightType(
    enum DentriteInitWeightType value)
{
    mDentInitWeightType = value;
}

const enum FeedForwardNeuralNet::DentriteInitWeightType& 
    FeedForwardNeuralNet::HiddenLayer::GetDentWeightType() const
{
    return mDentInitWeightType;
}


