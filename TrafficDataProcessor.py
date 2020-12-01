
import sys

from LineInfo import *
from BasicDataInfo import *
from StreetInfo import *
from GraphInfo import *


class TDataProcessor(object):
    def __init__(self):
        self.pointCollection = PointCollection()
        self.lineCollection = LineSegmentCollection()
        self.streetCollection = StreetCollection()
        self.builder = StreetsBuilder(self.pointCollection, self.lineCollection)
        self.graph = Graph()
        self.graphGenerator = GraphGen(self.streetCollection, self.graph)
        self.graphPrinter = GraphPtr(self.graph)
        
    def addStreet(self, name, coordinates):
        if self.streetCollection.SearchElm(name.lower()):
            sys.stderr.write("Error:addStreet fail. Street Name:" + name + " already exists\n")
            return

        street = self.streetCollection.getStreet(name.lower())
        self.builder.addStreetLines(street, coordinates)
        
    def changeStreet(self, name, coordinates):
        if not self.streetCollection.SearchElm(name.lower()):
            sys.stderr.write("Error:changeStreet fail. Street Name:" + name + " does not exist\n")
            return
        
        self.removeStreet(name.lower())
        self.addStreet(name.lower(), coordinates)
        
    def removeStreet(self, name):
        if self.streetCollection.SearchElm(name.lower()) == None:
            sys.stderr.write("Error:removeStreet fail. Street Name:" + name.lower() + " does not exist\n")
            return

        deletedLines = self.lineCollection.getDeletedLine(name.lower())
        self.pointCollection.removeDeletedStreetInfo(name.lower(), deletedLines)
        
        self.lineCollection.removeLineBySName(name.lower(), deletedLines)
        self.streetCollection.removStreet(name.lower())  

    def genGraph(self):
        self.graphGenerator.generate()
    
    def printGraph(self):
        print (self.graphPrinter.OutputGraph())
        
    def genPrintGraph(self):
        self.graphGenerator.generate()
        output=self.graphPrinter.OutputGraph()
        if output != "":
            print(output)
            sys.stdout.flush()

        
if __name__ == '__main__':
    processor = TDataProcessor()

    
    