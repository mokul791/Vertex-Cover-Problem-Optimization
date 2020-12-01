#include <unistd.h>
#include "DataInfo.hpp"
#include "RStreetGen.hpp"
#include "StreetInfo.hpp"
#include "StrCmdGen.hpp"
#include "RDataGen.hpp"

bool getDataFromStr(std::string const & str, int & data) {
	std::istringstream input(str);
	input >> data;
    if (input.fail()) {
    	return false;
    }
    return true;
}

int main(int argc, char** argv) {
    opterr = 0;

    int waitInterval = 5;
    StreetMgr streetMgr;

    std::shared_ptr<RandomStreetGenerator> streetGen = RandomStreetGenerator::create(streetMgr);
    std::shared_ptr<StreetCommandGenerator> streetCmdGen = StreetCommandGenerator::create(streetMgr);

    std::string cvalue;
    int ch = 0;
    while ((ch = getopt (argc, argv, "s:n:l:c:")) != -1)
        switch (ch)
        {
        case 's':
			{
				cvalue = optarg;
				int sValue = 0;
				if(!getDataFromStr(cvalue, sValue)) {
					std::cerr << "Error: rgen. s option argument error" << std::endl;
					return 1;
				}

				if(sValue < 2) {
					std::cerr << "Error: rgen. s option argument error" << std::endl;
					return 1;
				}



				streetGen->setStreetMaxNum(sValue);
			}

            break;

        case 'n':
			{
				cvalue = optarg;
				int nValue = 0;
				if(!getDataFromStr(cvalue, nValue)) {
					std::cerr << "Error: rgen. n option argument error" << std::endl;
					return 1;
				}

				if(nValue < 1) {
					std::cerr << "Error: rgen. n option argument error" << std::endl;
					return 1;
				}



				streetGen->setSegmentMaxNum(nValue);
			}

            break;

        case 'l':
			{
				cvalue = optarg;
				if(!getDataFromStr(cvalue, waitInterval)) {
					std::cerr << "Error: rgen. l option argument error" << std::endl;
					return 1;
				}

				if(waitInterval < 5) {
					std::cerr << "Error: rgen. l option argument error" << std::endl;
					return 1;
				}

			}

            break;

        case 'c':
			{
				cvalue = optarg;
				int coordinate = 0;
				if(!getDataFromStr(cvalue, coordinate)) {
					std::cerr << "Error: rgen. c option argument error" << std::endl;
					return 1;
				}

				if(coordinate < 1) {
					std::cerr << "Error: rgen. c option argument error" << std::endl;
					return 1;
				}


				streetGen->setCoordinateRange(coordinate);
			}

            break;

        case '?':
            std::cerr << "Error: rgen unknown option or missing arguments. Option:" << (char)optopt << std::endl;
            return 1;
        }


        while(1) {


        	if(!streetGen->generateStreets()) {
        		std::cerr << "Error: fail to generate streets" << std::endl;
        		return 1;
        	}

        	StringVec addCmdVec = streetCmdGen->generateAddStreetCommand();
        	StringVec::const_iterator addCmdVecIt = addCmdVec.begin();
        	for(; addCmdVecIt != addCmdVec.end(); addCmdVecIt++) {
        		std::cout << (*addCmdVecIt) << std::endl;

        	}

        	std::shared_ptr<RandomDataGen> randomDataGen = RandomDataGen::create();

        	unsigned int waitTime = randomDataGen->getRandomNum(waitInterval, 5);


        	sleep(waitTime);

        	StringVec removeCmdVec = streetCmdGen->generateRemoveStreetCommand();
        	StringVec::const_iterator removeCmdVecIt = removeCmdVec.begin();
        	for(; removeCmdVecIt != removeCmdVec.end(); removeCmdVecIt++) {
        		std::cout << (*removeCmdVecIt) << std::endl;
        	}

        	streetMgr.clearAllStreets();
        }

    return 0;
}
