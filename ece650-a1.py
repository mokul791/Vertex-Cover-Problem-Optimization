#!/usr/bin/env python

import sys
from TrafficDataProcessor import *
from CommandParser import *

# YOUR CODE GOES HERE

def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment
    
    tDataProcessor = TDataProcessor()
    cmdExec = CmdExec(tDataProcessor)

    while True:
        line = sys.stdin.readline()
        if line == '':
            break

        
        try:
            cmdExec.ExecCmd(line)
        except Exception as ex:
            sys.stderr.write("Error:" + str(ex) + '\n')

    sys.exit(0)

if __name__ == '__main__':
    main()
