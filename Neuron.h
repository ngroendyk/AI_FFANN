/*****************************************************************************
 * File: Neuron.h
 * Written By: Nate Groendyk
 * Date: 2 June 2018
 * 
 * Purpose: Definition of a Neural Network Neuron
 ****************************************************************************/
#ifndef NEURON_H
#define NEURON_H

//#include <boost/container/list.hpp>
#include <list>
#include <stdint.h>
#include <vector>

#include <boost/config.hpp>
#include <boost/container/detail/pair.hpp>
#include <boost/shared_ptr.hpp>

class Neuron;

typedef boost::shared_ptr<Neuron> NeuronPtr;
typedef std::list< NeuronPtr> NeuronPtrList;
typedef std::vector <NeuronPtr> NeuronPtrVect;
typedef double AxonOutput;
typedef uint64_t NeuronId;

class Neuron 
{
  public:

    enum ActivationFunc
    {
        eSigmoid = 0
    };

   	class Dentrite
	{
	  
	  public:
		Dentrite();
		~Dentrite();
		double mWeighting;
		double mInputValue;

        /**
         * This function handles the math in updating the Weights of a dentrite.
         *
         * At this point of function call, the forward pass must be done and the
         * backprop errortraining pass must be done. (givingus legit Neuron Error
         * amounts for the latest i/p, o/p training set, so this func only really
         * gets called by the FeedForwardNeuralNet class's DoLearningCycle().
         *
         * Anyway, this is a classic calc of BP eq #4, and also the inclusion of
         * the new weight calc. So you need to pass is the related previous layer
         * Neuron "k"'s axon output (that matches with the current dentrite)
         * and the current Neuron (that the dentrite lives in) Error amount.
         *
         * @param l_minus1KthNeuron_AxonOutput The l-1 kth Neuron Axon Ouput as
         *                                     described above.
         * @param l_jthNeuron_Error The error of the jth Neuron in the lth layer
         * @param NumOfTrainingSamples The total # of training samples used
         *                             to train this neural network....or the 
         *                             window of samples to use.
         * @param LearningRate The learning rate to use to pick a small enough
         *        delta to not over jump the minimum, but large enough to learn
         *        quickly. Basically we get this # multiplied by the computed
         *        error derivative (aka, the bias derivative). Normally this #
         *        should be small so a # between 0 and 1.
         */
        bool UpdateWeight(double l_minus1KthNeuron_AxonOutput,
                          double l_jthNeuron_Error,
                          double NumOfTrainingSamples,
                          double LearningRate);

	  private:

	};

	typedef std::vector<Dentrite> VecDentrite;

	Neuron(enum ActivationFunc PhoType);
	~Neuron();



	/**
	 * This function simply adds a dentrite to the list of
	 * dentrites for the given Neuron.
	 * A reference to the new Dentrite is returned
	 */
	Dentrite& AddDentrite();
	
	/**
	 * Getter for Axon
	 */
	double ReadAxon();

	/**
	 * This functions job is to read the current Dentrite vals
	 * and their weights. Sum them and apply the activation to
	 * the sum, and set the Axon var
	 */
	void CalcActivation();
	
	AxonOutput GetAxonResult(); /**< Getter for Axon */


    /**
     * This function is used to set the Error for the Neuron.
     * It simply takes the Neurons "BackProp weighted sum error"
     * and then applies the selected Activation Functions derivative
     * (calc'd at the currently stored forward weighted sum) to it
     * and stores it internally.
     */
    bool SetError(double BackPropWeightedSumError);

    /**
     * This function is used to obtain a the Neurons Error value.
     * (as last computed)
     */
    double GetError();

    /**
     * This function handles the math to update the Bias guestimate.
     *
     * At this point of function call, the forward pass must be done
     * and the backprop error training pass must be done (giving us legit
     * Neuron Error Amounts for latest i/p, o/p training set)
     * You basically pass it the training set size (or window size), and
     * the scalar learning rate, to set what the delta will be by which it
     * "moves to the minimum" ... (Newton Root finding)
     * I also suspect we can do some magic at improving this delta guess
     * hence a reason this is part of the Neuron calcs.
     *
     * @param NumOfTrainingSamples The total # of training samples used
     *                             to train this neural network....or the 
     *                             window of samples to use.
     * @param LearningRate The learning rate to use to pick a small enough
     *        delta to not over jump the minimum, but large enough to learn
     *        quickly. Basically we get this # multiplied by the computed
     *        error derivative (aka, the bias derivative). Normally this #
     *        should be small so a # between 0 and 1.
     */
    bool UpdateBias(double NumOfTrainingSamples, double LearningRate);



	/**
	 * This function acts as the Synapse for a particular
	 * Dentrite connected to this neuron. Meaning it provides
	 * the mechanism for a New input value to be loaded into
	 * the Neuron.
	 *
	 * @param Dentrite The element number of the dentrite to
	 *                 update.
	 * @param NewValue The new incoming Value from the other
	 *                 connected Neuron or Raw Sensor. (Via
	 *						 the Synaptic Cleft...our mappings)
	 */ 
	void Synapse_UpdateDentriteInput(uint64_t Dentrite, 
	  		   								double NewValue);

	/**
	 * This function acts as "Neuro-plastisity" where we
     * adjust the Dentrite coefficients, thus adjusting the
	 * strength of the Axon-Dentrite pathway (changing the
	 * gain associated with the inputs that come in through
	 * this synaptic cleft/output-input mapping)
	 * 
     * @param Dentrite The element number of the dentrite 
	 *                 whose pathway you wish to adjust
	 * @param NewCoef  The new coefficient used for the
	 *                 specified dentrite.
	 */
	void Plastisity_UpdateDentriteWeight(uint64_t Dentrite,
  												    double NewCoef);
	
	NeuronId GetId(); /**< Return mNeuronId */		
	VecDentrite& GetDentritesRef(); /**< Returns a ref to
													 mDentrites */
	double mBias;
    double mWeightedInput; /**< Combined weighted input */

  private:

	VecDentrite mDentrites;
	AxonOutput mAxon;
	
	static NeuronId mzNumOfNeurons;
	NeuronId mNeuronId;


    double mError; /**< The calculated error, during backprop */
    enum ActivationFunc mPhoType;
  
//Read the # of dentrites, and each dentrite's weight. That way we can always save a Neural Network that we like.

//have another class called NeuralNetwork, where we can create
//the array of Neurons, and setup 1-1 mappings of neurons outputs to dentrite inputs. It would also handle network training where we can calc the neuron errors and update the weightings fr local minima convergence.

};

#endif
