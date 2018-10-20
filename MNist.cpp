/*****************************************************************************
 * File: MNist.cpp
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Implement the MNIST dataset to load the MNIST data into an object
 ****************************************************************************/
 #include "idxrdr.h"
 #include "SysTools.h"
 #include "MNist.h"

 #include <stdint.h>
 #include <iostream>
 #include <fstream>
 #include <assert.h>


MNist::MNist(const std::string& AnswerFile, const std::string& ImageFile):
    mImagesRawData(ImageFile),
    mAnswersRawData(AnswerFile)
{
}
 
MNist::~MNist()
{
}

bool MNist::GetImage(uint32_t Offset,
                     std::vector< std::vector< double > >& RetImage)
{
    if ((RetImage.size() != 28) || (RetImage[0].size() != 28))
    {
        std::cout << "Error in MNist::GetImage() - MAtrix must be 28 X 28"
                  << std::endl;
        return false;
    }

    std::vector< uint8_t > RawData;
    if (!mImagesRawData.GetData(RawData))
    {
        std::cout << "Error in MNist::GetImage() - Bad Image vector from idxrdr"
                  << std::endl;
        return false;
    }

    for (uint32_t r = 0; r < 28; ++r)
    {
        for (uint32_t c = 0; c < 28; ++c)
        {
            RetImage[r][c] = static_cast < double >
                (RawData[(Offset*28*28)+((r*28)+c)]);
        }
    }

    return true;
}

bool MNist::GetAnswer(uint32_t Offset, std::vector< double >& RetAnswer)
{
    std::vector < uint8_t > RawData;

    if (!(mAnswersRawData.GetData(RawData)))
    {
        std::cout << "Error im MNist::GetAnswer() - Bad Answer Vector from"
                  << " idxrdr. " << std::endl;
        return false;
    }

    if (RetAnswer.size() != 10)
    {
        std::cout << "Error im MNist::GetAnswer() - Bad passedin RetAnswer Vec."
                  << "Its size is: " << RetAnswer.size() << " and we need 10. "
                  << std::endl;
        return false;
    }

    for (uint16_t i = 0; i < 10; i++)
    {
        RetAnswer[i] = 0.0;
    }

    RetAnswer[RawData[Offset]] = 1.0;
 
    return true;
}


int8_t MNist::GetAnswer(uint32_t Offset)
{
    std::vector < uint8_t > RawData;
    if (!(mAnswersRawData.GetData(RawData)))
    {
        std::cout << "Error im MNist::GetAnswer() - Bad Answer Vector from"
                  << " idxrdr. " << std::endl;
        return -1;
    }
   
    return RawData[Offset];
}

bool MNist::GetImageAnswerSet(uint32_t Offset, 
                       std::vector< std::vector< double > >& RetImage,
                       std::vector< double >& RetAnswer)
{
    return true;
}


void MNist::DebugPrintImage(uint32_t Offset)
{

    //Get the Data
    std::vector< std::vector< double > > Im(28, std::vector< double >(28));
    GetImage(Offset, Im);

    //Iterate through the Image, deebug print

    for (uint16_t r = 0; r < 28; ++r)
    {
        for (uint16_t c = 0; c < 28; ++c)
        {
            if (Im[r][c]  <= 10.0)
            {
                std::cout << "  ";
            }
            else
            {
                std::cout << "1 ";
            }
        }
        std::cout << std::endl;
    }

    return;
}


