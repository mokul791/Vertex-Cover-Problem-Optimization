#include "InputProcessor.hpp"

CommandProcessor::~CommandProcessor() {
}

class CommandProcessorImpl : public CommandProcessor {
public:
    virtual ~CommandProcessorImpl() {
    }

    CommandProcessorImpl():previousT_(Command::InvalidType) {
    }

    virtual void processCommand(Command const & cmd) {
        if(!checkCurCmd(cmd)) {
        	return;
        }

        if(cmd.execute()) {
        	previousT_ = cmd.type();
        }
    }

    bool checkCurCmd(Command const & cmd) {
    	if(previousT_ == Command::VType) {
    		if(cmd.type() == Command::EType) {
    			return true;
    		}
    		else {
    			std::cerr << "Error:input command wrong type. Please input E command." << std::endl;
    			return false;
    		}
    	}
    	else if (previousT_ == Command::EType || previousT_ == Command::sType) {
    		if(cmd.type() == Command::VType || cmd.type() == Command::sType) {
    			return true;
    		}
    		else {
    			std::cerr << "Error:input command wrong type. Please input V command or S Input." << std::endl;
    			return false;
    		}
    	}
    	else if (previousT_ == Command::InvalidType) {
    		if(cmd.type() == Command::VType) {
    			return true;
    		}
    		else {
    			std::cerr << "Error:input command wrong type. Please input V command." << std::endl;
    			return false;
    		}
    	}

    	return false;
    }


private:
    Command::Type previousT_;


};

CommandProcessor & CommandProcessor::instance() {
	static CommandProcessorImpl ins;
	return ins;
}
