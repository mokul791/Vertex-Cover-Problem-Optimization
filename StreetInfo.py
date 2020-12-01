

from LineInfo import *
from BasicDataInfo import *

class StreetInfo(object):
    def __init__(self, name):
        self.rname = name
        self.lines = []
        self.name = self.rname.lower()

    def addLine(self, line):    
        self.lines.append(line)
    
    def __str__(self):
        info = "(" + self.name + ":"
        
        for line in self.lines:
            info = info + line.name
            
        return info + ")"
    
class StreetCollection(GetData):
    def __init__(self):
        GetData.__init__(self)

    def getStreet(self, name):
        if  name.lower() in self.tData:
            return self.tData[name.lower()]
        
        street = StreetInfo(name)
        self.tData[street.name] = street
        return self.tData[street.name]
    
    def removStreet(self, name):
        if  name.lower() in self.tData:
            return self.tData.pop(name.lower())

class StreetsBuilder(object):
    def __init__(self, pointCollection, lineCollection):
        self.pointCollection = pointCollection
        self.lineCollection = lineCollection

    def addStreetLines(self, street, coordinates):
        for i in range(len(coordinates)):
            if i == len(coordinates)-1:
                break
            
            p0 = self.pointCollection.getPoint(coordinates[i][0], coordinates[i][1])
            p1 = self.pointCollection.getPoint(coordinates[i+1][0], coordinates[i+1][1])
            line = self.lineCollection.getLine(p0, p1, street.name)
            street.addLine(line)
            
            for k in self.lineCollection.getData():
                oneLine = self.lineCollection.getData()[k]
                

                if street.name in oneLine.streetIds:
                    continue
                
                line.addIntersectionPoint(oneLine, self.pointCollection)
                oneLine.addIntersectionPoint(line, self.pointCollection)
                
     
if __name__ == '__main__':
    pointCollection = PointCollection()
    lineCollection = LineSegmentCollection()
    streetCollection = StreetCollection()
    builder = StreetsBuilder(pointCollection, lineCollection)
    


    
    
    
    
    
    
    
    
    
    
    
    





