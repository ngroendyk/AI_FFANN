#!/bin/bash

EXEC="NeuralNetTest.exc"
if [ -e "$EXEC" ]; then
	echo "Deleting $EXEC"
	rm "$EXEC"
fi

echo "Building $EXEC"

#g++ -std=c++11 -I/usr/include/boost/mpl -I/usr/include/boost/typeof/std FeedNeuralNet.cpp Neuron.cpp -o NeuralNet.exc

g++ -std=c++11 -ggdb -Wall -I/usr/include FeedForwardNeuralNet.cpp Neuron.cpp NeuralNetTest.cpp StringTools.cpp SysTools.cpp idxrdr.cpp MNist.cpp -o "$EXEC"
