/*****************************************************************************
 * File: FeedForwardNeuralNet.h
 * Written By: Nate Groendyk
 * Date: 26 June 2018
 *
 * Purpose: Neural Network definition
 *
 * For our implementation of a Neural Network, 
 * There is a seperate input layer which always exists, and acts as Axons
 * to the first of the Hidden Layers Neurons. (so just as Feed Forward Network
 * topology is for the rest of the network, our i/ps are hooked to every neuron
 * in the first hidden layer)
 *
 * Also the last layer added in, its Axons are what get tied
 * to the outputs of your neural network.
 * You can have any number of hidden layers, and each hidden
 * layer can have any number of nerons, and hidden layers
 * do not need to maintain the same # of neurons.
 *****************************************************************************/

#ifndef FEED_FORWARD_NEURAL_NET_H
#define FEED_FORWARD_NEURAL_NET_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/container/detail/pair.hpp>
#include <map>
#include <vector>

#include <iostream>
#include "Neuron.h"

//using namespace std;

class FeedForwardNeuralNet
{
    
  public:

    /**< enum for specifying how Dentrites can be initialized*/
    enum DentriteInitWeightType
    {
        eZero = 0,
        eRandom,
        eUserSpec,
    };

    /**< enum for specifying the cost function type. For every built cost
     * function, you also need to add to the enum, and calc its derivative
     */
    enum CostFuncType
    {
        eLeastSquares = 0
    };

    /**
     * This class maintains the hidden layers of our F.F. Neural Net.
     */
	class HiddenLayer
	{
	  public:
		HiddenLayer();
		~HiddenLayer();

    	/**
         * Returns a reference to the Vector of Neurons in this hidden layer
         * Our ANN implementation stores all the Neurons for a given layer
         * in a Vector. This returns a reference to said Neuron Vector.
         */	
        NeuronPtrVect& GetLayerNeurons();
        
        void SetDentWeightType(DentriteInitWeightType value); /**< Setter */
        const DentriteInitWeightType& GetDentWeightType() const; /**< Getter */ 

        /**< Used for Initial Weight calcs for Neurons in this layer*/
        double mWeightUpperRange;
        double mWeightLowerRange;
        double mUserWeight;
	  private:
        NeuronPtrVect mpNeuronsInLayer;
        DentriteInitWeightType mDentInitWeightType;
	};
    typedef std::shared_ptr< HiddenLayer > HiddenLayerPtr;
    typedef std::vector< HiddenLayerPtr > HiddenLayers; /**< Vector of Layers*/


  private:
    //MEMBER VARIABLES

    HiddenLayers mFF_Layers; /**< Vector that holds all the hidden layers */
    std::vector< double > mInputVec;    /**< Holds currently passed in inputs */
    std::vector< double > mOutputVec;   /**< Holds Post compute results       */
    bool mIsNetworkLockedIn;
    enum CostFuncType mNetCostFunc; /**< Chosen Cost function Type for FF ANN */
    bool mStaleCompute; /**<Keep track of When compute happened last */

    static bool mzRandSet; /**< Keep track of if Random # generator is seeded */
    

  public:

	FeedForwardNeuralNet(uint64_t NumOfInputs, 
                         uint64_t NumOfOutputs,
                         enum CostFuncType ChosenCostFunction);
	~FeedForwardNeuralNet();

    /**
     * Our implementation for FF ANN sets up all the Hidden layers as a Vector
     * where each element holds its own HiddenLayer of Neurons.
     */
    HiddenLayers& GetFF_Layers(); /**< Getter for Hidden Layers Vector */
    
	/**
	 * This function loads the ANN mapped Input Neurons with
	 * values of the passed in Input Vector
	 *
     * @param Inputs	A vector holding the input values
	 */
	bool LoadInputs(const std::vector<double>& Inputs);
	
	/**
	 * This function loads an output vector with Axon outputs
	 *
     * @param Outputs	A vector holding the input values
	 */
	bool ReadOutputs(std::vector<double>& Outputs);


    /**
     * This function makes designing and creating a Feed Forward Neural Net easy
     *
     * This function makes designing and creating a FeedForward Neural Net easy.
     * Basically what you do is form a string that gets parsed by this function
     * indicating Many specifics about your Neual Net. Here's the string
     * breakdown:
     */
    bool EZCreateFFNN(std::string CreationStr);

	
    /**
     * After you set up the Hidden Layers, and the Neurons in the layers
     * You need to "Lock in" the network, so that the Dentrites can all be
     * created and populated with initial weights and biases.
     * Note: Before you look in the network, you need to make sure the last
     * layer has the same # of Neurons as your Output vector.
     */
    bool LockNetwork();

	/**
	 * This function does 1 computation across the entire
	 * FeedForward ANN given the current loaded input. Once
	 * done you can read the output Axons to obtain the value
	 * Note, this functions affects all Neurons in the ANN
     *
     * Note: It also calcs the current Weighted i/p "Z"
	 */
	bool DoCompute();

