
#include "RStreetGen.hpp"
#include "RDataGen.hpp"
#include "StreetInfo.hpp"

RandomStreetGenerator::~RandomStreetGenerator() {}

class RandomStreetGeneratorImpl : public RandomStreetGenerator
{
public:
	virtual ~RandomStreetGeneratorImpl() {}


	RandomStreetGeneratorImpl(StreetMgr & mgr)
	: streetMinNum_(2), streetMaxNum_(10), segmentMinNum_(1),
	  segmentMaxNum_(5), coordinateRange_(20), mgr_(mgr) {
	}

	void setStreetMaxNum(unsigned int maxNum) {
		streetMaxNum_ = maxNum;
	}

	void setSegmentMaxNum(unsigned int maxNum) {
		segmentMaxNum_ = maxNum;
	}

	void setCoordinateRange(unsigned int range) {
		coordinateRange_ = range;
	}

	bool generateStreets(void)
	{
		std::shared_ptr<RandomDataGen> randomDataGen = RandomDataGen::create();
	    genStreets(randomDataGen);

        return mgr_.getStreets().size() >= streetMinNum_;
	}

	void genStreets(std::shared_ptr<RandomDataGen> const & randomDataGen)
	{
		unsigned int streetNum = randomDataGen->getRandomNum(streetMaxNum_, streetMinNum_);

        unsigned int i = 0;
        while(1)
        {
        	if(mgr_.getStreets().size() == streetNum) {
        		break;
        	}

        	i++;

        	if(i == 25) {
        		break;
        	}

        	bool succ = false;

        	StreetInfo street(genStreetName(i));
        	succ = genStreetFromQuadrant(street, randomDataGen);

        	if(succ && (!mgr_.isStreetExist(street))) {

        		mgr_.addStreet(street);
        	}
        }
	}

	bool genStreetFromQuadrant(StreetInfo & street,
			std::shared_ptr<RandomDataGen> const & randomDataGen)
	{
		unsigned int quadrant = randomDataGen->getRandomNum(4, 1);

		unsigned int streetSegmentNum = randomDataGen->getRandomNum(segmentMaxNum_, segmentMinNum_);


    	unsigned int stepMax = coordinateRange_;
    	if(stepMax == 0) {
    		stepMax = 1;
    	}

		int startX = randomDataGen->getRandomNum(coordinateRange_, 1);
		int startY = randomDataGen->getRandomNum(coordinateRange_, 1);
		genStartPoint(quadrant, startX, startY);
		CoordinateInfo startPoint(startX, startY);

        unsigned int i = 0;
        while(1)
        {
        	if(street.getSegments().size() == streetSegmentNum) {
        		break;
        	}

        	if(i == 15) {
        		break;
        	}

        	i++;



    		unsigned int stepX = randomDataGen->getRandomNum(stepMax, 0);
    		unsigned int stepY = randomDataGen->getRandomNum(stepMax, 0);


    		if(stepX == 0 && stepY == 0) {
    			stepX = 1;
    			stepY = 1;
    		}

    		int newX = 0;
    		int newY = 0;

    		if(!genNewPoint(quadrant, startPoint, stepX, stepY, newX, newY)) {

    			continue;
    		}


    		CoordinateInfo newPoint(newX, newY);
    		StreetSegment seg(startPoint, newPoint);
    		street.addOneSegment(seg);

    		startPoint = newPoint;
        }

        return street.getSegments().size() >= segmentMinNum_;
	}

	void genStartPoint(unsigned int quadrant, int & x, int & y) {
        if(quadrant == 1) {
            return;
        }
        else if(quadrant == 2) {
        	x = -x;
        	return;
        }
        else if(quadrant == 3) {
        	x = -x;
        	y = -y;
        	return;
        }
        else {
        	y = -y;
        	return;
        }
	}

    bool genNewPoint(unsigned int quadrant, CoordinateInfo const & startPoint,
    		unsigned int stepX, unsigned int stepY,
    		int & newX, int & newY)
    {
        if(quadrant == 1) {
    		newX = startPoint.getx() - stepX;
    		newY = startPoint.gety() - stepY;

    		if((int)newX < (int)(-coordinateRange_) || (int)newY < (int)(-coordinateRange_)) {
    			return false;
    		}
    		else {
    			return true;
    		}
        }
        else if(quadrant == 2) {
    		newX = startPoint.getx() + stepX;
    		newY = startPoint.gety() - stepY;
    		if((int)newX > (int)coordinateRange_ || (int)newY < (int)(-coordinateRange_)) {
    			return false;
    		}
    		else {
    			return true;
    		}
        }
        else if(quadrant == 3) {
    		newX = startPoint.getx() + stepX;
    		newY = startPoint.gety() + stepY;
    		if((int)newX > (int)coordinateRange_ || (int)newY > (int)coordinateRange_) {
    			return false;
    		}
    		else {
    			return true;
    		}
        }
        else {
    		newX = startPoint.getx() - stepX;
    		newY = startPoint.gety() + stepY;

    		if((int)newX < (int)(-coordinateRange_) || (int)newY > (int)(coordinateRange_)) {
    			return false;;
    		}
    		else {
    			return true;
    		}
        }
    }

    void genTwoSimpleLine() {


    	mgr_.clearAllStreets();

    	StreetInfo street0(genStreetName(0));
    	CoordinateInfo start0(-coordinateRange_, coordinateRange_);
    	CoordinateInfo end0(coordinateRange_, -coordinateRange_);
    	StreetSegment segment0(start0, end0);
    	street0.addOneSegment(segment0);
    	mgr_.addStreet(street0);

    	StreetInfo street1(genStreetName(1));
    	CoordinateInfo start1(-coordinateRange_, -coordinateRange_);
    	CoordinateInfo end1(coordinateRange_, coordinateRange_);
    	StreetSegment segment1(start1, end1);
    	street1.addOneSegment(segment1);
    	mgr_.addStreet(street1);
    }

    std::string genStreetName(unsigned int i) {
    	std::ostringstream oss;
    	oss << "Street";
    	for(unsigned int j =0; j < i; j++) {
    		oss << "a";
    	}

    	return oss.str();
    }


private:
	unsigned int streetMinNum_;
	unsigned int streetMaxNum_;
	unsigned int segmentMinNum_;
	unsigned int segmentMaxNum_;
	unsigned int coordinateRange_;
	StreetMgr & mgr_;

};



std::shared_ptr<RandomStreetGenerator> RandomStreetGenerator::create(StreetMgr & mgr) {
	return std::shared_ptr<RandomStreetGenerator>(new RandomStreetGeneratorImpl(mgr));
}













