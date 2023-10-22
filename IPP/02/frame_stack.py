from stack import Stack
from errors import *
from funcs import *
from variable import *


class Frame_Stack():
    """Main class that handles frames and operations above them. Because of poor planning also handles READ and stack"""
    __instance = None
    def __new__(cls, input_file):
        if cls.__instance is None:
            cls.__instance = super(Frame_Stack, cls).__new__(cls)
            cls.__instance.__initialized = False
        return cls.__instance

    def __init__(self, input_file):
        self.g_frame = {}
        self.t_frame = None
        self.l_frame = None

        self.l_stack = Stack()
        self.stack = Stack()

        self.input_flag = False
        self.input_file = input_file
        self.input = None

    def check_input(self):
        if self.input_file is None:
            self.input_flag = True
        else:
            self.input = open(self.input_file, 'r')

    def create_frame(self):
        self.t_frame = {}

    def push_frame(self):
        if self.t_frame is None:
            err_print('[ERR] Cant push frame without defined tf')
            raise UnknownFrameException
        self.l_stack.push(self.t_frame)
        self.l_frame = self.l_stack.top()
        self.t_frame = None

    def pop_frame(self):
        try:
            self.t_frame = self.l_stack.pop()
        except IndexError:
            raise UnknownFrameException
        self.l_frame = self.l_stack.top()

    def def_var(self, var, var_type):
        """Parses var in string format and tries to store it in frame"""
        var = var.split("@")
        frame = self.get_frame(var[0].lower())
        if frame is None:
            err_print('[ERR] Invalid frame')
            raise UnknownFrameException
        self.check_var(frame, var[1])
        variable = Variable(var[1], var_type, None)
        frame[var[1]] = variable

    def move(self, var, var_type, value):
        """Parses var in string format and tries to store value in it"""
        var = var.split("@")
        frame = self.get_frame(var[0].lower())
        
        if value.__class__.__name__ == "Argument":
            if value.type == "var":
                value = self.get_var(value.value)
                if value is None:
                    raise UnknownValueException
            else:
                value = value.value

        if frame is None:
            err_print("[ERR] Invalid frame")
            raise UnknownFrameException

        if var[1] in frame:
            frame[var[1]].var_type = var_type
            frame[var[1]].value = value
        else:
            err_print('[ERR] Variable is not in frame')
            raise UnknownVariableException

    def get_frame(self, frame):
        """Gets specific frame from var 'prefix'"""
        if (frame == "lf"):
            frame = self.l_frame
        elif (frame == "tf"):
            frame = self.t_frame
        elif (frame == "gf"):
            frame = self.g_frame

        return frame

    def check_var(self, frame, var):
        """Checks if var is defined, throws exception if it is"""
        if var in frame:
            err_print("[ERR] Variable already defined")
            raise SemanticException

    def get_var(self, var):
        """Gets var value, throws exception if it does not exist"""
        var = var.split("@")
        frame = self.get_frame(var[0].lower())

        if frame is None:
            err_print('[ERR] Invalid frame')
            raise UnknownFrameException

        if var[1] not in frame:
            err_print('[ERR] Variable is not in frame')
            raise UnknownVariableException
        
        return frame[var[1]].value

    def get_var_type(self, var):
        """Gets var type, throws exception if it does not exist"""
        var = var.split("@")
        frame = self.get_frame(var[0].lower())

        if len(var) < 2:
            err_print('[ERR] Undefined var')
            raise UnknownVariableException

        if frame is None:
            err_print('[ERR] Invalid frame')
            raise UnknownFrameException

        if var[1] not in frame:
            err_print('[ERR] Variable is not in frame')
            raise UnknownVariableException
        
        if frame[var[1]].value is None:
            return ""

        return frame[var[1]].var_type

    def next_read(self):
        """Reads next input - bad design choice"""
        if (self.input_flag):
            try:
                value = input()
                return value.strip("\n")
            except EOFError:
                return None
        else:
            return self.input.readline()
