/*****************************************************************************
 * File: SysTools.h
 * Date: 24 July 2018
 * Written By: Nate Groendyk
 * Purpose: Do some basic binary stuff to things, and system detect stuff.
 * We are the definition file.
 ****************************************************************************/
#ifndef SYSTOOLS_H
#define SYSTOOLS_H


#include <stdint.h>
#include <cstring>
#include <fstream>
#include <vector>

namespace SysTools
{

enum Endian
{
    eBigEndian = 0,
    eLittleEndian
};

/**
 * Big Endian and Little Endian. ... Big Endian Byte Order: The most significant
 * byte (the "big end") of the data is placed at the byte with the lowest
 * address. The rest of the data is placed in order in the next three bytes in 
 * memory.
 */
bool IsBigEndian(); /**< Returns if Big Endian */
bool IsLittleEndian(); /**< Returns if Little Endian */
enum Endian EndianType(); /**< Returns system Endian */
void PrintEndian(); /**< Prints out the Endian-ness of the machine */

/**
 * This function converts a number passed in to the correct form
 * to match proper endian-ness.  Essentially it flips the bytes if it
 * detects it needs to be flipped. THIS FUNCTION IS DANGEROUS if the data size
 * you give it (via the NumType) doesnt match the width of the underlying data
 * you are copying from.... So be careful. 
 *
 * @param NumType the template type of number to be converted
 *                and returned
 * @param pNumber Pointer to the data that needs to endianized and returned as a
 *                number.
 * @param Endianess Specify the endianess of the data you are passing in.
 */
template <typename NumType>
NumType ToMachineEndianess(void * pNumber, enum Endian Endianess)
{
    uint16_t sizeType = sizeof(NumType);
    NumType RetNum;
    unsigned char * pcIterNumber = static_cast<unsigned char *>(pNumber);
    unsigned char * pcIterRetVal = reinterpret_cast<unsigned char *>(&RetNum);

    if ((IsBigEndian() && Endianess != eBigEndian) ||
        (IsLittleEndian() && Endianess != eLittleEndian))
    {
        //this means we need to flip the bytes
        for (uint16_t i = 0; i < sizeType; i++)
        {
            pcIterRetVal[sizeType - i - 1] = pcIterNumber[i]; 
        }  
    }
    else
    {
        //this means we dont need to flip the bytes
        memcpy(&RetNum, pNumber, sizeType);
    }

    return RetNum; 
}


template <typename NumT>
void From2Dto1D(std::vector< std::vector< NumT > >& Vect2D,
                std::vector< NumT >& Vect1D)
{

    for (uint32_t r = 0;r < Vect2D.size(); ++r) //iterate the rows
    {
        for (uint32_t c = 0; c < Vect2D[r].size(); ++c)
        {
            Vect1D.push_back(Vect2D[r][c]);
        } //end c
    } //end r
    return;
}


class FileGuard
{
  public:

    FileGuard(std::ifstream& FileToGuard);
    ~FileGuard();

 private:
   std::ifstream& mFileToGuard;

};


template <typename NumT>
int64_t max(std::vector < NumT >& Vec)
{
    uint64_t i;
    uint64_t MaxEle;
    NumT MaxVal;

    for (i = 0; i < Vec.size(); ++i)
    {
        if (!i)
        { //First it
            MaxVal = Vec[i];
            MaxEle = i;
            continue;
        }

        if (Vec[i] > MaxVal)
        {
            MaxVal = Vec[i];
            MaxEle = i;
        }
    }

    return MaxEle;

}


};

#endif
