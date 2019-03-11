
        
class nodevar():
    def __init__(self,id):
        
        self._id=id
        self._x=0
        self._y=0
        self._z=0
        self._type = 0 
        self._nb_a=0
        self.arc1=-1
        self.arc2=-1
        self.arc3=-1
        self.arc4=-1
        
        
    def addId(self,id):
        self._nb_a +=1
    
        if self._nb_a== 1:
            self.arc1=id
        if self._nb_a == 2:
            self.arc2=id
        if self._nb_a == 3:
            self.arc3=id
        if self._nb_a == 4:
            self.arc4=id