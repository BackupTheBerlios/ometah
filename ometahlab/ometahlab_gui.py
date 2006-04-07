#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab GUI is a user interface for ometahtests and ometahstats
#
#  Author : Johann Dréo <nojhan@gmail.com>
#  $Id: ometahlab_gui.py,v 1.2 2006/04/07 17:58:27 nojhan Exp $
#  Copyright : Free Software Fundation
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

from Tkinter import *

import ometahtest
import ometahstats


class MenuBar(Frame):
    """Menu bar"""
    def __init__(self, boss =None):
        
        Frame.__init__(self, borderwidth =2, relief =GROOVE)

        # add the main menu
        self.addMenu("Lab", {'About...':boss.about,
                             'Quit':boss.quit} )

        
    def addMenu(self,name,dicoLC):
        menu = Menubutton(self,text=name)
        menu.pack(side =LEFT, padx =5)
        sub = Menu(menu)
        for label in dicoLC:
            sub.add_command(label=label,underline=0,command=dicoLC[label])
            menu.configure(menu=sub)


class Tests(Frame):
    """The widget handling the tests"""
    def __init__(self,boss, core, menu=True):
        Frame.__init__(self, borderwidth =2, relief =GROOVE)
        self.core=core
        
        # list of tests instances
        self.tests = []
        
        ## GUI
        
        # menus
        if menu:
            self.core.menuBar.addMenu('Tests',{'Launch tests':self.start})
        
        # path to the ometah binary
        fraOptions = Frame(self)
        
        Label(fraOptions,text="Binary path:").grid(row=0,column=1,sticky="e")
        self.varBinary = StringVar()
        Entry(fraOptions,textvariable=self.varBinary,bg='white').grid(row=0,column=2)
        Button(fraOptions,text="Browse...",command=self.browseBinary,state='disabled').grid(row=0,column=3)
        self.varBinary.set("../ometah/ometah")
        
        Label(fraOptions,text="Runs nb:").grid(row=1,column=1,sticky="e")
        self.varRuns = IntVar()
        Entry(fraOptions,textvariable=self.varRuns,bg='white').grid(row=1,column=2)
        self.varRuns.set(100)
        
        fraOptions.pack(padx=3,pady=3)
        
        # test add line
        fraTestAdd = Frame(self)
        
        Label(fraTestAdd,text="Test:").pack(side=LEFT)
        self.varTestAdd = StringVar()
        self.entTest = Entry(fraTestAdd,textvariable=self.varTestAdd,bg='white')
        self.entTest.pack(side=LEFT,expand=YES,fill=X)
        
        Button(fraTestAdd,text="Build...",command=self.buildTest,state='disabled').pack(side=LEFT)
        Button(fraTestAdd,text="Add",command=self.addTest).pack(side=LEFT)
        
        fraTestAdd.pack(fill=X,padx=3,pady=3)
        
        # tests list
        fraTestsList = Frame(self)
        
        self.testsList = Listbox(fraTestsList,height=7)
        self.testsList.pack(side=LEFT,fill=BOTH,anchor='w',expand=YES)
        self.testsList.bind("<<ListboxSelect>>",self.updateEntryTest)
        self.entTest.bind("<Return>",self.addTest)
        
        self.testsScroll = Scrollbar(fraTestsList,command=self.testsList.yview)
        self.testsScroll.pack(side=LEFT,fill=Y,anchor='w')

        self.testsList.config(yscrollcommand=self.testsScroll.set)
        self.testsScroll.config(command=self.testsList.yview)
        
        self.fraTestsListCommands = Frame(fraTestsList)
        
        Button(self.fraTestsListCommands,text="Delete",command=self.delTest).pack(side=TOP)
        
        self.fraTestsListCommands.pack(side=LEFT,fill=Y)
        
        fraTestsList.pack(fill=BOTH,expand=YES,padx=3,pady=3)

        
        # bottom stuff
        fraCommands = Frame(self)
        
        Button(fraCommands,text="Launch tests",command=self.start).pack(side=RIGHT)
        
        self.labelState = Label(fraCommands,text="...")
        self.labelState.pack(side=RIGHT,expand=YES,fill=X)
        
        fraCommands.pack()
        
        
    def start(self):
        # build tests
        for i in self.testsList.get(0,END):
            self.tests += [ ometahtest.Test( self.varBinary.get(), i, self.varRuns.get() ) ]
        
        # launch
        for i in self.tests:
            i.start()
            
        self.labelState.configure(text= "Launched %i test" % ( len(self.testsList.get(0,END)) ) )
        
    def addTest(self):
        self.testsList.insert( END, self.varTestAdd.get() )
        self.varTestAdd.set("")
        
        self.labelState.configure(text= "%i tests" % ( len(self.testsList.get(0,END) ) ) ) 
        
    def delTest(self):
        curIndex = int( self.testsList.curselection()[0] )
        self.testsList.delete(curIndex,curIndex)
        
    def browseBinary(self):
        pass
        
    def buildTest(self):
        pass
        
    def updateEntryTest(self,event=None):
        curIndex = int( self.testsList.curselection()[0] )
        self.varTestAdd.set( self.testsList.get(curIndex) )
        

