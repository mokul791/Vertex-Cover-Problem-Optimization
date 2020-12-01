import sys
import re

from IntersectionCalc import line,point,intersection,Segment,fracRound,is_same_Line

#Global_Variables
RoadNameList=[]
RoadsDict={}
invalid_input_from_user = []

#Validity Checker
def validIp(i):
    if re.match(r"^a\s+(\"(([\w]+)\s?)+\")\s+(((\(-?\d+,-?\d+\))\s?)\s?)+$", i):
        add_Streets(i)
    elif re.match(r"^c\s+(\"(([\w]+)\s?)+\")\s+(((\(-?\d+,-?\d+\))\s?)\s?)+$", i):
        change_Streets(i)
    elif re.match(r"^r\s+(\"(([\w]+)\s?)+\")\s?$", i):
        remove_Streets(i)
    elif re.match(r"^g\s?$", i):
        GenGraph()
    elif re.match(r"\n",i):
        print ('Error: Please input road data correctly.')
    else:
        print ('Error: Input data format unrecognised.')
#For User Input
def StreetName(data):
    data = re.search('("(.*?)"|\'(.*?)\')', data)
    name = data.group(0)
    return name[1:len(name) - 1]

def StreetCord(data):
    find_coordinate = re.findall(r'(\(.*?\))', data)
    return find_coordinate
#For processing
def add_Streets(data):
    if str(StreetName(data).lower().strip()) in (RoadsDict.keys()):
        print ('Error: Road already exists')
    else:
        get_street_coordinates(data)

def get_street_coordinates(data):
    RoadsDict[str(StreetName(data)).lower().strip()] = StreetCord(data)
    return RoadsDict

def change_Streets(data):
    if str(StreetName(data).lower().strip()) in (RoadsDict.keys()):
        RoadsDict[str(StreetName(data)).lower().strip()] = StreetCord(data)
    else:
        print ('Error: Can not update. Road not found.')
def remove_Streets(data):
    if str(StreetName(data).lower().strip()) in (RoadsDict.keys()):
        del RoadsDict[str(StreetName(data)).lower().strip()]
    else:
        print ('Error: Road Name is not found in dictionary. Can not remove: ', data)
#For graph

def GenGraph():
    lines_list_roads = LineCoordinates()
    resultVertices, resultVerticesList = CalcIntersection(lines_list_roads)
    resultVertices = set(resultVertices)
    resultVertices_List = list(resultVertices)

    GraphVertices(resultVertices_List)
    verticesList = GraphEdge(resultVertices_List, ListMatrix(lines_list_roads),resultVerticesList)
    GraphLastEdge(verticesList)

def GraphVertices(data):
    x = 0
    strig = " "
    for j in data:
        k = CorString(j)
        x=x+1
        stg = " {}: {} ".format(x, k)
        strig = strig + "\n" + stg

    print "V = {" + strig + "\n}"


def GraphLastEdge(data):
    Ed = []
    newE = " "
    for edge in data:
        data = tuple(edge)
        Ed.append(data)
    for items in Ed:
        if items == Ed[-1]:
            newE = newE + "\n" + str(items)
        else:
            newE = newE + "\n" + str(items) + ","
    print 'E = {', newE.replace('(', '<').replace(')', '>').replace(' ', ''), '\n}'

def CorString(point_string):
    coordinate = re.split(',', point_string)
    return point(coordinate[0], coordinate[1])


def GraphEdge(resultVertices_List, lines_List, resultVerticesList):
    verticesList = []
    for i in range(len(resultVertices_List) - 1):
        point1 = CorString(resultVertices_List[i])
        for j in range(i + 1, len(resultVertices_List)):
            point2 = CorString(resultVertices_List[j])
            if ValidEdge(point1, point2, lines_List):
                if (resultVerticesList[resultVertices_List[i]] or resultVerticesList[resultVertices_List[j]]):
                    if not CorIntersection(point1, point2, resultVerticesList):
                        verticesList.append([i + 1, j + 1])

    return verticesList

def CorIntersection(point1, point2, resultVerticesList):
    for key, value in resultVerticesList.iteritems():
        if (value):
            point = CorString(key)
            if ((not SameCorIntersection(point, point1)) and (not SameCorIntersection(point, point2))):
                lies_Between = Segment(point1, point2).is_Between(point)
                if lies_Between:
                    return True
    return False
