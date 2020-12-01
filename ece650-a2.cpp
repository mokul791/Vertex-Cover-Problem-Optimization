
#include "InputParser.hpp"
#include "InputProcessor.hpp"

int main(int argc, char** argv) {

    while (!std::cin.eof()) {

        std::string line;
        std::getline(std::cin, line);

        if (line.size() == 0) {
            continue;
        }

        std::shared_ptr<Command> cmd = CommandParser::getCommand(line);
        if(cmd.get() == NULL) {
        	std::cerr << "Error:input command format not matched. Please input it in correct format." << std::endl;
        	continue;
        }

        CommandProcessor::instance().processCommand(*cmd);
    }

}
