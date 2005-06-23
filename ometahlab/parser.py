#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Test module for Open Metaheuristic, main file
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# Started 06/2005
#
#  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
#  for difficult optimization.
#   
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

try:
    import psyco
    psyco.full()
except:
    pass

import string


class XMLParser:
    """ to parse the XML output """

    __currentNode__ = None
    __pointsList__ = None
    __header__ = None
    
    
    def __init__(self, xmlFile, interf):
        """ constructor, xmlFile is the file object of the XML output """
        self.__xmlFile = xmlFile
        self.__interface = interf
        self.__class = 'diversification'

    def readXml(self):
        from xml.dom.minidom import parse
        self.doc = parse(self.__xmlFile)

    def __getRootElement(self):
        """ returns the rool element of the XML tree """
        if self.__currentNode__ == None:
            self.__currentNode__ = self.doc.documentElement
        return self.__currentNode__

    def __getText(self, node):
        return node.childNodes[0].nodeValue

    def __getListFloat(self, nodes):
        li = []
        for n in nodes:
            li.append(float(self.__getText(n)))
        return li

    def __getSplitListFloat(self, domObject):
        """ from a string of coordinates separated with blkspace,
        return the splitted string as a list of float """
        slist = self.__getText(domObject)
        slist = string.split(slist)
        return [float(x) for x in slist]

    def __getSplitCommaListFloat(self, domObject):
        """ from a string of coordinates separated with blkspace,
        return the splitted string as a list of float """
        slist = self.__getText(domObject)
        slist = string.split(slist, sep=',')
        return [float(x) for x in slist]
    
    
    def getPoints(self):
        """ returns a list of Points """
        if self.__pointsList__ != None:
            return self.__pointsList__
        self.__pointsList__ = []


    
        optimization = self.__getRootElement().getElementsByTagName('optimization')[0]
        for iteration in optimization.getElementsByTagName('iteration'):
            for step in iteration.getElementsByTagName('step'):
                if step.getAttribute('class') == self.__class:
                    for sample in step.getElementsByTagName('sample'):
                        for point in sample.getElementsByTagName('point'):
                            if point.nodeType == point.ELEMENT_NODE:
                                p = Point()
                                try:
                                    p.value = float(self.__getText(point.getElementsByTagName('values')[0]))
                                    p.coords = self.__getSplitListFloat(point.getElementsByTagName('solution')[0])                                    
                                except:
                                    self.__interface.log('ERROR : XML element missing [XMLParser.getPoints]\n')
                                self.__pointsList__.append(p)
        # give index for each pt, used to get the #FES to reach a success
        #! SEE ometahtest.__calculSuccessRates()
        """
        count = 0
        for p in self.__pointsList__:
            if count == 0:
                count = 1
            else:
                count = 0
            p.index = count
            print p.index,
        """
        return self.__pointsList__
    
    def getHeader(self):
        """ creates a Header object for informations in XML header """
        if self.__header__ != None:
            return self.__header__
        self.__header__ = Header()
        try:
            probl = self.__getRootElement().getElementsByTagName('problem')[0]
            self.__header__.problem.key = self.__getText(probl.getElementsByTagName('key')[0])
            self.__header__.problem.name = self.__getText(probl.getElementsByTagName('name')[0])
            self.__header__.problem.description = self.__getText(probl.getElementsByTagName('description')[0])
            self.__header__.problem.dimension = int(self.__getText(probl.getElementsByTagName('dimension')[0]))
            self.__header__.problem.reference = self.__getText(probl.getElementsByTagName('reference')[0])
            self.__header__.problem.accuracy = float(self.__getText(probl.getElementsByTagName('accuracy')[0]))
        except:
            self.__interface.log('ERROR: XML exploration error for <problem> simple values [XMLParser.getHeader]')

        try:
            optimums = probl.getElementsByTagName('optimums')[0]
            for point in optimums.getElementsByTagName('point'):
                if point.nodeType == point.ELEMENT_NODE:
                    p = Point()
                    p.value = float(self.__getText(point.getElementsByTagName('values')[0]))
                    p.coords = self.__getSplitListFloat(point.getElementsByTagName('solution')[0])
                    self.__header__.problem.optimum.append(p)
            bounds = probl.getElementsByTagName('bounds')[0]
            mini = bounds.getElementsByTagName('minimums')[0]
            for point in mini.getElementsByTagName('point'):
                if point.nodeType == point.ELEMENT_NODE:
                    p = Point()
                    p.coords = self.__getSplitListFloat(point.getElementsByTagName('solution')[0])
                    self.__header__.problem.min_bound.append(p)
            maxi = bounds.getElementsByTagName('maximums')[0]        
            for point in maxi.getElementsByTagName('point'):
                if point.nodeType == point.ELEMENT_NODE:
                    p = Point()
                    p.coords = self.__getSplitListFloat(point.getElementsByTagName('solution')[0])
                    self.__header__.problem.max_bound.append(p)

        except:
            self.__interface.log('ERROR : XML exploration error for <problem> [XMLParser.getHeader]\n')

        try:
            metah = self.__getRootElement().getElementsByTagName('metaheuristic')[0]
            self.__header__.metah.key = self.__getText(metah.getElementsByTagName('key')[0])
            self.__header__.metah.name = self.__getText(metah.getElementsByTagName('name')[0])
            self.__header__.metah.family = self.__getText(metah.getElementsByTagName('family')[0])
            self.__header__.metah.acronym = self.__getText(metah.getElementsByTagName('accronym')[0])
            self.__header__.metah.description = self.__getText(metah.getElementsByTagName('description')[0])
            self.__header__.metah.reference = self.__getText(metah.getElementsByTagName('reference')[0])
        except:
            self.__interface.log('ERROR : XML exploration error for <metaheuristic> [XMLParser.getHeader]\n')
        # ADD CODE TO GET PARAMETERS
        try:
            params = self.__getRootElement().getElementsByTagName('parameters')[0]
            for p in params.getElementsByTagName('parameter'):
                name = self.__getText(p.getElementsByTagName('name')[0])
                if name == "sampleSize":
                    self.__header__.parameters.sampleSize = self.__getText(p.getElementsByTagName('value')[0])
                elif name == "iterationsMaxNumber":
                    self.__header__.parameters.maxIterations = self.__getText(p.getElementsByTagName('value')[0])
                elif name == "evaluationsMaxNumber":
                    self.__header__.parameters.maxEvaluations = self.__getText(p.getElementsByTagName('value')[0])
                elif name == "valueThreshold":
                    self.__header__.parameters.treshold = self.__getText(p.getElementsByTagName('value')[0])
                elif name == "randomSeed":
                    self.__header__.parameters.randomSeed = self.__getText(p.getElementsByTagName('value')[0])
                else:
                    self.__interface.log('ERROR : Unknown parameter [XMLParser.getHeader]\n')                                                                       
        except:
            self.__interface.log('ERROR : Unexpected error [XMLParser.getHeader]\n')                                                                       
        return self.__header__


