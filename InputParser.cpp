#include <iostream>
#include <sstream>
#include <vector>
#include "InputParser.hpp"

std::shared_ptr<Command> CommandParser::getCommand(std::string const & line) {
    if(line.size() < 3) {
    	return std::shared_ptr<Command>(NULL);
    }

    std::string cmdSymbol = line.substr(0, 1);

    std::string paramLine;
    std::string space = line.substr(1, 1);
    if(space != " ") {
    	paramLine = line.substr(1, line.size()-1);
    }
    else {
    	paramLine = line.substr(2, line.size()-2);
    }

    if(cmdSymbol == "V") {
    	std::shared_ptr<Command> pCmd = getVCommand(paramLine);
    	if(pCmd.get() != NULL) {
            std::cout << line << std::endl;
    	}

        return pCmd;
    }
    else if(cmdSymbol == "E") {
    	std::shared_ptr<Command> pCmd = getECommand(paramLine);
    	if(pCmd.get() != NULL) {
            std::cout << line << std::endl;
    	}
    	return pCmd;
    }
    else if(cmdSymbol == "s") {
    	return getSCommand(paramLine);
    }
    else {
    	return std::shared_ptr<Command>(NULL);
    }
}

//V 15
std::shared_ptr<Command> CommandParser::getVCommand(std::string const & paramLine) {
	if(paramLine.find("-") != std::string::npos) {
		return std::shared_ptr<Command>(NULL);
	}

	std::istringstream input(paramLine);
	unsigned int num = 0;
	input >> num;
    if (input.fail()) {
    	return std::shared_ptr<Command>(NULL);
    }

    return std::shared_ptr<Command>(new VCommand(num));
}

//E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
std::shared_ptr<Command> CommandParser::getECommand(std::string const & paramLine) {
    std::string processStr = paramLine;
    eraseCharFromOneStr(processStr, '{');
    eraseCharFromOneStr(processStr, '}');
    eraseCharFromOneStr(processStr, ' ');
    eraseCharFromOneStr(processStr, '<');
    eraseCharFromOneStr(processStr, '>');

    StringVec vertexVec;
    splitStrByChar(processStr, ',', vertexVec);

    UIntPairVec points;
    StringVec::const_iterator vertexVecIt = vertexVec.begin();
    while(vertexVecIt != vertexVec.end()) {
    	bool isValidNum = true;
    	unsigned int p1 = 0;
    	std::istringstream strPoint1(*vertexVecIt);
    	strPoint1 >> p1;
        if (strPoint1.fail()) {
        	isValidNum = false;
        	//return std::shared_ptr<Input>(NULL);
        }

    	vertexVecIt++;
    	if(vertexVecIt == vertexVec.end()) {
    		break;
    	}

    	unsigned int p2 = 0;
    	std::istringstream strPoint2(*vertexVecIt);
    	strPoint2 >> p2;
        if (strPoint2.fail()) {
        	isValidNum = false;
        	//return std::shared_ptr<Input>(NULL);
        }

        if(isValidNum) {
        	UIntPair pair(p1, p2);
        	points.push_back(pair);
        }

    	vertexVecIt++;
    }

	return std::shared_ptr<Command>(new ECommand(points));
}

void CommandParser::eraseCharFromOneStr(std::string & str, char eCh)
{
	std::string::size_type pos = std::string::npos;
    while((pos = str.find(eCh)) != std::string::npos) {
    	str.erase(pos, 1);
    }
}

void CommandParser::splitStrByChar(std::string const & str, char ch, StringVec & outVec)
{
	std::string::size_type pos = std::string::npos;
	std::string::size_type sPos = 0;
	while((pos = str.find(ch, sPos)) != std::string::npos) {
        if(pos == str.size() - 1) {
        	break;
        }

        outVec.push_back(str.substr(sPos, pos-sPos));
		sPos = pos + 1;
	}

	if(sPos != std::string::npos) {
		outVec.push_back(str.substr(sPos, str.size()-sPos));
	}
}

//s 2 10
std::shared_ptr<Command> CommandParser::getSCommand(std::string const & paramLine) {
	if(paramLine.find("-") != std::string::npos) {
		return std::shared_ptr<Command>(NULL);
	}

	std::istringstream strLine(paramLine);
	unsigned int start = 0;
	unsigned int end = 0;
	strLine >> start >> end;
    if (strLine.fail()) {
    	return std::shared_ptr<Command>(NULL);
    }

	return std::shared_ptr<Command>(new SCommand(start, end));
}









