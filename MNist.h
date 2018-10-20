/*****************************************************************************
 * File: MNist.h
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Implement the MNIST dataset to load the MNIST data into an object
 * This is the definition file
 ****************************************************************************/
#ifndef MNIST_H
#define MNIST_H
 
 #include "SysTools.h"
 #include "idxrdr.h"
 #include <vector>
 #include <fstream>
 #include <stdint.h>
 #include <iostream>

//globals

class MNist
{
  public:
    MNist(const std::string& AnswerFile, const std::string& ImageFile);
    ~MNist();

    bool GetImage(uint32_t Offset, 
                  std::vector< std::vector< double > >& RetImage);

    bool GetAnswer(uint32_t Offset, std::vector< double >& RetAnswer);

    bool GetImageAnswerSet(uint32_t Offset, 
                           std::vector< std::vector< double > >& RetImage,
                           std::vector< double >& RetAnswer);

    int8_t GetAnswer(uint32_t Offset);

    void DebugPrintImage(uint32_t Offset); /**< Ascii art this beast */

  private:

    idx_rdr mImagesRawData;
    idx_rdr mAnswersRawData;
  
};

#endif

