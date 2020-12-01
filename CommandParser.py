
import sys
import re
from TrafficDataProcessor import *

class GetIP(object):
    def __init__(self, dataProcessor, sName, coordinates):
        self.dataProcessor = dataProcessor
        self.streetName = sName
        self.coordinates = coordinates

    def execute(self):
        pass

class AddStr(GetIP):
    def __init__(self, dataProcessor, sName, coordinates):
        GetIP.__init__(self, dataProcessor, sName, coordinates)

    def execute(self):
        self.dataProcessor.addStreet(self.streetName, self.coordinates)
        
    def __str__(self):
        return "AddStr. StreetName:" + self.streetName + ", coordinates:" + \
            str(self.coordinates)
    
    __repr__ = __str__

class ChangeStr(GetIP):
    def __init__(self, dataProcessor, sName, coordinates):
        GetIP.__init__(self, dataProcessor, sName, coordinates)

    def execute(self):
        self.dataProcessor.changeStreet(self.streetName, self.coordinates) 
    
    def __str__(self):
        return "ChangeStr. StreetName:" + self.streetName + ", coordinates:" + \
            str(self.coordinates)
    
    __repr__ = __str__
    
class RemStr(object):
    def __init__(self, dataProcessor, sName):
        self.dataProcessor = dataProcessor
        self.sName = sName

    def execute(self):
        self.dataProcessor.removeStreet(self.sName)   
        
    def __str__(self):
        return "RemStr. StreetName:" + self.sName
    __repr__ = __str__    
        
class GenGraph(object):
    def __init__(self, dataProcessor):
        self.dataProcessor = dataProcessor

    def execute(self):
        self.dataProcessor.genPrintGraph()
    
    def __str__(self):
        return "GenGraph"
    __repr__ = __str__      

class CmdExec(object):
    def __init__(self, dataProcessor):
        self.dataProcessor = dataProcessor
        
    def ExecCmd(self, line):
        command = self.ParseCmd(line)
        if command == None:
            sys.stderr.write("Error:input command format is not valid. Please check it.\n")
        else:
            command.execute()
    
    def ParseCmd(self, line):
        command = self.Check(line)
        if command != None:
            return command
        
        command = self.GetRemCmd(line)
        if command != None:
            return command       
        
        command = self.GetGphCmd(line)
        if command != None:
            return command       

        return None
        
    def Check(self, line):
        r=re.compile(r' *[ac] +"[a-zA-Z ]+" +(\([+-]?[0-9]+,[+-]?[0-9]+\) *)+[\r\n ]*$')
        if not r.match(line):
            return None
        
        r=re.compile(r' *([ac]) +"([a-zA-Z ]+)"')
        
        cmdStr = r.search(line).group(1)
        streetName = r.search(line).group(2)
        
        r=re.compile(r'\([+-]?[0-9]+,[+-]?[0-9]+\)')
        coordinatesStr = r.findall(line)
        
        coordinates = []
        for oneCoordinateStr in coordinatesStr:
            oneCoordiateL = oneCoordinateStr.split(',')
            xCoordinate = oneCoordiateL[0].strip('( ')
            yCoordinate = oneCoordiateL[1].strip(') ')
            oneCoordiate = (xCoordinate, yCoordinate)
            coordinates.append(oneCoordiate)
            
        if cmdStr == 'a':
            return AddStr(self.dataProcessor, streetName, coordinates)
        elif cmdStr == 'c':
            return ChangeStr(self.dataProcessor, streetName, coordinates)
        else:
            return None
        
    def GetRemCmd(self, line):
        r=re.compile(r' *r +"[a-zA-Z ]+"[\r\n ]*$')
        if not r.match(line):
            return None
        
        r=re.compile(r' *r +"([a-zA-Z ]+)"')  
        streetName = r.search(line).group(1)
        
        command = RemStr(self.dataProcessor, streetName)
        return command 

    def GetGphCmd(self, line):
        r=re.compile(r' *g[\r\n ]*$')
        if not r.match(line):
            return None
        
        command = GenGraph(self.dataProcessor)
        return command



if __name__ == '__main__':
    tDataProcessor = TDataProcessor()
    executer = CmdExec(tDataProcessor)

    

    
    
    