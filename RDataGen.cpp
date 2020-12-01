

#include "RDataGen.hpp"

RandomDataGen::~RandomDataGen() {
}

class RandomDataGenImpl : public RandomDataGen {
public:
	~RandomDataGenImpl() {
		urandom_.close();
	}

	RandomDataGenImpl():urandom_("/dev/urandom") {
		if (urandom_.fail()) {
            std::cerr << "Error: cannot open /dev/urandom\n";
            throw "random gen error";
        }
	}

	virtual unsigned int getRandomNum(unsigned int max, unsigned int min)
	{
    	unsigned int num = 0;
    	urandom_.read((char*)&num, sizeof(unsigned int));
    	return num%(max-min+1) + min;
	}

private:
	std::ifstream urandom_;
};

std::shared_ptr<RandomDataGen> RandomDataGen::create() {
	return std::shared_ptr<RandomDataGen>(new RandomDataGenImpl);
}
