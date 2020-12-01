

def InBetween(value, value1, value2):
    if value1 > value2:
        return value >= value2 and value <= value1
    else :
        return value >= value1 and value <= value2

def PrintOP(obj):
    print ('\n'.join(['%s:%s' % item for item in obj.__dict__.items()]))

class GetData(object):
    def __init__(self):
        self.tData = {}
 
    def SearchElm(self, name):
        if name in self.tData:
            return self.tData[name]
        else:
            return None
 
    def addElm(self, element):
        print("GetData add new element, " + element.name)
        self.tData[element.name] = element
        
    def getData(self):
        return self.tData

    def __str__(self):
        info = ""
        for i in self.tData:
            info = info + " " + str(self.tData[i]) + "\n"
        
        return info
            
    __repr__ = __str__


if __name__ == '__main__':
    pass