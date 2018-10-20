/*****************************************************************************
 * File: NeuralNetTest.cpp
 * Written By: Nate Groendyk
 * Date: 26 June 2018
 *
 * Purpose: Test a Neural Network
 *****************************************************************************/

#include <iostream>
#include "FeedForwardNeuralNet.h"
#include <vector>
#include "SysTools.h"
//#include "idxrdr.h"
#include "MNist.h"
#include <cstdlib>


using namespace std;
using namespace SysTools;


int main(int argc, char* argv[])
{

	FeedForwardNeuralNet SkyNet(28 * 28, 10, FeedForwardNeuralNet::eLeastSquares);
//    uint32_t elePick=0; 
//    elePick = atol(argv[1]);    

    std::cout << "Loading Training set..." << std::endl;

    

/*
    idx_rdr TrainingDataAnswers("./data/train-labels-idx1-ubyte");
//    TrainingDataAnswers.DebugPrint();
  
    idx_rdr TrainingData("./data/train-images-idx3-ubyte");
//    TrainingData.DebugPrint();
*/

/*
 
    //Add First Layer.
    SkyNet.GetFF_Layers().push_back(
        FeedForwardNeuralNet::HiddenLayerPtr(new 
        FeedForwardNeuralNet::HiddenLayer));

    FeedForwardNeuralNet::HiddenLayer& CurLayer = 
        *(SkyNet.GetFF_Layers().back());

    //configure that layers Neurons Weight Init types.
    CurLayer.mUserWeight = .10;
    CurLayer.SetDentWeightType(
        FeedForwardNeuralNet::DentriteInitWeightType::eUserSpec);

    //Add some Neurons to layer. 2
    for (int i = 0; i < 2; ++i)
    {
        CurLayer.GetLayerNeurons().push_back(NeuronPtr
            (new Neuron(Neuron::ActivationFunc::eSigmoid)));
    } 
 ////////////
    //Add another layer
    SkyNet.GetFF_Layers().push_back(
        FeedForwardNeuralNet::HiddenLayerPtr(new 
        FeedForwardNeuralNet::HiddenLayer));

    FeedForwardNeuralNet::HiddenLayer& CurLayer2 = 
        *(SkyNet.GetFF_Layers().back());

    //configure that layers Neurons Weight Init types.
    CurLayer2.mUserWeight = .15;
    CurLayer2.SetDentWeightType(
        FeedForwardNeuralNet::DentriteInitWeightType::eUserSpec);

    //Add some Neurons to layer. 3
    for (int i = 0; i < 3; ++i)
    {
        CurLayer2.GetLayerNeurons().push_back(NeuronPtr
            (new Neuron(Neuron::ActivationFunc::eSigmoid)));
    } 
 //////////////

    //Add another layer
    SkyNet.GetFF_Layers().push_back(
        FeedForwardNeuralNet::HiddenLayerPtr(new 
        FeedForwardNeuralNet::HiddenLayer));

    FeedForwardNeuralNet::HiddenLayer& CurLayer3 = 
        *(SkyNet.GetFF_Layers().back());

    //configure that layers Neurons Weight Init types.
    CurLayer3.mUserWeight = .32;
    CurLayer3.SetDentWeightType(
        FeedForwardNeuralNet::DentriteInitWeightType::eUserSpec);

    //Add some Neurons to layer. 2
    for (int i = 0; i < 2; ++i)
    {
        CurLayer3.GetLayerNeurons().push_back(NeuronPtr
            (new Neuron(Neuron::ActivationFunc::eSigmoid)));
    } 

/////////////
*/


    //Add First Layer.
    SkyNet.GetFF_Layers().push_back(
        FeedForwardNeuralNet::HiddenLayerPtr(new 
        FeedForwardNeuralNet::HiddenLayer));

    FeedForwardNeuralNet::HiddenLayer& 
    CurLayer(*(SkyNet.GetFF_Layers().back()));

    //configure that layers Neurons Weight Init types.
//    CurLayer.mUserWeight = 1.0 / (28.0 * 28.0 * 255.0);
//    CurLayer.mUserWeight = 0.010; //(1.0/256.0)*(1.0/(28.0*28.0));
//    CurLayer.mUserWeight = (1.0/255.0);
    CurLayer.mWeightUpperRange = 1.0 / (28.0 * 28.0 * 255.0);

    CurLayer.mWeightLowerRange = -1.0 / (28.0 * 28.0 * 255.0);



    CurLayer.SetDentWeightType(
        FeedForwardNeuralNet::DentriteInitWeightType::eRandom);

    //Add some Neurons to layer. 1
    for (int i = 0; i < 15; ++i)
    {
        CurLayer.GetLayerNeurons().push_back(NeuronPtr
            (new Neuron(Neuron::ActivationFunc::eSigmoid)));
    } 
 ////////////
    //Add another layer
    SkyNet.GetFF_Layers().push_back(
        FeedForwardNeuralNet::HiddenLayerPtr(new 
        FeedForwardNeuralNet::HiddenLayer));

    FeedForwardNeuralNet::HiddenLayer& 
        CurLayer2(*(SkyNet.GetFF_Layers().back()));

    //configure that layers Neurons Weight Init types.
    //CurLayer2.mUserWeight = 1.0/15.0; //1.0/15.0;
    CurLayer2.mWeightUpperRange = 1.0/15.0;
    CurLayer2.mWeightLowerRange = -1.0/15.0;

    CurLayer2.SetDentWeightType(
        FeedForwardNeuralNet::DentriteInitWeightType::eRandom);

    //Add some Neurons to layer. 2
    for (int i = 0; i < 10; ++i)
    {
        CurLayer2.GetLayerNeurons().push_back(NeuronPtr
            (new Neuron(Neuron::ActivationFunc::eSigmoid)));
    } 
 //////////////

    
    //Lock in the Net.
    if (SkyNet.LockNetwork())
    {
        std::cout << "Locked the Network config in" << std::endl;
    }
    else
    {
        std::cout << "LOCKED FAILED! " << std::endl;
    }



    //Load the MNist DataSet
    MNist MNistData("./data/train-labels-idx1-ubyte",
                    "./data/train-images-idx3-ubyte");

   /* 
    std::cout << "Element: " << elePick << ", Answer: " 
              << static_cast< int16_t>(MNistData.GetAnswer(elePick)) 
              << std::endl << std::endl;
    MNistData.DebugPrintImage(elePick);
    std::cout << std::endl;
    std::vector< double > OutV(10);
    MNistData.GetAnswer(elePick, OutV);
    for ( uint16_t i = 0; i < 10; i++)
    {
        std::cout << "OutVec[" << i << "] = " << OutV[i] << std::endl;
    }
    */

uint32_t LearningCycleCnt = 0;
uint32_t GoodMatchCnt = 0;
 
for (;;)  
{ 
    GoodMatchCnt = 0;
      
    //Load the Training images into the Neural net.
    for (uint32_t ImgCnt = 0; ImgCnt < 50000; ImgCnt++)
    {
        //Get Image.
        std::vector< std::vector< double > > TrainIm(28, std::vector< double >
            (28));
        MNistData.GetImage(ImgCnt, TrainIm);
        std::vector< double >Image1D;
        SysTools::From2Dto1D< double >(TrainIm, Image1D);

        //Get Answer Vector.
        std::vector < double > TrainAnsVec(10);;
        MNistData.GetAnswer(ImgCnt, TrainAnsVec);

        //Map to input of NN
        SkyNet.LoadInputs(Image1D);
        SkyNet.DoCompute();
        
        //Build the output vector.
/*
MNistData.DebugPrintImage(ImgCnt);
std::cout <<"------------" <<std::endl;
for (uint32_t it2 = 0; it2 < 10; ++it2)
{
    std::cout << "TrainAnsVec[" << it2 << "] = " << TrainAnsVec[it2] <<
        std::endl;
}
*/
        //SkyNet.DoLearningCycle(TrainAnsVec, 50000, 0.5);
        //SkyNet.DoLearningCycle(TrainAnsVec, 1.0, 3.0);

//Mods made: Set the biases always to start at zero (dont know our error at
//start)
// changed NumOf Samples to always be  a double, no mixing types.

// 3) theres still an issue withthe randon number generator version.
// 4) Also lots of wastefull copying around
        SkyNet.DoLearningCycle(TrainAnsVec, 50000.0, 3.0);



/*
        if (!(ImgCnt % 1000)) 
        {
            std::cout << "ImgCnt: " << ImgCnt << std::endl;
        }
*/
    } //end for: ImgCnt


    //Now Test how well it does.
    //bool GoodMatch;
    for (uint32_t ImgCnt = 50000; ImgCnt < 60000; ImgCnt++)
    {
        //Get Image.
        std::vector< std::vector< double > > TestIm(28, std::vector< double >
            (28));
        MNistData.GetImage(ImgCnt, TestIm);
        std::vector< double >Image1D;
        SysTools::From2Dto1D< double >(TestIm, Image1D);

        //Get Answer Vector.
        std::vector < double > TestAnsVec(10);;
        MNistData.GetAnswer(ImgCnt, TestAnsVec);

        //Map to input of NN
        SkyNet.LoadInputs(Image1D);
        SkyNet.DoCompute();

        //Get the outputs
        std::vector< double > NetOutput(10);
        for (uint16_t outIt = 0; outIt < 10; outIt++)
        {
            NetOutput[outIt] = 0.0;
        }
        SkyNet.ReadOutputs(NetOutput);

/*
        //print some results
        std::cout << "---------------------------------------" << std::endl;
        MNistData.DebugPrintImage(ImgCnt);
        std::cout << "Ans: " << static_cast< int16_t >
            (MNistData.GetAnswer(ImgCnt)) << std::endl;
        std::cout << "Network Calc'd Output: " << std::endl;
        for (uint16_t it = 0; it < 10; it++)
        {
            std::cout << "NetResult[" << static_cast< int16_t >(it) << "] = " 
                << NetOutput[it] << std::endl;
        }
        std::cout << "---------------------------------------" << std::endl;
*/


        std::cout << "Answer Vec |  Output Network Vec" << std::endl;
        for (uint16_t i = 0; i < 10; ++i)
        {
            std::cout << " SkyNet.ReadOutputs[" << i << "] = "
                      << NetOutput[i] << "  |  AnswerVec[" << i 
                      << "] = " << TestAnsVec[i] << std::endl;
        }

        //Compare the current output vector, to the answer.
/*
        GoodMatch = false;
        for (uint8_t it = 0; it < 10 ; ++it)
        {
            if (NetOutput[it]  >= 0.8)
            {
                if (MNistData.GetAnswer(ImgCnt) == it)
                { //we hit a matching number
                    GoodMatch = true;
                }
                else
                {   
                    GoodMatch = false;
                }
            }

        } //loop through the digits.
*/
/* Sincewe dont get nice  0, 1, variations in output guesses, lets simply
   look for the max of the outputs. */

        if  (SysTools::max(NetOutput) == MNistData.GetAnswer(ImgCnt))
        {
            ++GoodMatchCnt;
        }
        //check if it failed.
        //if (GoodMatch)
        //{
        //    ++GoodMatchCnt;
       // }
/*
        //Report some results
        if (!(ImgCnt % 1000))
        {
            std::cout << "ImgCnt: " << ImgCnt << ". Good Matches: " 
                      << GoodMatchCnt << std::endl;
        }
*/
    } //end for: ImgCnt

    std::cout << "Cycle Cnt: " << LearningCycleCnt << ". Good Matches: " << GoodMatchCnt << std::endl;
    ++LearningCycleCnt;
}
















 


/*

    //Load in the training samples
    std::cout << "Running Training Data..." << std::endl;
    std::vector < uint8_t > InVal;
    if (!TrainingData.GetData(InVal))
    {
        std::cout << "Couldnt Get the data from GetData()" 
                  << std::endl;
        assert(false); 
    }
    std::vector < uint8_t > OutVal;
    if (!TrainingDataAnswers.GetData(OutVal) )
    {
        std::cout << "Couldnt get the data from GetData()"
                  << std::endl;
        assert(false);
    }

    uint32_t SampEle = 0;
    for (uint32_t SampCnt = 0; SampCnt < 50000; ++SampCnt)
    {
        std::vector< double > InTrainingSamp;

        //Build the current image as an input
        for (uint32_t r = 0; r < 28; ++r)
        {
            for (uint32_t c = 0; c < 28; ++c)
            {
                InTrainingSamp.push_back(InVal[SampEle]);
                ++SampEle;
            }
        }

        //After 28*28 pixel's we have our data
        SkyNet.LoadInputs(InTrainingSamp); //load the current image.
        SkyNet.DoCompute();

        //Build the output vector.
        std::vector < double > TrainOutput({0.0, 0.0, 0.0, 0.0, 0.0, 
                                            0.0, 0.0, 0.0, 0.0, 0.0});
        TrainOutput[OutVal[SampCnt]] = 1.0;
       
        //Pass it in as a training vector.
        SkyNet.DoLearningCycle(TrainOutput, 50000.0, 0.5);      
    
        if (!(SampCnt % 1000))
        {
            std::cout << "SampCnt: " << SampCnt << " Size: " 
                      << InTrainingSamp.size() << std::endl;
        } 
        InTrainingSamp.clear();
    } //iterate through the samples.     

    //Network is trained!
    std::cout << "Network is trained! " << std::endl;

    SkyNet.DebugPrint(true, true, true, true, true, true, true);


    std::cout << "Running the test. " << std::endl;





    //Lets Pass the values through and see how we do.
    bool GoodMatch;
    uint32_t GoodMatchCnt = 0;
    for (uint32_t GuessOpCnt = 50000; GuessOpCnt < 60000; ++GuessOpCnt)
    {
         std::vector< double > InDataSamp;

        //Build the current image as an input
        for (uint32_t r = 0; r < 28; ++r)
        {
            for (uint32_t c = 0; c < 28; ++c)
            {
                InDataSamp.push_back(InVal[SampEle]);
                ++SampEle;
            }
        }

        //After 28*28 pixel's we have our data
        SkyNet.LoadInputs(InDataSamp); //load the current image.
        SkyNet.DoCompute();

        //Build the output vector.
        std::vector < double > ObservedOutput({0.0, 0.0, 0.0, 0.0, 0.0,
                                               0.0, 0.0, 0.0, 0.0, 0.0});

        SkyNet.ReadOutputs(ObservedOutput);
        //Find the value that is 0.75 or higher
        GoodMatch = false; //assume bad.
        for (uint16_t OutIcnt = 0; OutIcnt < 10; ++OutIcnt)
        {
            if (ObservedOutput[OutIcnt] >= 0.75 )
            {
                //check if read "hi" output matches the correctly set letter
                //from the training answers.
                if ( OutVal[GuessOpCnt] != OutIcnt)
                { //oh no didnt match!
                    GoodMatch = false;
                    break;           
                }
                else
                {
                    GoodMatch = true;
                }
            } //end check for "set" letters
        } 

        //How did this match go?
        if (GoodMatch)
        {
            ++GoodMatchCnt;
        }
        InDataSamp.clear();

        if (!(GuessOpCnt % 1000))
        {
            std::cout << "Images Processed so far: " << GuessOpCnt 
                      << ". Num of matched: " << GoodMatchCnt << std::endl;
        }

    } 

    std::cout << "Good Match Cnt: " << GoodMatchCnt << " total images:10000 "
              << std::endl;
*/

    return 0;


////////////
    std::vector< double > In({1,2,3});
    std::vector< double > TrainedOutV({.5,.98});
 

std::cout<< "1 - Load IP & compute" << std::endl;
    SkyNet.LoadInputs(In); //Now Load Inputs
    SkyNet.DoCompute(); //Do the Compute
std::cout << "1 - Weights, Biases, inputs, and Outputs of Network" << std::endl;
    SkyNet.DebugPrint(true, true, true, true, true, true, true); 
std::cout << "1 - Now do the Learning Cycle" << std::endl;


    SkyNet.DoLearningCycle(TrainedOutV,
                           1,
                           1);
std::cout << "1 - Learning Cycle Results: Weights & Biases)" << std::endl;


    SkyNet.DebugPrint(true, true, true, true, true, true, true); 


     
    
    //Pass in Training samples
          


    //Do a learning cycle


	//Build Our Input Layer
//	SkyNet.AddHiddenNetworkLayer();

	//Add Input Neurons
/*
	for (int i = 0; i < 10; i++)
	{
		NeuronPtr pCurNeuron = SkyNet.AddNeuronToLayer();
		if (pCurNeuron)
		{	
			cout << "Neuron Id: " << pCurNeuron->GetId() << endl;
			Neuron::Dentrite& CurDent(pCurNeuron->AddDentrite());
			CurDent.mWeighting=1.0;

		}	
	}
*/

	
	//Map the i/p Neurons to our input Vector.
//	SkyNet.CreateInputNeuronsAndVectorMap(10, 1.0, -0.5);


	//Add A Hidden Layer
//   SkyNet.AddHiddenNetworkLayer();

	//Give it some Neurons.

/*
	for (int i = 0; i < 100; i++)
	{
		NeuronPtr pCurNeuron = SkyNet.AddNeuronToLayer();
		if (pCurNeuron)
		{	
			cout << "Neuron Id: " << pCurNeuron->GetId() << endl;

		}	
	}
*/

/*

RANDOM TTHOUGHTS
"the flowy ness of a bio neural network is not adequately mimicd in todays ANNs"

Heres the issue, Each time or sample new data comes in the entire network is reiterated upon. But in realife its different. Also in realife its digital, but we are passing doubles as action potentials.

So think about it, I see something life a flash of light a couple of my receptors fire. My brains networks arent all fired at once, but rather only the affected neurons that fire matter, and maybe it takes a few other firings that are interleaved between new data samples, meaning there is semi-neural processing or even feedback loops that are happening between data samples. Our ANN networks are shit in comparison, because you dont get this fluidity in how the data processed, rather you get a complete compute across the whole network. For another example: suppose in the brain prop delay is important for doing some phase relationships, you wont see that in our FF ANN since data all props a 
t the same rate


MORE RANDOM THOUGTS:
In the Feed Forward Neural Net form it isnt possible to say "learn new things" or rather to "classify new information" meaning, the output neurons which reps things we want to know about, dont actually change in number (well they can shrink but dont grow), this results in a limit to our classification ability. An example: we currently train our nets weights based on "Here is the data, here is the actual result...adjust weights to minimize the error." (Hence there is also a thing to be said about resending the same dataset in a row to hit min error weights, but I digress). In this example, we always know "what we want", and this doesnt represent our actual brains ability to learn new things.... "Oh this is a Epsilon Delta proof, and this is how it works", or "Oh this is caled a star fruit?" How can we capture this type of adaptive learning and reasoning

*/



	//Add an output Hidden Layer (last Layer)
//	SkyNet.AddHiddenNetworkLayer();

	//Add and Map the output Neurons to our output Vector. 
//	SkyNet.CreateOutputNeuronsAndVectorMap(2,1.0,-0.5); //2 output elements

	//Hook up the NeuralNet
//	SkyNet.InitialHookUp();
	
	
	//--Now lets start training--//
	std::vector <double> InVals;
	std::vector <double> OutVals;

	//make the InVals as big as is needed
	//make the OutVals as big as is needed

	//Load an actual onput set into InVals;
	//Send the input set into the ANN
//	SkyNet.LoadInputs(InVals);

	//Calculate the network
//	SkyNet.DoCompute();

	//Get Outputs.
//	SkyNet.ReadOutputs(OutVals);
	//Read them


	//--Now lets start passing regular data into it.--//

	//--and lets see how well it does--//


	cout << "Hello World" << endl;




	return 0;
}
