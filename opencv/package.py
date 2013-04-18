# -*- coding: utf-8 -*-

import datatype
import cvtype

class Ident(object):
    def __init__(self, ident = ""):
        self.ident = ident
        
    def className(self):
        return "{0}{1}".format(self.ident[0].upper(), self.ident[1:])
        
    def constant(self):
        """
        >>> Ident("srcPos").constant()
        'SRC_POS'
        >>> Ident("SrcPosF").constant()
        'SRC_POS_F'
        >>> Ident("SrcPos1").constant()
        'SRC_POS_1'
        """
        result = ""
        for c in self.ident:
            if c.isupper() or c.isdigit():
                result += '_'
            result += c.upper()
        return result.strip("_")
        
    def method(self):
        return self.ident
        
    def attribute(self):
        return "m_{0}".format(self.method())
        
    def upper(self):
        return self.ident.upper()
        
    def lower(self):
        return self.ident.lower()
        
    def __str__(self):
        return self.ident
    
class Package(object):
    def __init__(self, ident, major, minor, patch):
        self.ident = Ident(ident)
        self.name = ""
        self.description = ""
        self.methods = []
        self.major = major
        self.minor = minor
        self.patch = patch
        
class Acceptor(object):
    def accept(self, visitor):
        raise NotImplementedError
        
class Compound(object):
    def __init__(self, args):
        self.args = args
        
    def accept(self, visitor):
        visitor.visitCompound(self)
            
class Size(Compound):
    def __init__(self, xArg, yArg):
        self.args = [xArg, yArg]
        
    def create(self):
        xCvData = "{0}".format(self.args[0].ident)
        yCvData = "{0}".format(self.args[1].ident)
        return "cv::Size({0}CvData, {1}CvData)".format(xCvData, yCvData)

class Argument(Acceptor):
    def __init__(self, ident, name, cvType, dataType, description = "",
                 initIn = None, initOut = None):
        self.ident = Ident(ident)
        self.name = name
        self.cvType = cvType
        self.dataType = dataType
        self.description = description
        self.initIn = [] if initIn == None else initIn
        self.initOut = [] if initOut == None else initOut
        
    def copyFrom(self, arg):
        self.ident = arg.ident
        self.name = arg.name
        self.description = arg.description
        self.cvType = arg.cvType
        self.dataType = arg.dataType
        self.initIn = arg.initIn
        self.initOut = arg.initOut
    
class InputArgument(Argument):
    pass

class OutputArgument(Argument):
    def __init__(self, ident, name, cvType, dataType):
        super(OutputArgument, self).__init__(ident, name, cvType, dataType)
        
class Parameter(InputArgument):
    def __init__(self, ident, name, cvType, dataType, inPlace = False,
                 isInit = False, default = None, rules = None):
        super(Parameter, self).__init__(ident, name, cvType, dataType)
        self.inPlace = inPlace
        self.isInit = isInit
        self.default = default
        self.rules = [] if rules == None else rules
    
    def accept(self, visitor):
        visitor.visitParameter(self)     

class NumericParameter(Parameter):
    def __init__(self, ident, name, cvType, dataType, default = None,
                 minValue = None, maxValue = None, step = None, rules = None):
        super(NumericParameter, self).__init__(
            ident, name, cvType, dataType, default = default, rules = rules)
        self.minValue = minValue
        self.maxValue = maxValue
        self.step = step
    
    def accept(self, visitor):
        visitor.visitNumericParameter(self)
    
class EnumParameter(Parameter):
    def __init__(self, ident, name):
        super(EnumParameter, self).__init__(ident, name, cvtype.Int(),
                                            datatype.Enum())
        self.descriptions = []
    
    def accept(self, visitor):
        visitor.visitEnumParameter(self)
        
class Method(object):
    def __init__(self, ident):
        self.ident = Ident(ident)
        self.name = ""
        self.doc = ""
        self.functions = []
        self.options = []
        
class Option(object):
    def __init__(self, ident, name, args = None):
        self.ident = Ident(ident)
        self.name = name
        self.args = [] if args == None else args

class Constant(Argument):
    def __init__(self, ident, cvType, value):
        self.cvType = cvType
        self.value = value
        self.ident = ident
    
    def accept(self, visitor):
        visitor.visitConstant(self)
    
class Input(InputArgument):
    def __init__(self, arg, inPlace = False):
        if arg:
            self.copyFrom(arg)
        self.inPlace = inPlace
    
    def accept(self, visitor):
        visitor.visitInput(self)

class Output(OutputArgument):
    def __init__(self, arg):
        if arg:
            self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitOutput(self)
        
class RefInput(OutputArgument):
    def __init__(self, arg, refArg):
        self.copyFrom(arg)
        self.refArg = refArg
        
    def accept(self, visitor):
        visitor.visitRefInput(self)
        
class Allocation(OutputArgument):
    def __init__(self, arg):
        self.copyFrom(arg)
    
    def accept(self, visitor):
        visitor.visitAllocation(self)

class EnumDescription(object):
    def __init__(self, ident, name, cvIdent = None):
        self.ident = ident
        self.name = name
        if cvIdent:
            self.cvIdent = cvIdent
        else:
            self.cvIdent = "cv::{0}".format(ident)     
    
class ParameterRule(object):
    def check(self, parameter, doc):
        return ""

class OddRule(ParameterRule):
    def check(self, parameter, doc):
        doc.line("if(int(castedValue) % 2 == 0)")
        doc.increaseIndent()
        doc.line((
            'throw runtime::WrongParameterValue(*{0}Parameter, '
            '*this, "Only odd values are allowed");'
            ).format(parameter.ident.attribute()))
        doc.decreaseIndent()

class EvenRule(ParameterRule):
    def check(self, parameter, doc):
        doc.line("if(int(castedValue) % 2 == 1)")
        doc.increaseIndent()
        doc.line((
            'throw runtime::WrongParameterValue(*{0}Parameter, '
            '*this, "Only even values are allowed");'
            ).format(parameter.ident.attribute()))
        doc.decreaseIndent()


    