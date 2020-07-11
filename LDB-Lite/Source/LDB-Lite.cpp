#include "LDB-Lite.h"

using namespace LDBL;

// <====================================== GENERIC FUNCTIONS ======================================>

/* A function that will be used to parse the variables in LDBL file lines. It will get a std::string and 
 * a char and will separate into two substrings, one containing all chars from the string begin to the first
 * found char passed by parameter in the string, and the other being where the iterator of the first
 * statments stopped + 1 utill the of the string. This will be mainly used at input operations.  */
std::pair<std::string, std::string> splitByChar(std::string str, char c)
{
    std::pair<std::string, std::string> splittedStr = { "", "" };

    std::size_t offset = str.find_first_of(c);

    if (offset != std::string::npos) // If find_first_of don't find the char it returns npos!
    {
        splittedStr.first = str.substr(0, offset);
        splittedStr.second = str.substr(offset + 1, str.size());
    }
    else
        splittedStr.first = str;

    return splittedStr;
}

std::vector<std::string>::const_iterator findVar(std::vector<std::string>* lines, std::string name)
{
    for (std::size_t i = 0; i < lines->size(); i++)
        if (splitByChar((*lines)[i].substr(1), VAR_CONTENT_INIT).first == name)
            return lines->begin() + i;

    return lines->begin() + lines->size();
}

void eraseVarFrom(std::vector<std::string>* lines, std::string name)
{
    std::vector<std::string>::const_iterator varPos = findVar(lines, name);

    if (varPos < lines->begin() + lines->size())
        lines->erase(varPos);
} 

void eraseVarFrom(std::vector<std::string>* lines, std::map<std::string, VarInfo>* varMap, std::string name)
{
    std::vector<std::string>::const_iterator varPos = findVar(lines, name);

    if (varPos < lines->begin() + lines->size())
    {
        lines->erase(varPos);
        varMap->erase(name);
    }
}

// <====================================== FILE/MAPPING FUNCTIONS ======================================>

std::vector<std::string> readFileLines(std::string path)
{
    if (splitByChar(path, '.').second != std::string(LDBL_FILE_EXT).substr(1))
        throw std::exception("Invalid file extension!");

    std::ifstream inputFile = std::ifstream(path);
    std::vector<std::string> lines = std::vector<std::string>();

    std::string line;
    while (std::getline(inputFile, line))
        if (!line.empty())
            lines.push_back(line);

    inputFile.close();

    return lines;
}

std::map<std::string, VarInfo> mapLines(std::vector<std::string>* lines)
{
    std::map<std::string, VarInfo> varMap = std::map<std::string, VarInfo>();
    
    for (std::string line : *lines)
    {
        std::vector<std::string> varContent = std::vector<std::string>();

        std::pair<std::string, std::string> splittedStr = splitByChar(line.substr(1), VAR_CONTENT_INIT);
        std::string varName = splittedStr.first;

        do
        {
            splittedStr = splitByChar(splittedStr.second, ARRAY_CONTENT_SEP);
            
            varContent.push_back(splittedStr.first);
        }
        while (!splittedStr.second.empty());

        varMap.emplace(varName, VarInfo(varName, varContent, varContent.size() > 1 ? VarMode::Array : VarMode::Single));
    }

    return varMap;
}

// <====================================== VarInfo DEFINITION ======================================>

VarInfo::VarInfo() 
{
    name = "";
    content = std::vector<std::string>();
    mode = VarMode::Single;
}

VarInfo::VarInfo(std::string varName, std::vector<std::string> varContent, VarMode varMode)
{
    name = varName;
    content = varContent;
    // value = varContent.front();
    mode = varMode;
}

// <====================================== INPUT METHODS ======================================>

std::vector<std::string> In::listVars(std::vector<std::string>* lines)
{
    std::vector<std::string> varNames = std::vector<std::string>();
    for (std::string line :* lines)
        varNames.push_back(splitByChar(line.substr(1), VAR_CONTENT_INIT).first);

    return varNames;
}

std::vector<std::string> In::listVars(std::string path)
{
    std::vector<std::string> lines = readFileLines(path);

    return listVars(&lines);
}

VarInfo In::getVar(std::vector<std::string>* lines, std::string name) { return mapLines(lines)[name]; }

VarInfo In::getVar(std::string path, std::string name)
{
    std::vector<std::string> lines = readFileLines(path);

    return In::getVar(&lines, name);
}

// std::string In::getSingle(std::string path, std::string name) { return In::getVar(path, name).content.front(); }

// std::vector<std::string> In::getArray(std::string path, std::string name) { return In::getVar(path, name).content; }

// <====================================== OUTPUT METHODS ======================================>

