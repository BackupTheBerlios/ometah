#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Test module for Open Metaheuristic, main file
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# Started 06/2005
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  This module contains XMLParser class, to get data from ometah XML output,
#  and definitions of structure to record this data : Header, Problem,
#  Metaheuristic, Parameters, and Point classes.
#
###
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
    """ Parse the XML output of ometah, creating a list of points,
    and saves additional informations in class Header."""

    __currentNode__ = None
    __pointsList__ = None
    __header__ = None
    
    
    def __init__(self, xmlFile, interf):
        """ Constructor, xmlFile is the file object of the XML output, interface is needed to log errors. """
        self.__xmlFile = xmlFile
        self.__interface = interf
        self.__class = 'diversification'

    def readXml(self):
        """ Parse the XML document. """
        from xml.dom.minidom import parse
        self.doc = parse(self.__xmlFile)

    def __getRootElement(self):
        """ Returns the root element of the XML tree """
        if self.__currentNode__ == None:
            self.__currentNode__ = self.doc.documentElement
        return self.__currentNode__

    def __getText(self, node):
        """ Return the node value, as a string."""
        return node.childNodes[0].nodeValue

    def __getListFloat(self, nodes):
        """ Returns the list of float elements from nodes list."""
        li = []
        for n in nodes:
            li.append(float(self.__getText(n)))
        return li

    def __getSplitListFloat(self, domObject):
        """ From a string of coordinates separated with blank space, returns the splitted string as a list of float. """
        slist = self.__getText(domObject)
        slist = string.split(slist)
        return [float(x) for x in slist]

    
    def getPoints(self):
        """ Returns the list of points given in XML output, as Point objects. """
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
        # give an index to each Point
        c = 1
        for p in self.__pointsList__:
            p.index = c
            c += 1
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
    """ Descriptive informations of a problem. """

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
        """ Void constructor."""
        pass

class Metaheuristic:
    """ Descriptive informations of a metaheuristic. """

    key = None
    name = None
    family = None
    acronym = None
    description = None
    reference = None
    
    def __init__(self):
        """ Void constructor."""
        pass


class Parameters:
    """ The set of parameters specified into <parameters> elemens in XML output """

    sampleSize = None
    maxIterations = None
    maxEvaluations = None
    treshold = None
    randomSeed = None

    def __init__(self):
        pass


class Header:
    """ Additional informations in XML file concerning problem, metaheuristic, and parameters."""

    problem = Problem()
    metah = Metaheuristic()
    parameters = Parameters()

    def __init__(self):
        """ Void constructor."""
        pass
    

class Point:
    """ A point has a set of coordinates, a value, an error relative to the problem's optimum, and an index. """

    coords = None
    value = None
    error = None
    index = 0

    def __init__(self):
        """ Point constructor. """
        pass

if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