def SameCorIntersection(point1,point2):
    if(point1.x == point2.x and point1.y == point2.y):
        return True
    return False

def ValidEdge(point1, point2, lines_List):
    for line in lines_List:
        lies_Between = Segment(line.src, line.dst).is_Between(point1) and Segment(line.src, line.dst).is_Between(point2)
        if lies_Between:
            return True
    return False


def ListMatrix(road_Lines):
    l_s = []
    for r_s in road_Lines:
        for i in r_s:
            l_s.append(i)
    return l_s

def LineCoordinates():
    lines_list_roads = []

    for key, values in RoadsDict.iteritems():
        lines_list = []
        for p in range(len(values) - 1):
            coordinate_1 = values[p][1:len(values[p]) - 1]
            coordinate_2 = values[p + 1][1:len(values[p + 1]) - 1]
            coordinate_1 = re.split(',', coordinate_1)
            coordinate_2 = re.split(',', coordinate_2)
            li = line(point(coordinate_1[0], coordinate_1[1]), point(coordinate_2[0], coordinate_2[1]))
            lines_list.append(li)
        lines_list_roads.append(lines_list)

    return lines_list_roads

def CalcIntersection(lines_list_roads):
    results = []
    resultVerticesList={}
    for l in range(len(lines_list_roads) - 1):
        lines_List = lines_list_roads[l]
        for m in range(len(lines_List)):
            line = lines_List[m]
            road_List_To_Compare = ListMatrix(lines_list_roads[l + 1:len(lines_list_roads)])
            for n in range(len(road_List_To_Compare)):
                intesection_Line = road_List_To_Compare[n]
                intersect_Point = intersection(line, intesection_Line)

                if (is_same_Line(intesection_Line, line) and (intersect_Point.x == 0 and intersect_Point.y == 0)):
                    intersect_String = str(line.src.x) + ',' + str(line.src.y)
                    results.append(intersect_String)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = True

                    intersect_String = str(line.dst.x) + ',' + str(line.dst.y)
                    results.append(intersect_String)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = True

                    intersect_String = str(intesection_Line.src.x) + ',' + str(intesection_Line.src.y)
                    results.append(intersect_String)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = True

                    intersect_String = str(intesection_Line.dst.x) + ',' + str(intesection_Line.dst.y)
                    results.append(intersect_String)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = True

                if (not (intersect_Point.x == 0 and intersect_Point.y == 0)):
                    intersection_validity = Segment(intesection_Line.src, intesection_Line.dst).is_Between(
                        intersect_Point) and Segment(line.src, line.dst).is_Between(intersect_Point)

                    if intersection_validity:
                        intersect_String = str(intersect_Point.x) + ',' + str(intersect_Point.y)
                        results.append(intersect_String)

                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = True

                        intersect_String = str(line.src.x) + ',' + str(line.src.y)
                        results.append(intersect_String)

                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(line.dst.x) + ',' + str(line.dst.y)
                        results.append(intersect_String)

                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(intesection_Line.src.x) + ',' + str(intesection_Line.src.y)
                        results.append(intersect_String)

                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(intesection_Line.dst.x) + ',' + str(intesection_Line.dst.y)
                        results.append(intersect_String)

                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False
                    else:
                        intersect_String = str(line.src.x) + ',' + str(line.src.y)
                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(line.dst.x) + ',' + str(line.dst.y)
                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(intesection_Line.src.x) + ',' + str(intesection_Line.src.y)
                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False

                        intersect_String = str(intesection_Line.dst.x) + ',' + str(intesection_Line.dst.y)
                        if not resultVerticesList.has_key(intersect_String):
                            resultVerticesList[intersect_String] = False
                else:
                    intersect_String = str(line.src.x) + ',' + str(line.src.y)
                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = False
                    intersect_String = str(line.dst.x) + ',' + str(line.dst.y)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = False
                    intersect_String = str(intesection_Line.src.x) + ',' + str(intesection_Line.src.y)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = False
                    intersect_String = str(intesection_Line.dst.x) + ',' + str(intesection_Line.dst.y)

                    if not resultVerticesList.has_key(intersect_String):
                        resultVerticesList[intersect_String] = False

    return results, resultVerticesList


#main
def main():
    while True:
        line = sys.stdin.readline()
        try:
            if line == '':
                break
            else:
                b = line.strip()
                validIp(b)
        except:
            print("Error,Please give input correctly.")
    sys.exit(0)

if __name__ == '__main__':
    main()
 
