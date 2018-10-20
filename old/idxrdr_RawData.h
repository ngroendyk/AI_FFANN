/*****************************************************************************
 * File: idxrdr_RawData.h
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Interface class to load different potential data types stored with
 * idx.
 *
 * This is the definition file
 ****************************************************************************/
#ifndef IDX_RDR_RAWDATA_H
#define IDX_RDR_RAWDATA_H

 #include "SysTools.h"
 #include "idxrdr.h"

 #include <stdint.h>
 #include <iostream>
 #include <boost/shared_ptr.hpp> 
 

//globals
class idxrdr_RawData;
typedef std::shared_ptr< idxrdr_RawData > idxrdr_RawDataPtr; 

class idxrdr_RawData
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


    idxrdr_RawData(enum enDataType DatType, idxrdr& DataHolder);


std::vector<uint8_t>& Uint8DataVec,
                   std::vector<int8_t>& Int8DataVec,
                   std::vector<int16_t>& Int16DataVec,
                   std::vector<int32_t>& Int32DataVec,
                   std::vector<float>& Float32DataVec,
                   std::vector<double>& Double32DataVec);
 :
    mUint8DataVec(Uint8DataVec),
    mInt8DataVec(Int8DataVec),
    mInt16DataVec(Int16DataVec),
    mInt32DataVec(Int32DataVec),
    mFloat32DataVec(Float32DataVec),
    mDouble32DataVec(Double32DataVec)
    {
    }





    ~idxrdr_RawData(); 

    void static Create(idxrdr_RawData::enDataType DatType, 
                       idxrdr_RawDataPtr& pRawData);

  private:

    //References to the users data vars
    std::vector<uint8_t>& mUint8DataVec;
    std::vector<int8_t>& mInt8DataVec;
    std::vector<int16_t>& mInt16DataVec;
    std::vector<int32_t>& mInt32DataVec;
    std::vector<float>& mFloat32DataVec;
    std::vector<double>& mDouble32DataVec;


    

};



#endif
