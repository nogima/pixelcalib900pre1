import xml.etree.ElementTree as ET


def RPNumber(name):
    mylist = name.split('_')
    n=0
    n+=int(mylist[5])
    if mylist[2]== 'Right':
        n+=100
    if mylist[1] == '220':
        n+=20
    return n


class RPIDAndDistFromXML:

    def __init__(self,filename='RP_Dist_Beam_Cent.xml',verbose=False):

        tree = ET.parse(filename)
        root = tree.getroot()
    
        ns = {'DDLS': 'http://www.cern.ch/cms/DDL'}
        self.mydict = {}
        self.vnames=[]
        self.vdist=[]
        self.vrpn=[]

        for child in root:
            for rp in child.findall('DDLS:Constant',ns):
                name= rp.get('name')
                textvalue=rp.get('value')
                dist= float(textvalue.split('*')[0])
                unit= textvalue.split('*')[1]
                rpn=RPNumber(name)
                if(verbose): print name, dist, unit,rpn
                self.mydict[rpn]=dist
                self.vnames+=[name]
                self.vrpn+=[rpn]
                self.vdist+=[dist]

        if(verbose):
            print self.mydict
        


