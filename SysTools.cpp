/*****************************************************************************
 * File: SysTools.cpp
 * Date: 24 July 2018
 * Written By: Nate Groendyk
 * Purpose: Do some basic binary stuff to things, and system detect stuff.
 ****************************************************************************/

#include "SysTools.h"
#include <iostream>
#include <fstream>

bool SysTools::IsBigEndian()
{
    union 
    {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

bool SysTools::IsLittleEndian()
{
    return !IsBigEndian();
}

enum SysTools::Endian SysTools::EndianType()
{
    if (IsLittleEndian())
    {
        return eLittleEndian;
    }
    else
    {
        return eBigEndian;
    }
}

void SysTools::PrintEndian()
{
    if (IsBigEndian())
    {
        std::cout << "This machine is Big Endian." << std::endl;
    }
    else
    {
        std::cout <<"This machine is Little Endian." << std::endl;
    }
}

SysTools::FileGuard::FileGuard(std::ifstream& FileToGuard) :
    mFileToGuard(FileToGuard)
{
}

SysTools::FileGuard::~FileGuard()
{
    if (mFileToGuard.is_open())
    {
        mFileToGuard.close();
    }
}