// This method will set an array or a single, based on VarInfo!
void Out::setVar(std::vector<std::string>* lines, VarInfo info, bool overwrite)
{ 
    std::string line = std::string();
    
    switch (info.mode)
    {
    case VarMode::Single: 
        line += SINGLE_DEC_PFX; break; 

    case VarMode::Array: 
        line += ARRAY_DEC_PFX; break;
    }

    line += info.name + VAR_CONTENT_INIT;

    for (std::size_t i = 0; i < info.content.size(); i++)
    {
        if (i > 0)
            line += ARRAY_CONTENT_SEP;

        line += info.content[i];
    }

    if (overwrite)
    {
        std::vector<std::string>::const_iterator varPos = findVar(lines, info.name);
        
        lines->insert(varPos == lines->begin() + lines->size() ? lines->end() : lines->erase(varPos), line);
    }
    else
        lines->push_back(line);
}

// This method will set an array or a single, based on VarInfo!
void Out::setVar(std::string path, VarInfo info, bool overwrite)
{
    std::ofstream outputFile = std::ofstream(path);
    std::vector<std::string> lines = readFileLines(path);

    Out::setVar(&lines, info, overwrite);

    for (std::string line : lines)
        outputFile << line << '\n';

    outputFile.close();
}

// This method will set a single value variable!
void Out::setVar(std::string path, std::string name, std::string value, bool overwrite)
{
    Out::setVar(path, VarInfo(name, std::vector<std::string>{value}), overwrite);
}

// This method will set a multi-value array variable!
void Out::setVar(std::string path, std::string name, std::vector<std::string> content, bool overwrite)
{
    Out::setVar(path, VarInfo(name, content, VarMode::Array), overwrite);
}

/*
void Out::setSingle(std::string path, std::string name, std::string value, bool overwrite)
{
    Out::setVar(path, VarInfo(name, std::vector<std::string>{value}), overwrite);
}

void Out::setArray(std::string path, std::string name, std::vector<std::string> content, bool overwrite) 
{
    Out::setVar(path, VarInfo(name, content, VarMode::Array), overwrite);
}
*/

// This method will erase the line which contains the variable!
void Out::eraseVar(std::vector<std::string>* lines, std::string name) { eraseVarFrom(lines, name); }

// This method will erase the line which contains the variable!
void Out::eraseVar(std::string path, std::string name)
{
    std::ofstream outputFile = std::ofstream(path);
    std::vector<std::string> lines = readFileLines(path);

    Out::eraseVar(&lines, name);

    for (std::string line : lines)
        outputFile << line << '\n';

    outputFile.close();
}

// <====================================== DOC CLASS DEFINITION ======================================>

Doc::Doc(std::string path)
{   
    lines = new std::vector<std::string>{ readFileLines(path) };
    varMap = new std::map<std::string, VarInfo>{ mapLines(lines) };

    outFileStream.open(path);
} 

Doc::~Doc()
{
    for (std::string line : *lines)
        outFileStream << line << '\n';
    
    delete lines;
    delete varMap;

    outFileStream.close();
}

std::vector<std::string> Doc::listVars() { return In::listVars(lines); }

VarInfo Doc::getVar(std::string name) { return In::getVar(lines, name); }

// std::string Doc::getSingle(std::string name) { return In::getVar(lines, name).content.front(); }

// std::vector<std::string> Doc::getArray(std::string name) { return In::getVar(lines, name).content; }

// This method will set an array or a single, based on VarInfo!
void Doc::setVar(VarInfo info, bool overwrite)
{
    Out::setVar(lines, info, overwrite);
    
    varMap->emplace(info.name, info);
}

// This method will set a single value variable!
void Doc::setVar(std::string name, std::string value, bool overwrite)
{
    VarInfo info = VarInfo(name, std::vector<std::string>{value});

    Out::setVar(lines, info, overwrite);

    varMap->emplace(info.name, info);
}

// This method will set a multi-value array variable!
void Doc::setVar(std::string name, std::vector<std::string> content, bool overwrite)
{
    VarInfo info = VarInfo(name, content, VarMode::Array);

    Out::setVar(lines, info, overwrite);

    varMap->emplace(info.name, info);
}

/*
void Doc::setSingle(std::string name, std::string value, bool overwrite)
{
    VarInfo info(name, std::vector<std::string>{value});

    Out::setVar(lines, info, overwrite);
    
    varMap->emplace(info.name, info);
}

void Doc::setArray(std::string name, std::vector<std::string> content, bool overwrite)
{
    VarInfo info(name, content, VarMode::Array);

    Out::setVar(lines, info, overwrite);

    varMap->emplace(info.name, info);
}
*/

void Doc::eraseVar(std::string name) { eraseVarFrom(lines, varMap, name); }