    /**
     * Loads Training output data, and compute new weights (Learn!)
     *
     * So how the Feed Forward Neural Net works is by first giving it
     * input samples, but also running a Training cycle on it, where
     * for a given i/p, it calcs the output, but then you feed it the correct
     * answer (in the training cycle) which it then uses to adjust its internal
     * dentrite weights to better calculate the output.
     * So big things here, are usually the # of training samples is "known"
     * (or you can rock the moving average window), and a learning rate.
     * delta to move by. These are input parms.
     *
     * @param CorrectOutputs The vector that contains the correct output
     *                       results for the currently loaded, post-computed
     *                       input.
     * @param NumOfTrainingSamples The number of samples you want the average
     *                             to make up.
     * @param LearningRate The rate at which we learn in the network, or more
     *                     precisely the delta we move from each derivative
     *                     to get back to the minimum of the cost curve.
     */
    double DoLearningCycle(const std::vector<double>& CorrectOutputs,
                           double NumOfTrainingSamples,
                           double LearningRate);

    /**
     * Debug print the Network Stuffs
     *
     * @param PrintWeights
     * @param PrintBiases
     * @Param PrintError
     * @Param PrintDentriteInputs
     * @param PrintAxonOutputs
     * @param PrintInputVect,
     * @param PrintOutoutVect
     */
    void DebugPrint(bool PrintWeights,
                    bool PrintBiases,
                    bool PrintError,
                    bool PrintDentriteInputs,
                    bool PrintAxonOutputs,
                    bool PrintInputVect,
                    bool PrintOutputVect);



    /**
     * Save network Config.
     *
     * This function takes a snapshot of all the neurons, dentrites, and
     * their related weights. It does not save intermediate values like
     * Errors and weighted sums. (since these can be recalc'd quickly from
     * a forward pass and learning back-pass)
     *
     * @param FileName The name of the file to save the network as.
     * @param OverwriteExisting Set if you can overwrite pre-existing files.
     *                          else set false and if file exists, this fails.
     */
    bool SaveNetworkConfig(std::string FileName, bool OverwriteExisting);

    
    /**
     * Load network config.
     *
     * This function loads a previously saved snapshot of all tlhe neurons,
     * dentrites, and their related weights. It also locks the network so the
     * config is usable right away.
     *
     * @param FileName The filename to use when loading the network.
     */
    bool LoadNetworkConfig(std::string FileName);






     /** Save Weights and Network config */
     /** Load weights and network config */


    /** EZ network setup given the string chars I used be4 */


	/**
	 * This function does several things. First, given the
	 * specified size, it adds that many new Neurons to the
	 * current layer (which better be the first layer, if no
	 * layers exists, it creates the first layer).
	 *
	 * Second, it is assumed that a user will want to pass in
	 * a vector as the input values (When the ANN compute
	 * happens). This means that we need to maintain a map of
	 * input Array offsets to the actual Neuron Ids, (or
	 * shared pointers). So instead of user taking care of
	 * that, we build that map inside the ANN class (for use 
	 * by the "LoadInputVector" func.
	 *
	 * Third, we create 1 dentrite and setup default bias and
	 * weightings.
	 *
	 * @param InputVectorSize The size of the input vector in
									  elements
	 * @param Weight The Default weighting of the 1 dentrite.
	 * @param Bias The Default Bias of the 1 dentrite.
	 */

/*	bool CreateInputNeuronsAndVectorMap(
			uint64_t InputVectorSize,
			double Weight,
			double Bias);
*/
	
	/**
	 * This function does several things. First, given the
	 * specified size, it adds that many new Neurons to the
	 * current layer (to be used as outputs).
	 *
	 * Second, it is assumed that a user will want to pass in
	 * a vector as the output values vector (after the ANN 
	 * compute happens). This means that we need to maintain 
	 * a map of output Array offsets to the actual Neuron Ids,
    * (or shared pointers). So instead of user taking care of
	 * that, we build that map inside the ANN class (for use 
	 * by the "LoadOutputVector" func.
	 *
	 * @param OutputVectorSize The size of the output vector in
	 *								   elements
	 * @param Weight The Default weighting of the 1 dentrite.
	 * @param Bias The Default Bias of the 1 dentrite.
	 */
/*
	bool CreateOutputNeuronsAndVectorMap(
			uint64_t OutputVectorSize, double Weight, 
			double Bias);
*/


//pay rent and jason and taxes'

	/*Build a hook up function to create the attachments of
	 *neurons outputs (and sensor outputss or raw input sites)'    *to the neuron dentrites. track connetions with a std::map
    *
    *Build functions to setup the number of neurons to add
	 *on a per-hidden-layer-basis. design hookup func to
    *repect the new hooks.
    *
    *Create a calc network func which takes currently loaded
    *raw site inputs, passes them to the neurons, via their
	 *synapse func. do this for each hidden layer across all
    *neurons in that layer.
    *
    * Add fnctions whcich calcs the error of the weights and
    * adjusts the weights.
    *
    * note the additional hidden layers act like a Moving average window filter, each layer is anoter "order"
	 */


/** Some other thoughts around funcs, we need a better way to handle graident
 * fading....how does the brain handle it.? multiplier? */

};

#endif
