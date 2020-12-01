

from BasicDataInfo import *

class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)
        self.name = self.buildPointName(self.x, self.y)
        self.lineIds = set()
        self.streetIds = set()
        
    def __eq__(self, l):
        if(l == None):
            return False
        
        return self.name == l.name
        
    def addLineId(self, lineId):
        self.lineIds.add(lineId)
    
    def addStreetIds(self, streetIds):
        self.streetIds = self.streetIds.union(streetIds)
        
    def addStreetId(self, streetId):
        self.streetIds.add(streetId)
        
    def removeStreetInfo(self, streetId, deletedLineIds):
        if streetId in self.streetIds:
            self.streetIds.remove(streetId)    
        
        for oneDeleteLineId in deletedLineIds:
            if oneDeleteLineId.name in self.lineIds:
                self.lineIds.remove(oneDeleteLineId.name)
                
    def clearLineStrId(self):
        self.lineIds = set()
        self.streetIds = set()   
    
            
    def __str__(self):
        return self.name + ": sIds:" + str(self.streetIds) + ", lIds:"    \
            + str(self.lineIds) + ")"
            
    __repr__ = __str__
    
    def __hash__(self):
        return hash(self.name)
        
    @staticmethod
    def buildPointName(x, y):
        return "(" + "{0:.2f}".format(x) + "_" + "{0:.2f}".format(y) + ")"
    
class PointCollection(GetData):
    def __init__(self):
        GetData.__init__(self)

    def getPoint(self, x, y):
        name = Point.buildPointName(float(x), float(y))
        if  name in self.tData:
            return self.tData[name]
        
        p = Point(x, y)
        self.tData[name] = p
        return self.tData[name]
    
    def removeDeletedStreetInfo(self, streetId, deletedLineIds):
        for key in self.tData:
            self.tData[key].removeStreetInfo(streetId, deletedLineIds)

class LineSegment(object):
    def __init__(self, point1, point2, streetId):