class Problem:
    """ descriptive informations of a problem
    only public attributes, descriptive class..."""

    key = None
    name = None
    description = None
    formula = None
    dimension = None
    optimum = []   # list of Point
    min_bound = [] # idem
    max_bound = [] # idem
    reference = None
    accuracy = 0.0
    
    def __init__(self):
        pass

class Metaheuristic:
    """ descriptive informations of a metaheuristic
    only public attributes, descriptive class..."""

    key = None
    name = None
    family = None
    acronym = None
    description = None
    reference = None
    
    def __init__(self):
        pass


class Parameters:
    """ the set of parameters specified into <parameters>
    elements in XML output """

    sampleSize = None
    maxIterations = None
    maxEvaluations = None
    treshold = None
    randomSeed = None

    def __init__(self):
        pass


class Header:
    """ for additional informations in XML file """

    problem = Problem()
    metah = Metaheuristic()
    parameters = Parameters()

    def __init__(self):
        pass
    
    def toString(self):
        """ return a long string with long description """
        m = self.metah
        p = self.problem
        par = self.parameters
        
        s = '\n\nMETAHEURISTIC:\n\tName: %s\n\tKey: %s\n\tFamily: %s\n\tAcronym: %s\n' % \
            (m.name, m.key,  m.family, m.acronym)
        s += '\tDescription: %s\n' % \
             (m.description)
        s += 'PROBLEM:\n\tName: %s\n\tKey: %s\n\tDescription: %s\n\tDimension: %s\n\n' % \
             (p.name, p.key, p.description, p.dimension)
        s+= 'PARAMETERS:\n\tSample size:%s\n\tMax iterations: %s\n\tMax evaluations: %s\n\tSeed: %s\n' % \
            (par.sampleSize, par.maxIterations, par.maxEvaluations, par.randomSeed)
        return s

    def getTitle(self):
        """ return a short string for titling files """
        s = "%s_%s_" % (self.problem.name, self.metah.acronym)
        return s

class Point:
    """ a point has a set of coordinates, and a value """

    coords = None
    value = None
    error = None
    index = 0

    def __init__(self):
        pass

if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
