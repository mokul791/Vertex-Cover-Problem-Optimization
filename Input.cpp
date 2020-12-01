

#include "Input.hpp"
#include "GraphGen.hpp"

Command::~Command() {
}

VCommand::VCommand(unsigned int num): num_(num) {
}

bool VCommand::execute() const {
    return DiagramDataMgr::instance().createDiagram(num_);
}

Command::Type VCommand::type()const {
	return VType;
}

std::string VCommand::str () const{
    std::ostringstream oss;
    oss << "VertexInput:" << num_;
    return oss.str();
}

VCommand::~VCommand() {
}

ECommand::ECommand(UIntPairVec const & points)
    : points_(points) {
}

bool ECommand::execute() const {
	return DiagramDataMgr::instance().buildDiagramEdges(points_);
}

Command::Type ECommand::type()const{
	return EType;
}

std::string ECommand::str () const {
    std::ostringstream oss;
    oss << "EdgeInput:";

    std::ostringstream ossPoint;
    UIntPairVec::const_iterator pointIt = points_.begin();
    for(; pointIt != points_.end(); pointIt++) {
    	ossPoint << "(" << pointIt->first << "," << pointIt-> second << ") ";
    }

    oss << ossPoint.str();

    return oss.str();
}

ECommand::~ECommand() {
}

SCommand::SCommand(unsigned int start, unsigned int end)
    : start_(start), end_(end) {
}

bool SCommand::execute() const {
	return DiagramDataMgr::instance().genShortPath(start_, end_);
}

Command::Type SCommand::type()const{
	return sType;
}

std::string SCommand::str () const{
    std::ostringstream oss;
    oss << "ShortestPathInput:" << start_ << ", " << end_;
    return oss.str();
}

SCommand::~SCommand() {
}


