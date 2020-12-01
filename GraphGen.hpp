

#ifndef DIAGRAMDATAMGR_HPP_
#define DIAGRAMDATAMGR_HPP_

#include "DataInfo.hpp"

class DiagramDataMgr {
public:
	virtual ~DiagramDataMgr();

	virtual bool createDiagram(unsigned int num) = 0;
	virtual bool buildDiagramEdges(UIntPairVec const & pairs) = 0;
	virtual bool genShortPath(unsigned int start, unsigned int end) = 0;

    static DiagramDataMgr & instance();

};

#endif 
