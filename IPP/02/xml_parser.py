import sys
import xml.etree.ElementTree as ET

from funcs import *
from errors import *
from frame_stack import *
from instruction import *

class Parser:
    """Parser singleton, responsible for xml parsing"""
    __instance = None
    def __new__(cls, source_file):
        if cls.__instance is None:
            cls.__instance = super(Parser, cls).__new__(cls)
            cls.__instance.__initialized = False
        return cls.__instance

    def __init__(self, source_file):
        self.source_file = source_file

        self.root = None
        self.input = None

        self.amount = 0
        self.counter = 0
        self.instruction = 0

        self.source_flag = False
        self.input_flag = False

    def check_files(self):
        if self.source_file is None:
            self.source_file = sys.stdin.read()
            self.source_flag = True


    def parse_source(self):
        """Parses and creates element tree from inputed xml file"""
        try:
            if (self.source_flag):
                self.root = ET.fromstring(self.source_file)
            else:
                tree = ET.parse(self.source_file)
                self.root = tree.getroot()
        except ET.ParseError:
            err_print('[ERR] XML not well-formed')
            raise BadXmlException

        try:
            self.root[:] = sorted(
                           self.root,
                           key=lambda child: (
                               child.tag, int(child.get('order'))
                            )
                        )
        except ValueError:
            raise UnknownXmlException
        except TypeError:
            raise UnknownXmlException

        self.amount = len(self.root)

        if self.root.get('language') != "IPPcode23":
            err_print('[ERR] invalid language')
            raise UnknownXmlException

    def next_instruction(self) -> Instruction:
        """Returns next instruction in line"""
        if self.counter >= self.amount:
            return None

        instruction_element = self.root[self.counter]

        if instruction_element.tag != "instruction":
            err_print('[ERR] Invalid tag')
            raise UnknownXmlException

        if instruction_element.get('order') is None:
            err_print('[ERR] Order is required attribute')
            raise UnknownXmlException

        try:
            order = int(instruction_element.get('order'))
        except ValueError:
            err_print('[ERR] Order has to be positive int')
            raise UnknownXmlException

        if order <= self.instruction:
            err_print('[ERR] invalid opcode counter')
            raise UnknownXmlException

        self.instruction = order
        self.counter += 1

        opcode = instruction_element.get("opcode")
        if opcode is None:
            err_print('[ERR] Opcode missing')
            raise UnknownXmlException

        opcode = opcode.lower()

        match opcode:
            case "move":
                instruction = Move(instruction_element, None)

            case "createframe":
                instruction = Create_Frame(instruction_element, None)

            case "pushframe":
                instruction = Push_Frame(instruction_element, None)

            case "popframe":
                instruction = Pop_Frame(instruction_element, None)

            case "defvar":
                instruction = Defvar(instruction_element, None)

            case "call":
                instruction = Call(instruction_element, None)

            case "return":
                instruction = Return(instruction_element, None)

            case "pushs":
                instruction = Pushs(instruction_element, None)

            case "pops":
                instruction = Pops(instruction_element, None)

            case "add":
                instruction = Add(instruction_element, None)

            case "sub":
                instruction = Sub(instruction_element, None)
                
            case "mul":
                instruction = Mul(instruction_element, None)

            case "idiv":
                instruction = Idiv(instruction_element, None)

            case "lt":
                instruction = Lt(instruction_element, None)

            case "gt":
                instruction = Gt(instruction_element, None)

            case "eq":
                instruction = Eq(instruction_element, None)

            case "and":
                instruction = And(instruction_element, None)

            case "or":
                instruction = Or(instruction_element, None)

            case "not":
                instruction = Not(instruction_element, None)

            case "int2char":
                instruction = Int2char(instruction_element, None)

            case "stri2int":
                instruction = Stri2int(instruction_element, None)

            case "read":
                instruction = Read(instruction_element, None)

            case "write":
                instruction = Write(instruction_element, None)

            case "concat":
                instruction = Concat(instruction_element, None)

            case "strlen":
                instruction = Strlen(instruction_element, None)

            case "getchar":
                instruction = Getchar(instruction_element, None)

            case "setchar":
                instruction = Setchar(instruction_element, None)

            case "type":
                instruction = Type(instruction_element, None)

            case "label":
                instruction = Label(instruction_element, None)

            case "jump":
                instruction = Jump(instruction_element, None)

            case "jumpifeq":
                instruction = Jumpifeq(instruction_element, None)

            case "jumpifneq":
                instruction = Jumpifneq(instruction_element, None)

            case "exit":
                instruction = Exit(instruction_element, None)

            case "dprint":
                instruction = Dprint(instruction_element, None)

            case "break":
                instruction = Break(instruction_element, None)

            case _:
                err_print('[ERR] Unknown opcode')
                raise UnknownXmlException()

        return instruction

