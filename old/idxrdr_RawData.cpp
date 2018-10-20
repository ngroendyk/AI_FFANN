/*****************************************************************************
 * File: idxrdr_RawData.cpp
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Interface class to load different potential data types stored with
 * idx.
 ****************************************************************************/

 #include "SysTools.h"
 #include "idxrdr_RawData.h"

 #include <stdint.h>
 #include <iostream>
 #include <boost/shared_ptr.hpp> 



idxrdr_RawData::idxrdr_RawData(std::vector<uint8_t>& Uint8DataVec,
                               std::vector<int8_t>& Int8DataVec,
                               std::vector<int16_t>& Int16DataVec,
                               std::vector<int32_t>& Int32DataVec,
                               std::vector<float>& Float32DataVec,
                               std::vector<double>& Double32DataVec) :
    mUint8DataVec(Uint8DataVec),
    mInt8DataVec(Int8DataVec),
    mInt16DataVec(Int16DataVec),
    mInt32DataVec(Int32DataVec),
    mFloat32DataVec(Float32DataVec),
    mDouble32DataVec(Double32DataVec)
{
}




void idxrdr_RawData::Create(enum idxrdr_RawData::enDataType DatType, 
                            idxrdr_RawDataPtr& pRawData)
{
    if (pRawData)
    {
        std::cout << " Create() - Data Already in ptr. Cant recreate, free"
                  << "first." << std::endl;
        return;
    }

    //figure out the data type, and create the object
    switch (DatType)
    {
      case idxrdr_RawData::eUint8:
        //pRawData.reset(new idx_DataHandlr<uint8_t>());
        std::cout << "Uint8";
        break;
      case idxrdr_RawData::eInt8:
        std::cout << "Int8";
        break;
      case idxrdr_RawData::eInt16:
        std::cout << "Int16";
        break;
      case idxrdr_RawData::eInt32:
        std::cout << "Int32";
        break;
      case idxrdr_RawData::eFloat32:
        std::cout << "Float32";
        break;
      case idxrdr_RawData::eDouble64:
        std::cout << "Double64";
        break;
      case idxrdr_RawData::eUnknown:
        std::cout << "Unknown";
    }; //end switch
 

}


idxrdr_RawData::~idxrdr_RawData()
{
}

idxrdr_RawData::idxrdr_RawData()
{
}


