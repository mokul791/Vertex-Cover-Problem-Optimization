#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "DataInfo.hpp"

int main (int argc, char **argv) {
    int pipeA1A2[2];
    pipe(pipeA1A2);

    int pipeRgenA1[2];
    pipe(pipeRgenA1);

    pid_t a2Pid = fork ();
    if(a2Pid == 0)
    {
        dup2(pipeA1A2[0], STDIN_FILENO);
        close(pipeA1A2[1]);
        close(pipeA1A2[0]);

        char * myargv[2];
        myargv[0] = (char*)"ece650-a2";
        myargv[1] = nullptr;
        execv ("./ece650-a2", myargv);

        std::cerr << "Error: execute ece650-a2 error\n";
        return 1;
    }
    else if (a2Pid < 0) {
		std::cerr << "Error: could not fork\n";
		return 1;
    }

    pid_t a1Pid = fork ();
    if(a1Pid == 0)
    {
        dup2(pipeA1A2[1], STDOUT_FILENO);
        close(pipeA1A2[0]);
        close(pipeA1A2[1]);

        dup2(pipeRgenA1[0], STDIN_FILENO);
        close(pipeRgenA1[1]);
        close(pipeRgenA1[0]);

        char * myargv[2];
        myargv[0] = (char*)"ece650-a1.py";
        myargv[1] = nullptr;
        execv ("./ece650-a1.py", myargv);

        std::cerr << "Error: execute ece650-a1.py error\n";
        return 1;
    }
    else if (a1Pid < 0) {
		std::cerr << "Error: could not fork\n";
		return 1;
    }

    dup2(pipeA1A2[1], STDOUT_FILENO);
    close(pipeA1A2[0]);
    close(pipeA1A2[1]);

    pid_t rGenPid = fork ();
    if(rGenPid == 0)
    {
        dup2(pipeRgenA1[1], STDOUT_FILENO);
        close(pipeRgenA1[0]);
        close(pipeRgenA1[1]);

        char ** myargv = new char*[argc+1];
        for(int i = 0; i < argc; i++) {
            if(i == 0) {
            	myargv[0] = (char*)"rgen";
            	continue;
            }

            myargv[i] = argv[i];
        }
        myargv[argc] = nullptr;

        execv ("./rgen", myargv);

        std::cerr << "Error: execute rgen error\n";
        return 1;
    }
    else if (rGenPid < 0) {
		std::cerr << "Error: could not fork\n";
		return 1;
    }

    while (!std::cin.eof()) {

        std::string line;
        std::getline(std::cin, line);

        if (line.size() == 0) {
            continue;
        }

        std::cout << line << std::endl;
    }

    close(STDOUT_FILENO);

    int status = 0;
    kill(rGenPid, SIGTERM);
    waitpid(rGenPid, &status, 0);

    kill(a1Pid, SIGTERM);
    waitpid(a1Pid, &status, 0);

    kill(a2Pid, SIGTERM);
    waitpid(a2Pid, &status, 0);


    return 0;
}
