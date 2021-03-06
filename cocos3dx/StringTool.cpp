/****************************************************************************
Copyright (c) Chukong Technologies Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "StringTool.h"

namespace cocos3d
{
StringTool::StringTool()
{
}

StringTool::~StringTool()
{
}

std::vector<std::string> StringTool::StringSplitByString(const std::string &str, const std::string &strKey, int IgnoreCase /* = false */)
{
    std::vector<std::string> Result;
    const char *pLastPos = NULL;
    char *pStart = new char[str.size() + 1];

    strncpy(pStart, str.c_str(), str.size() + 1);

    Result.clear();

    pLastPos = NULL;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        int nCompareResult = 0;

       /* if (IgnoreCase)
        {
            nCompareResult = strnicmp(pStart + i, strKey.c_str(), strKey.size());
        }
        else*/
        {
			nCompareResult = strncmp(pStart + i, strKey.c_str(), strKey.size());
        }

        if (!nCompareResult)
        {
            pStart[i] = '\0';
            if (pLastPos)
            {
                Result.push_back(std::string(pLastPos) );
                pLastPos = NULL;
            }
            i += strKey.size() - 1;
        }
        else
        {
            if (!pLastPos) // 一个新字符串的开始
            {
                pLastPos = pStart + i;
            }
        }
    }
    if (pLastPos)
    {
        Result.push_back(std::string(pLastPos) );
        pLastPos = NULL;
    }

    delete []pStart;
    return Result;
}

std::vector<std::string> StringTool::StringSplitByChar(const std::string &str, char cKey, int IgnoreCase /* = false */)
{
    std::vector<std::string> Result;
    
    const char *pLastPos = NULL;
    char *pStart = new char[str.size() + 1];

    strncpy(pStart, str.c_str(), str.size() + 1);

    Result.clear();

    pLastPos = NULL;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        int nCompareResult = -1;

        if (IgnoreCase)
        {
            nCompareResult = toupper(pStart[i]) - toupper(cKey);
        }
        else
        {
            nCompareResult = pStart[i] - cKey;
        }

        if (!nCompareResult)
        {
            pStart[i] = '\0';
            if (pLastPos)
            {
                Result.push_back(std::string(pLastPos) );
                pLastPos = NULL;
            }
        }
        else
        {
            if (!pLastPos) // 一个新字符串的开始
            {
                pLastPos = pStart + i;
            }
        }
    }
    if (pLastPos)
    {
        Result.push_back(std::string(pLastPos) );
        pLastPos = NULL;
    }

    delete []pStart;
    return Result;
}

std::string StringTool::getFileName(const std::string& filepath)
{
    size_t index1 = filepath.find_last_of('\\');
    size_t index2 = filepath.find_last_of('/');
    size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
	size_t length = filepath.length();
    std::string output = filepath.substr(index + 1, length);

    return output;
}

std::string StringTool::getFileName(const std::string& filepath,const std::string& expName)
{
    size_t index1 = filepath.find_last_of('\\');
    size_t index2 = filepath.find_last_of('/');
    size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
	size_t length = filepath.length();
    std::string filename = filepath.substr(index + 1, length);
    length = filename.length();

    std::string output = filename.substr(0, (length-expName.length()-1));
    return output;
}

std::string StringTool::getFilePath(const std::string& filename)
{
    int index1 = filename.find_last_of('\\');
    int index2 = filename.find_last_of('/');
    int index = (index1 != -1 && index1 > index2 ? index1 : index2);
    std::string filepath = filename.substr(0,index+1);

    return filepath;
}

std::string StringTool::toString(bool b)
{
    return b ? "true" : "false";
}

void StringTool::fromString(const std::string &str, bool& b)
{
    b = str == "true";
}

}