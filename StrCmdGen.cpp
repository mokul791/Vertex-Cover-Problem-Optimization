
#include "StrCmdGen.hpp"
#include "StreetInfo.hpp"

StreetCommandGenerator::~StreetCommandGenerator() {}

class StreetCommandGeneratorImpl : public StreetCommandGenerator {
public:
	StreetCommandGeneratorImpl(StreetMgr const & mgr)
    :mgr_(mgr) {
	}

	StringVec generateAddStreetCommand() {
		StringVec cmdVec;

		StreetMgr::StreetMap const & streets = mgr_.getStreets();
		StreetMgr::StreetMap::const_iterator streetIt = streets.begin();
		for(; streetIt != streets.end(); streetIt++) {
            cmdVec.push_back(genAddCommand(streetIt->second));
		}

		cmdVec.push_back("g");

        return cmdVec;
	}

	std::string genAddCommand(StreetInfo const & streetInfo) {
        std::ostringstream oss;
        oss << "a \"" << streetInfo.getName() << "\" ";

        StreetInfo::SegmentVec const & segments = streetInfo.getSegments();
        StreetInfo::SegmentVec::size_type i = 0;
        for(; i != segments.size(); i++) {
        	if(i == 0) {
        		oss << getCoordinateStr(segments[i].getStart()) << " ";
        		oss << getCoordinateStr(segments[i].getEnd());
        	}
        	else {
        		oss << getCoordinateStr(segments[i].getEnd());
        	}

        	if(i != segments.size()-1) {
        		oss << " ";
        	}
        }

        return oss.str();
	}

	std::string getCoordinateStr(CoordinateInfo const & coordinate) {
		std::ostringstream oss;
		oss << "(" << coordinate.getx() << "," << coordinate.gety() << ")";
		return oss.str();
	}

	StringVec generateRemoveStreetCommand() {
		StringVec cmdVec;

		StreetMgr::StreetMap const & streets = mgr_.getStreets();
		StreetMgr::StreetMap::const_iterator streetIt = streets.begin();
		for(; streetIt != streets.end(); streetIt++) {
            std::string command = "r \"" + streetIt->second.getName() + "\"";
            cmdVec.push_back(command);
		}

        return cmdVec;
	}

private:
	StreetMgr const & mgr_;

};

std::shared_ptr<StreetCommandGenerator> StreetCommandGenerator::create(StreetMgr const & mgr)
{
    return std::shared_ptr<StreetCommandGenerator>(new StreetCommandGeneratorImpl(mgr));
}