class Stats(Frame):
    """The widget handling the stats"""
    def __init__(self,boss, core, menu=True):
        Frame.__init__(self, borderwidth =2, relief =GROOVE)
        self.core=core
        
        # 
        self.plugins = ometahstats.PluginManager('.','plugins')
        
        # menus
        if menu:
            self.core.menuBar.addMenu('Stats',{'Process':self.start})
        
        # test add line
        fraTestAdd = Frame(self)
        
        Label(fraTestAdd,text="Results path:").pack(side=LEFT)
        self.varTestAdd = StringVar()
        self.entTest = Entry(fraTestAdd,textvariable=self.varTestAdd,bg='white')
        self.entTest.pack(side=LEFT,expand=YES,fill=X)
        
        Button(fraTestAdd,text="Browse...",command=self.browseTest,state='disabled').pack(side=LEFT)
        Button(fraTestAdd,text="Add",command=self.addTest).pack(side=LEFT)
        
        fraTestAdd.pack(fill=X,padx=3,pady=3)
        
        # tests list
        fraTestsList = Frame(self)
        
        self.testsList = Listbox(fraTestsList,height=7)
        self.testsList.pack(side=LEFT,fill=BOTH,anchor='w',expand=YES)
        self.testsList.bind("<<ListboxSelect>>",self.updateEntryTest)
        self.entTest.bind("<Return>",self.addTest)
        
        self.testsScroll = Scrollbar(fraTestsList,command=self.testsList.yview)
        self.testsScroll.pack(side=LEFT,fill=Y,anchor='w')

        self.testsList.config(yscrollcommand=self.testsScroll.set)
        self.testsScroll.config(command=self.testsList.yview)
        
        self.fraTestsListCommands = Frame(fraTestsList)
        
        Button(self.fraTestsListCommands,text="Delete",command=self.delTest).pack(side=TOP)
        
        self.fraTestsListCommands.pack(side=LEFT,fill=Y)
        
        fraTestsList.pack(fill=BOTH,expand=YES,padx=3,pady=3)
        
        # plugin list
        
        Label(self,text="Available plugins:").pack()
        
        fraPluginList = Frame(self)
                
        self.checkPlugins = {}
        
        self.plugins.available.sort()
        for p in self.plugins.available:
            self.checkPlugins[p]=IntVar()
            Checkbutton(self, text=p.replace("_"," "), variable = self.checkPlugins[p] ).pack(anchor='w')
            
        fraPluginList.pack()
        
        # type entry
        fraFileType = Frame(self)
        
        Label(fraFileType,text="File type:").pack(side=LEFT)
        self.varFileType = StringVar()
        self.entFileType = Entry(fraFileType,textvariable=self.varFileType,bg='white')
        self.varFileType.set('html')
        self.entFileType.pack(side=LEFT,expand=YES,fill=X)
        
        fraFileType.pack()
        
        # bottom stuff
        fraCommands = Frame(self)
        
        Button(fraCommands,text="Process",command=self.start).pack(side=RIGHT)
        
        self.labelState = Label(fraCommands,text="...")
        self.labelState.pack(side=RIGHT,expand=YES,fill=X)
        
        fraCommands.pack()
        

    def updateEntryTest(self,event=None):
        curIndex = int( self.testsList.curselection()[0] )
        self.varTestAdd.set( self.testsList.get(curIndex) )

    def addTest(self):
        self.testsList.insert( END, self.varTestAdd.get() )
        self.varTestAdd.set("")
        
    def browseTest(self):
        pass
        
    def delTest(self):
        curIndex = int( self.testsList.curselection()[0] )
        self.testsList.delete(curIndex,curIndex)
    
    def start(self):
        # get the asked plugin list
        askedPlugins = []
        for p in self.checkPlugins:
                if self.checkPlugins[p].get() == 1:
                    askedPlugins += [p]
                    
        # get the asked path list
        askedPaths = []
        for p in self.testsList.get(0,END):
            askedPaths += [p]
            
        # launch it
        ometahstats.process(askedPaths,askedPlugins,self.varFileType.get())
        
        self.labelState.configure(text= "Processed %i path, produced %i files" % ( len(self.testsList.get(0,END)), len(askedPlugins) ) )
        
        
class Gui(Frame):
    """The main widget"""
    def __init__(self):
        Frame.__init__(self)
        
        self.master.title("oMetah Lab GUI")
        
        # menu
        self.menuBar = MenuBar(self)
        self.menuBar.pack(side=TOP, expand=NO, fill=X)
        
        # tabs
        self.fraButtons = Frame(self.master, pady=5)
        
        self.butTests = Button(self.fraButtons,text="Tests",command=self.showTests,state='disabled')
        self.butTests.pack(side=LEFT)
        self.butStats = Button(self.fraButtons,text="Stats",command=self.showStats)
        self.butStats.pack(side=LEFT)
        
        self.fraButtons.pack(side=TOP,expand=NO,fill=X)
        
        # tests frame
        self.fraTests = Tests(self,self,menu=True)
        self.fraTests.pack(side=TOP, expand=YES, fill=BOTH)
        
        self.fraStats = Stats(self,self,menu=True)
        self.fraStats.pack(side=TOP, expand=YES, fill=BOTH)
        self.fraStats.destroy()
        
        
    def showTests(self):
        self.butTests.configure(state='disabled')
        self.fraStats.destroy()
        self.fraTests = Tests(self,self,menu=False)
        self.fraTests.pack(side=TOP, expand=YES, fill=BOTH) 
        self.butStats.configure(state='active')
        
    def showStats(self):
        self.butStats.configure(state='disabled')
        self.fraTests.destroy()
        self.fraStats = Stats(self,self,menu=False)
        self.fraStats.pack(side=TOP, expand=YES, fill=BOTH)
        self.butTests.configure(state='active')
        
    def about(self):
        self.tk.call('tk_messageBox',
                     '-title','About oMetah Lab GUI',
                    '-message',"""This is a GUI for oMetah Lab, a tool for metaheuristic conception.

Authors:
* Johann "nojhan" Dréo <nojhan@gmail.com>

License: LGPL""")
        
    
    def quit(self):
        self.destroy()
        Frame.quit(self)


if __name__ == '__main__':
    Gui().mainloop()
