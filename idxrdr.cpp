/*****************************************************************************
 * File: idxrdr.cpp
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Implement the idx format so we can read the MNIST data set
 ****************************************************************************/
 #include "idxrdr.h"
 #include <stdint.h>
 #include <iostream>
 #include <fstream>
 #include "SysTools.h"
 #include <assert.h>

using namespace std;
//globals

idx_rdr::idx_rdr(const std::string& RawDataFileName) :
    mMagicNumber(),
    mDataType(),
    mIs1D(),
    mUint8DataVec(),
    mInt8DataVec(),
    mInt16DataVec(),
    mInt32DataVec(),
    mFloat32DataVec(),
    mDouble64DataVec(),
    mNumOfRows(),
    mNumOfCols()
{
    char TempBuf[1024];
    
    //Open the file for binary access
    ifstream IdxFile(RawDataFileName, ios::in | ios::binary);
    if (!IdxFile)
    {
        std::cout << "idx_rdr: Error opening file." << std::endl;
        //error!
        return;
    }

    SysTools::FileGuard IdxGaurd(IdxFile); //make sure we close it.

    //Try and read the Magic Number
    memset(TempBuf, '\0', sizeof(TempBuf));
    IdxFile.read(TempBuf, sizeof(uint32_t));
    if (!IdxFile)
    {
        std::cout << "idx_rdr: Error reading file." << std::endl;
        //error!
        return;
    }
    mMagicNumber = SysTools::ToMachineEndianess<uint32_t>(&TempBuf[0],
        SysTools::eBigEndian);

    //Rip down the Magic number.
    uint32_t MagNumMask = 0xffff0000;
    if (mMagicNumber & MagNumMask)
    {
        std::cout << "idx_rdr: MagicNumber error. MSB 2 bytes are not zero." 
            << std::endl;
        return;
    }

    MagNumMask=0xffff00ff;
    switch ((mMagicNumber & ~MagNumMask) >> 8)
    {
      case 0x08:
        mDataType = eUint8;
        break;
      case 0x09:
        mDataType = eInt8;
        break;
      case 0x0B:
        mDataType = eInt16;
        break;
      case 0x0C:
        mDataType = eInt32;
        break;
      case 0x0D:
        mDataType = eFloat32;
        break;
      case 0x0E:
        mDataType = eDouble64;
        break;
      default:
        mDataType = eUnknown;
    }; //end switch

    MagNumMask=0x000000ff;
    if ((MagNumMask & mMagicNumber) == 1)
    {
        mIs1D = true;
    }
    else if ((MagNumMask & mMagicNumber) == 3)
    {
        mIs1D = false;
    }
    else
    {
        std::cout << "Unknown 1D or 2D. Assuming 1D. Byte is: "
                  << (MagNumMask & mMagicNumber) << std::endl;
        mIs1D = true;
    }

    //Try reading # of items.
    memset(TempBuf, '\0', sizeof(TempBuf));
    IdxFile.read(TempBuf, sizeof(uint32_t));
    if (!IdxFile)
    {
        std::cout << "idx_rdr: Error reading file (# of items)." << std::endl;
        //error!
        return;
    }
    mNumberOfItems = SysTools::ToMachineEndianess<uint32_t>(&TempBuf[0],
        SysTools::eBigEndian);

    //Now try and get the items. If 1D this just is the list, if 2D theres
    //more to the format
    uint32_t i = 0;
    if (mIs1D)
    {
        //means we just have a big old wack of numbers, lets keep loading them.
        for (; i < mNumberOfItems; i++)
        {
            if (!LoadNextValue(IdxFile))
            {
                if (i < mNumberOfItems - 1)
                {
                    std::cout << "idx_rdr: File had less items then "
                              << "reported. Reported: " << mNumberOfItems 
                              << "Items loaded in file: " << i << std::endl;
                }
                break;
            }
        }
    }
    else
    {
        //read the # of rows and # of columns

        //Try reading # of rows
        memset(TempBuf, '\0', sizeof(TempBuf));
        IdxFile.read(TempBuf, sizeof(uint32_t));
        if (!IdxFile)
        {
            std::cout << "idx_rdr: Error reading file (# of Rows)." 
                      << std::endl;
            //error!
            return;
        }
        mNumOfRows = SysTools::ToMachineEndianess< uint32_t >(&TempBuf[0],
            SysTools::eBigEndian);

        //Try reading # of cols
        memset(TempBuf, '\0', sizeof(TempBuf));
        IdxFile.read(TempBuf, sizeof(uint32_t));
        if (!IdxFile)
        {
            std::cout << "idx_rdr: Error reading file (# of Rows)." 
                      << std::endl;
            //error!
            return;
        }
        mNumOfCols = SysTools::ToMachineEndianess< uint32_t >(&TempBuf[0],
            SysTools::eBigEndian);
 
        //means we just have a big old wack of numbers, lets keep loading them.
        for (i = 0; i < mNumberOfItems; i++)
        {
            for (uint32_t r = 0; r < mNumOfRows; ++r)
            {
                for (uint32_t c = 0; c < mNumOfCols; ++c)
                {
                    if (!LoadNextValue(IdxFile))
                    {
                        if (i < mNumberOfItems - 1)
                        {
                            std::cout << "idx_rdr: File had less items then "
                                      << "reported. Reported: " 
                                      << mNumberOfItems 
                                      << "Items loaded in file: " 
                                      << i << std::endl;
                        }
                        break;
                    }
                } 
            }
        } //iterate through the # of matricies
    } 
}


