

from LineInfo import *
from BasicDataInfo import *
from StreetInfo import *

class Vertices(object):
    def __init__(self, p, id):
        self.p = p
        self.name = p.name
        self.displayName = "(" + "{0:.2f}".format(self.p.x) + "," + "{0:.2f}".format(self.p.y) + ")"
        self.id = id
        
    def __eq__(self, p): 
        return self.p == p
            
    def __str__(self):
        return self.name
            
    __repr__ = __str__
    
    def __hash__(self):
        return hash(self.p)


class Edges(object):
    def __init__(self, point1, point2):

        self.__initPoint(point1, point2)
        self.name = Edges.buildName(self.p1, self.p2)
    
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
   
    def __eq__(self, l):
        if(l == None):
            return False
        
        return self.name == l.name
    
    def __hash__(self):
        return hash(self.name)
        
    @staticmethod
    def buildName(p1, p2):
        if p1.x < p2.x:
            return p1.name + "_" + p2.name
        elif p1.x > p2.x:
            return p2.name + "_" + p1.name

        if p1.y < p2.y:
            return p1.name + "_" + p2.name
        elif p1.y > p2.y:
            return p2.name + "_" + p1.name

        return p1.name + "_" + p2.name

    def __str__(self):
        return self.name
    
    __repr__ = __str__

class Graph(object):
    def __init__(self):
        self.edges = set()
        self.vertices = {}
        
    def addEdge(self, edge):
        if edge in self.edges:
            return
        
        self.edges.add(edge)
        
        if not edge.p1.name in self.vertices:
            vertex1 = Vertices(edge.p1, len(self.vertices))
            self.vertices[edge.p1.name] = vertex1

        if not edge.p2.name in self.vertices:
            vertex2 = Vertices(edge.p2, len(self.vertices))
            self.vertices[edge.p2.name] = vertex2
            
    def __str__(self):
        output = "(edges:" + str(self.edges) + "\n" + "vertices:" + str(self.displayVertices)
        return output
    
    def clear(self):
        self.edges = set()
        self.vertices = {}

class GraphGen(object):
    def __init__(self, streetCollection, graph):
        self.streetCollection = streetCollection
        self.graph = graph

    def generate(self):
        self.graph.clear()
                 
        for k in self.streetCollection.getData():
            oneStreet = self.streetCollection.getData()[k]           
            
            for oneLine in oneStreet.lines:
                
                if(len(oneLine.intersections) == 0):
                    if(self.isEdgePoint(oneLine.p1) or self.isEdgePoint(oneLine.p2)):
                        oneEdge = Edges(oneLine.p1, oneLine.p2)
                        self.graph.addEdge(oneEdge)
                    continue
                  
                for i in range(len(oneLine.intersections)):
                    oneIntersection = oneLine.intersections[i]
                    
                    if i == 0:
                        if oneLine.p1 != oneIntersection:
                            oneEdge = Edges(oneLine.p1, oneIntersection)
                            self.graph.addEdge(oneEdge)
                        
                    if i == len(oneLine.intersections)-1:
                        if oneIntersection != oneLine.p2:
                            oneEdge = Edges(oneIntersection, oneLine.p2)
                            self.graph.addEdge(oneEdge)
                        
                    if(i + 1 < len(oneLine.intersections)):
                        if oneIntersection != oneLine.intersections[i+1]:
                            oneEdge = Edges(oneIntersection, oneLine.intersections[i+1])
                            self.graph.addEdge(oneEdge)

    
    def isEdgePoint(self, p):
        return len(p.streetIds)> 1 and len(p.lineIds) > 1   


class GraphPtr(object):
    def __init__(self, graph):
        self.graph = graph
    
    def OutputGraph(self):

        vertices = sorted(self.graph.vertices.items(), key = lambda x : x[1].id)

        output = "V " + str(len(self.graph.vertices)) + "\n"
        output += "E {"
        i = 0
        for edge in self.graph.edges:
            vertex1 = self.graph.vertices[edge.p1.name]
            vertex2 = self.graph.vertices[edge.p2.name]
            output += "<" + str(vertex1.id) + "," + str(vertex2.id) + ">"
            
            if(i != len(self.graph.edges) -1):
                output += ","
            i+=1
        
        output += "}"
        
        return output


if __name__ == '__main__':
    pass