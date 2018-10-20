/*****************************************************************************
 * File: idxrdr.h
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Implement the idx format so we can read the MNIST data set
 * This is the definition file
 ****************************************************************************/

#ifndef IDXRDR_H
#define IDXRDR_H
 
 #include <stdint.h>
 #include <iostream>
 #include "SysTools.h"
 #include <vector>
 #include <fstream>

//globals

class idx_rdr
{
  public:

    //Supported data types of the idx format
    enum enDataType:int16_t
    {
        eUnknown = 0,
        eUint8,
        eInt8,
        eInt16,
        eInt32,
        eFloat32,
        eDouble64
    };


    idx_rdr(const std::string& RawDataFileName); /**< Pass the raw data file to 
                                                      load*/
    ~idx_rdr();
    void DebugPrint(); /**< Tell us stuff about this idx object */

    bool GetData(std::vector< uint8_t >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< int8_t >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< int16_t >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< int32_t >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< int64_t >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< float >& RetData); /**<Return actual Data Vec*/
    bool GetData(std::vector< double >& RetData); /**<Return actual Data Vec*/









  private:

    bool LoadNextValue(std::ifstream & Idxfh); /**<Simply loads the next data 
                                                   type */

    uint32_t mMagicNumber;
    uint32_t mNumberOfItems;
    enum enDataType mDataType;
    bool mIs1D; /**< if set, then 1D vector, if not set, then 2D matrix */

    //Return data types. We use the one that the data is, and have overloads
    //for the others.
    std::vector<uint8_t> mUint8DataVec;
    std::vector<int8_t> mInt8DataVec;
    std::vector<int16_t> mInt16DataVec;
    std::vector<int32_t> mInt32DataVec;
    std::vector<float> mFloat32DataVec;
    std::vector<double> mDouble64DataVec;

    uint32_t mNumOfRows;
    uint32_t mNumOfCols;

};

#endif

