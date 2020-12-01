
#include "GraphGen.hpp"

DiagramDataMgr::~DiagramDataMgr() {
}

class Vertex
{
public:

	typedef std::vector<std::shared_ptr<Vertex> > VertexPtrVec;

	Vertex(unsigned int id) : id_(id) {
	}

	unsigned int getId() const {
		return id_;
	}

    void addEdge(std::shared_ptr<Vertex> const & point) {
    	if(point->getId() == getId()) {
    		return;
    	}

    	VertexPtrVec::const_iterator vertexVecIt = edges_.begin();
    	for(; vertexVecIt != edges_.end(); vertexVecIt++)
    	{
    		if((*vertexVecIt)->getId() == point->getId()) {
    			return;
    		}
    	}

    	edges_.push_back(point);
    }

    VertexPtrVec const & getEdges()const {
    	return edges_;
    }

    std::string str() const {
        std::ostringstream oss;
        oss << "id:" << id_ << ", ";

        oss << "edges:";
        VertexPtrVec::const_iterator edgeIt = edges_.begin();
        for(; edgeIt != edges_.end(); edgeIt++) {
        	oss << (*edgeIt)->getId() << ",";
        }

        return oss.str();
    }

private:
	unsigned int id_;
	VertexPtrVec edges_;

};

class DiagramDataMgrImpl : public DiagramDataMgr {
public:

	typedef std::map<unsigned int, std::shared_ptr<Vertex> > VertexPtrMap;
	typedef std::set<unsigned int> VertexSet;
	typedef std::map<unsigned int, unsigned int> VetexParentMap;

	virtual ~DiagramDataMgrImpl() {}

	virtual bool createDiagram(unsigned int num)
	{
		vertexMap_.clear();


		for(unsigned int i = 0; i < num; i++)
		{
			vertexMap_[i] = std::shared_ptr<Vertex> (new Vertex(i));
		}

        return true;
	}

	virtual bool buildDiagramEdges(UIntPairVec const & pairs)
	{
		if(!checkPairValid(pairs)) {
			std::cerr << "Error:E command fail. Vertex not exists" << std::endl;
			return false;
		}

		UIntPairVec::const_iterator pairIt = pairs.begin();
		for(; pairIt != pairs.end(); pairIt++)
		{
			std::shared_ptr<Vertex> & vertex1 = vertexMap_[pairIt->first];
			std::shared_ptr<Vertex> & vertex2 = vertexMap_[pairIt->second];

			vertex1->addEdge(vertex2);
			vertex2->addEdge(vertex1);
		}


        return true;
	}

	bool checkPairValid(UIntPairVec const & pairs) {
		UIntPairVec::const_iterator pairIt = pairs.begin();
		for(; pairIt != pairs.end(); pairIt++)
		{
			if(vertexMap_.find(pairIt->first) == vertexMap_.end()
					|| vertexMap_.find(pairIt->second) == vertexMap_.end()) {
				return false;
			}
		}

        return true;
	}

	virtual bool genShortPath(unsigned int start, unsigned int end)
	{
        if(vertexMap_.find(start) == vertexMap_.end()
					|| vertexMap_.find(end) == vertexMap_.end()) {
			std::cerr << "Error:s command fail. Vertex not exists" << std::endl;
			return false;
        }

        VetexParentMap parentMap;
        bfsVisit(*(vertexMap_[start]), *(vertexMap_[end]), parentMap);

        UIntVec paths;
        genPathVec(*(vertexMap_[start]), *(vertexMap_[end]), parentMap, paths);
        if(paths.empty()) {
        	std::cerr << "Error:s command fail. No path exists." << std::endl;
        	return false;
        }

        printPath(paths);

        return true;
	}

	void bfsVisit(Vertex const & sVertex, Vertex const & eVertex,
			VetexParentMap & parentMap)
	{
        if(sVertex.getId() == eVertex.getId()) {
        	parentMap[sVertex.getId()] = eVertex.getId();
        	return;
        }

        VertexSet visitedSet;
        UIntQueue bfsQueue;

        bfsQueue.push(sVertex.getId());
        visitedSet.insert(sVertex.getId());
        while(!bfsQueue.empty())
        {
        	unsigned int curId = bfsQueue.front();
        	bfsQueue.pop();


        	Vertex::VertexPtrVec const & curEdges = vertexMap_[curId]->getEdges();
        	Vertex::VertexPtrVec::const_iterator edgeIt = curEdges.begin();
        	for(; edgeIt != curEdges.end(); edgeIt++)
        	{
        		unsigned int edgeId = (*edgeIt)->getId();
        		if(visitedSet.find(edgeId) != visitedSet.end()) {
        			continue;
        		}


        		bfsQueue.push(edgeId);
        		visitedSet.insert(edgeId);

        		parentMap[edgeId] = curId;
            	if(edgeId == eVertex.getId()) {
            		return;
            	}
        	}
        }
	}

    void genPathVec(Vertex const & sVertex, Vertex const & eVertex, VetexParentMap const & parentMap,
    		UIntVec & paths)
    {
    	paths.clear();
    	if(parentMap.find(eVertex.getId()) == parentMap.end()) {
    		return;
    	}

    	if(parentMap.find(sVertex.getId()) != parentMap.end()) {
    		paths.push_back(sVertex.getId());
    		return;
    	}

    	UIntVec tmpPaths;
    	unsigned int curId = eVertex.getId();
    	tmpPaths.push_back(curId);

    	VetexParentMap::const_iterator pMapIt = parentMap.find(curId);
    	while(pMapIt != parentMap.end()) {
    		tmpPaths.push_back(pMapIt->second);

    		curId = pMapIt->second;
    		pMapIt = parentMap.find(curId);
    	}

    	UIntVec::const_reverse_iterator rIt = tmpPaths.rbegin();
    	for(; rIt != tmpPaths.rend(); rIt++) {
    		paths.push_back(*rIt);
    	}


    }

    void printPath(UIntVec const & paths) {
        for(unsigned int i = 0; i < paths.size(); i++) {
        	std::cout << paths[i];
        	if(i != paths.size()-1) {
        		std::cout << "-";
        	}
        }

        std::cout << std::endl;
    }


private:
	VertexPtrMap vertexMap_;

};

DiagramDataMgr & DiagramDataMgr::instance() {
	static DiagramDataMgrImpl ins;
	return ins;
}