idx_rdr::~idx_rdr()
{
}

bool idx_rdr::LoadNextValue(std::ifstream & Idxfh)
{
    char TempBuf[ (sizeof(double) >= sizeof(uint64_t)) ? sizeof(double):
        sizeof(uint64_t)];
    int16_t ItemSize = 0;

    //Try to read the next value.
    memset(TempBuf, '\0', sizeof(TempBuf));

    //Read data. But what are we. 
    switch (mDataType)
    {
      case eUint8:
        ItemSize = sizeof(uint8_t);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eInt8:
        ItemSize = sizeof(int8_t);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eInt16:
        ItemSize = sizeof(int16_t);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eInt32:
        ItemSize = sizeof(int32_t);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eFloat32:
        ItemSize = sizeof(float);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eDouble64:
        ItemSize = sizeof(double);
        Idxfh.read(TempBuf, ItemSize);
        break;
      case eUnknown:
        std::cout << "Unknown" << std::endl;
        return false;
        break;
    }; //end switch
 
    if (!Idxfh)
    {
        //Couldn't read data. Return, maybe end of file.
        return false;
    }

    uint8_t Item_uint8 = 0;
    int8_t Item_int8 = 0;
    int16_t Item_int16 = 0;    
    int32_t Item_int32 = 0L;
    float Item_float = 0.0f;
    double Item_double= 0.0;

    //Ensure endianness and push into return vectors.
    switch (mDataType)
    {
      case eUint8:
        Item_uint8 = SysTools::ToMachineEndianess< uint8_t >(
            TempBuf, SysTools::eBigEndian);
        mUint8DataVec.push_back(Item_uint8);
        break;
      case eInt8:
        Item_int8 = SysTools::ToMachineEndianess< int8_t >(
            TempBuf, SysTools::eBigEndian);
        mInt8DataVec.push_back(Item_int8);
        break;
      case eInt16:
        Item_int16 = SysTools::ToMachineEndianess< int16_t >(
            TempBuf, SysTools::eBigEndian);
        mInt16DataVec.push_back(Item_int16);
        break;
      case eInt32:
        Item_int32 = SysTools::ToMachineEndianess< int32_t >(
            TempBuf, SysTools::eBigEndian);
        mInt32DataVec.push_back(Item_int32);
        break;
      case eFloat32:
        Item_float = SysTools::ToMachineEndianess< float >(
            TempBuf, SysTools::eBigEndian);
        mFloat32DataVec.push_back(Item_float);
        break;
      case eDouble64:
        Item_double = SysTools::ToMachineEndianess< double >(
            TempBuf, SysTools::eBigEndian);
        mDouble64DataVec.push_back(Item_double);
        break;
      case eUnknown:
        std::cout << "Unknown" << std::endl;
        assert(0); //should have failed already
        return false;
        break;
    }; //end switch
    return true;
}

void idx_rdr::DebugPrint()
{
    std::cout << "Magic Number: " << mMagicNumber << std::endl;
    std::cout << "  DataType Supported: ";
    switch (mDataType)
    {
      case eUint8:
        std::cout << "Uint8";
        break;
      case eInt8:
        std::cout << "Int8";
        break;
      case eInt16:
        std::cout << "Int16";
        break;
      case eInt32:
        std::cout << "Int32";
        break;
      case eFloat32:
        std::cout << "Float32";
        break;
      case eDouble64:
        std::cout << "Double64";
        break;
      case eUnknown:
        std::cout << "Unknown";
        break;
    }; //end switch
    std::cout << std::endl;
    std::cout << "Number of Items: " << mNumberOfItems << std::endl;
    std::cout << "Vector or Matrix: ";
    if (mIs1D)
    {
        std::cout << "1D Vector" << std::endl;
    }
    else
    {
        std::cout << "2D Matrix" << std::endl;
        std::cout << "# of Rows: " << mNumOfRows << std::endl;
        std::cout << "# of Cols: " << mNumOfCols << std::endl;
    }
}

bool idx_rdr::GetData(std::vector< uint8_t >& RetData)
{
    if (mDataType == eUint8)
    {
        RetData = mUint8DataVec;
        return true;
    }
    return false;
}

bool idx_rdr::GetData(std::vector< int8_t >& RetData)
{
    if (mDataType == eInt8)
    {
        RetData = mInt8DataVec;
        return true;
    }
    return false;
}

bool idx_rdr::GetData(std::vector< int16_t >& RetData)
{
    if (mDataType == eInt16)
    {
        RetData = mInt16DataVec;
        return true;
    }
    return false;
}

bool idx_rdr::GetData(std::vector< int32_t >& RetData)
{
    if (mDataType == eInt32)
    {
        RetData = mInt32DataVec;
        return true;
    }
    return false;
}

bool idx_rdr::GetData(std::vector< float >& RetData)
{
    if (mDataType == eFloat32)
    {
        RetData = mFloat32DataVec;
        return true;
    }
    return false;
}

bool idx_rdr::GetData(std::vector< double >& RetData)
{
    if (mDataType == eDouble64)
    {
        RetData = mDouble64DataVec;
        return true;
    }
    return false;
}

