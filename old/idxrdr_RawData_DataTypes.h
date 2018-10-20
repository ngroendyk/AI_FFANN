/*****************************************************************************
 * File: idxrdr_RawData_DataTypes.h
 * Written By: Nate Groendyk
 * Date: 24 July 2018
 * 
 * Purpose: Template Implementation class to Polymorph out data from idx format
 * files.
 ****************************************************************************/

#ifndef IDX_RDR_RAWDATA_DATATYPES_H
#define IDX_RDR_RAWDATA_DATATYPES_H

 #include "SysTools.h"
 #include "idxrdr.h"
 #include "idxrdr_RawData.h"

 #include <stdint.h>
 #include <iostream>
 #include <boost/shared_ptr.hpp> 
 #include <vector>
 #include <fstream>


/**
 * This template class does the heavy lifting of handling RawData from the idx
 * formatted file. The idea is to polymorph this class, so we dont have to type
 * tons of the same functions.
 */

template <class DType>
class idx_DataHandlr : idxrdr_RawData

{
    idx_DataHandlr(){};
    ~idx_DataHandler(){};

    bool LoadNextValue(std::vector<DType>& DataVect, ifstream& OpenedDataFile)
    {
        char TempBuf[sizeof(DType)] = {0};


        IdxFile.read(TempBuf,sizeof(uint32_t));
        if (!IdxFile)
        {
            std::cout << "idx_rdr: Error reading file." << std::endl;
            //error!
            return;
        }
        mMagicNumber = SysTools::ToMachineEndianess<uint32_t>(&TempBuf[0],
            SysTools::eBigEndian);

        return true;
    }
};


