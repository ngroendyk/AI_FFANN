/*****************************************************************************
 * File: StringTools.h
 * Written By: Nate Groendyk
 * Date: 2 June 2018
 * 
 * Purpose: Implement a series of commonly used string tools.
 ****************************************************************************/
#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H 

#include <stdint.h>
#include <vector>
#include <string>

namespace StringTools
{


/**
 * This function splits strings into substring vector delimitted by token
 *
 * @param StringToParse The string you will be ripping apart.
 * @param Token the delimitter you'll be using to seperate returned entries
 * @return A vector<string> that contains the parsed results.
 */
std::vector<std::string> SplitToVector(std::string StringToParse, 
                                        std::string Token);
};

#endif