#         if point1 == point2:
#             raise Exception("line init, same point. x:%s y:%s"%(point1.x, point1.y))
        
        self.__initPoint(point1, point2)
        self.name = self.buildLineName(point1, point2)
        
        self.ea = self.p1.y - self.p2.y
        self.eb = self.p2.x - self.p1.x
        self.ec = self.p1.x*self.p2.y - self.p2.x*self.p1.y

        self.intersections = []
        self.streetIds = {streetId}
        
        self.p1.addLineId(self.name)
        self.p1.addStreetId(streetId)
        
        self.p2.addLineId(self.name)
        self.p2.addStreetId(streetId)
        
    def __eq__(self, l):
        if(l == None):
            return False
        
        return self.name == l.name
    
    def __hash__(self):
        return hash(self.name)
        
    def addStreetId(self, streetId):
        self.streetIds.add(streetId)
        self.p1.addStreetId(streetId)
        self.p2.addStreetId(streetId)
        
    def removeStreedId(self, sId):
        if sId in self.streetIds:
            self.streetIds.remove(sId)
            
        deletedIntersecPointNames = set()    
        for oneInterPoint in self.intersections:
            if len(oneInterPoint.streetIds) < 2 or len(oneInterPoint.lineIds) < 2:
                deletedIntersecPointNames.add(oneInterPoint)
                oneInterPoint.clearLineStrId()
                
        for oneDeleteInterPoint in deletedIntersecPointNames:
            self.intersections.remove(oneDeleteInterPoint)    

    def __getInterSectionPoint(self, l2, pointsCollection):
        d = self.ea*l2.eb - l2.ea*self.eb
        
        if d == 0:
            return None
        
        if(self.eb*l2.ec - l2.eb*self.ec == 0):
            x = 0
        else:
            x = (self.eb*l2.ec - l2.eb*self.ec)/d
            
        if(l2.ea*self.ec - self.ea*l2.ec == 0):
            y = 0
        else:
            y = (l2.ea*self.ec - self.ea*l2.ec)/d           

        p = pointsCollection.getPoint(x, y)
        return p

    def isPointBetweenLine(self, p):
        return InBetween(p.x, self.p1.x, self.p2.x) and InBetween(
            p.y, self.p1.y, self.p2.y)

    def addIntersectionPoint(self, l, pointsCollection):
        if l == self:
            return
              
        p = self.__getInterSectionPoint(l, pointsCollection)
        
        if p == None:

            self.addOverLapLineIntersection(l)
            return
        elif not (self.isPointBetweenLine(p) and l.isPointBetweenLine(p)):
            return
        
        self.addOneIntersection(p)
        
    def addOverLapLineIntersection(self, l):    
        if "{0:.2f}".format(self.p1.x) == "{0:.2f}".format(self.p2.x):
            if "{0:.2f}".format(l.p1.x) == "{0:.2f}".format(l.p2.x) \
            and "{0:.2f}".format(l.p1.x)== "{0:.2f}".format(self.p2.x):
                self.addOneIntersection(l.p1)
                self.addOneIntersection(l.p2)
            return
        
        if "{0:.2f}".format(self.p1.y) == "{0:.2f}".format(self.p2.y):
            if "{0:.2f}".format(l.p1.y) == "{0:.2f}".format(l.p2.y) \
            and "{0:.2f}".format(l.p1.y)== "{0:.2f}".format(self.p2.y):
                self.addOneIntersection(l.p1)
                self.addOneIntersection(l.p2)
            return
        
        if ("{0:.2f}".format(l.p1.y) == "{0:.2f}".format(l.p2.y))  \
        or ("{0:.2f}".format(l.p1.x) == "{0:.2f}".format(l.p2.x)):
            return
        
        k = (self.p2.y-self.p1.y)/(self.p2.x-self.p1.x)
        k1 = (l.p2.y-l.p1.y)/(l.p2.x-l.p1.x)
        
        if ("{0:.2f}".format(k) != "{0:.2f}".format(k1)):
            return
        
        if l.p1 != self.p1:
            if (l.p1.x == self.p1.x):
                return
            
            k2 = (l.p1.y-self.p1.y)/(l.p1.x-self.p1.x)
            if ("{0:.2f}".format(k1) == "{0:.2f}".format(k2)):
                self.addOneIntersection(l.p1)
                self.addOneIntersection(l.p2)
            return
        
        if l.p1 != self.p2:
            if (l.p1.x == self.p2.x):
                return            
            
            k2 = (l.p1.y-self.p2.y)/(l.p1.x-self.p2.x)
            if ("{0:.2f}".format(k1) == "{0:.2f}".format(k2)):
                self.addOneIntersection(l.p1)
                self.addOneIntersection(l.p2)
            return
        
    def addOneIntersection(self, p):
        if not self.isPointBetweenLine(p):
            return
        
        p.addLineId(self.name)
        p.addStreetIds(self.streetIds)
        
        for onePoint in self.intersections:
            if onePoint == p:
                return

        self.intersections.append(p)
        self.sortIntersections()
    
    def sortIntersections(self):
        if len(self.intersections) == 1:
            return
        
        if "{0:.2f}".format(self.intersections[0].x) == "{0:.2f}".format(self.intersections[1].x):
            self.intersections.sort(key=lambda obj:obj.y)
            return
        
        self.intersections.sort(key=lambda obj:obj.x)    
        
    def __initPoint(self, p1, p2):
        if p1.x < p2.x:
            self.p1 = p1
            self.p2 = p2
            return
        elif p1.x > p2.x:
            self.p1 = p2
            self.p2 = p1
            return

        if p1.y < p2.y:
            self.p1 = p1
            self.p2 = p2
            return
        elif p1.y > p2.y:
            self.p1 = p2
            self.p2 = p1
            return

        self.p1 = p1
        self.p2 = p2     

    @staticmethod
    def buildLineName(p1, p2):
        if p1.x < p2.x:
            return "(" + p1.name + "_" + p2.name + ")"
        elif p1.x > p2.x:
            return "(" + p2.name + "_" + p1.name + ")"

        if p1.y < p2.y:
            return "(" + p1.name + "_" + p2.name + ")"
        elif p1.y > p2.y:
            return "(" + p2.name + "_" + p1.name + ")"

        return "(" + p1.name + "_" + p2.name + ")"

    def __str__(self):
        info = "[" + self.name + ", sIds:" + str(self.streetIds) + ", inters(" 
        
        sectionInfo = ""
        for point in self.intersections:
            sectionInfo=sectionInfo+point.name
        
        return info + sectionInfo + ")]"
    
    __repr__ = __str__
    
    
class LineSegmentCollection(GetData):
    def __init__(self):
        GetData.__init__(self)

    def getLine(self, p1, p2, sId):
        name = LineSegment.buildLineName(p1,p2)
        if name in self.tData:
            return self.tData[name]
        
        line = LineSegment(p1, p2, sId)
        self.tData[name] = line
        return self.tData[name] 
 
    def removeLineBySName(self, sId, deletedLineNames):
        for deleteOneKey in deletedLineNames:
            self.tData.pop(deleteOneKey.name) 
            
        for key in self.tData:
            self.tData[key].removeStreedId(sId)

    def getDeletedLine(self, sId):
        deletedLines = set()
        for key in self.tData:
            oneLine = self.tData[key]
            if len(oneLine.streetIds) == 1 and (sId in oneLine.streetIds):
                deletedLines.add(oneLine) 
        
        return  deletedLines
            
def testLine(line1, line2, pointsCollection):
    line1.addIntersectionPoint(line2, pointsCollection)
    print("line1:" + str(line1))
    print("p1:" + str(line1.p1))
    print("p2:" + str(line1.p2))
    print("intersections:" + str(line1.intersections))
    
    line2.addIntersectionPoint(line1, pointsCollection)
    print("line2:" + str(line2))
    print("p1:" + str(line2.p1))
    print("p2:" + str(line2.p2))
    print("intersections:" + str(line2.intersections))

if __name__ == '__main__':
    
    pointsCollection = PointCollection()
    



    
    
    
    
