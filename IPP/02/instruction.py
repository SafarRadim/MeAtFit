import xml.etree.ElementTree as ET

from errors import *
from funcs import *
from frame_stack import *
from argument import *


class Instruction:
    """'Abstract' instruction class"""
    def __init__(self, element, frames, arg_amount):
        self.element = element
        self.frames = frames

        self.arg_amount = arg_amount
        self.opcode = ""
        self.operands = []

    def check_first(self, expected: str):
        """Checks if first param is of expected type, throws exception otherwise"""
        if self.operands[0].type.lower() != expected:
            err_print(f'[ERR] {self.opcode} needs {self.expected} as first operand')
            raise UnknownXmlException

    def set_frames(self, frames):
        self.frames = frames

    def get_opcode(self):
        self.opcode = self.element.get("opcode")

    def check_arg_types(self, expected: str, values: list, offset=1) -> int:
        """Find the amount of args which have expected type, stores values into list. 
        Throws exceptions if variables are not set or do not exist"""
        ret_val = 0
        for operand in self.operands[offset:]:
            if operand.type.lower() == expected:
                values.append(operand.value)
                ret_val += 1

            elif operand.type.lower() == "var":
                var_type = self.frames.get_var_type(operand.value)
                if var_type == "":
                    err_print('[ERR] Unset variable')
                    raise UnknownValueException

                if var_type == expected:
                    values.append(self.frames.get_var(operand.value))
                    ret_val += 1

        return ret_val

    def parse_args(self):
        """Parses instruction arguments"""
        if len(self.element) < self.arg_amount:
            err_print("[ERR] Wrong amount of operands")
            raise UnknownXmlException

        for i in range(self.arg_amount):
            tag = self.element.find(f'arg{i+1}')
            if tag is None:
                continue
            arg = Argument(tag)
            arg.parse_argument()
            self.operands.append(arg)

        if (len(self.operands) != len(self.element.findall("./"))):
            err_print("[ERR] Invalid tag")
            raise UnknownXmlException

    def execute(self):
        """Executes intrepretation of instruction. Has to be defined for each one."""
        raise NotImplementedError

    def __str__(self):
        return self.__class__.__name__


class Move(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        self.frames.move(self.operands[0].value,
                         self.operands[1].type,
                         self.operands[1])


class Create_Frame(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 0)

    def execute(self):
        self.frames.create_frame()


class Push_Frame(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 0)

    def execute(self):
        self.frames.push_frame()


class Pop_Frame(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 0)

    def execute(self):
        self.frames.pop_frame()


class Defvar(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        self.check_first("var")
        self.frames.def_var(self.operands[0].value, self.operands[0].type)


class Call(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Return(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Pushs(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        symb_type = self.operands[0].get_type(self.frames)
        symb_value = self.operands[0].get_val(self.frames)
        
        if symb_value == "":
            err_print('[ERR] Cant push undefined var')
            raise UnknownValueException

        var = Variable(None, symb_type, symb_value)
        self.frames.stack.push(var)


class Pops(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        self.check_first("var")
        if (self.frames.stack.empty):
            err_print(f'[ERR] Cant {self.opcode} from empty stack')
            raise UnknownValueException
       
        var = self.frames.stack.pop()
        self.frames.move(self.operands[0].value, var.type, var.val)


class Add(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        values = []
        self.check_first("var")

        if (self.check_arg_types("int", values) != 2):
            err_print(f'[ERR] {self.opcode} needs two int args')
            raise BadOperandsException

        val = int(values[0]) + int(values[1])
        self.frames.move(self.operands[0].value, "int", val)


class Sub(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        values = []
        self.check_first("var")

        if (self.check_arg_types("int", values) != 2):
            err_print(f'[ERR] {self.opcode} needs two int args')
            raise BadOperandsException

        val = int(values[0]) - int(values[1])
        self.frames.move(self.operands[0].value, "int", val)


class Mul(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        values = []
        self.check_first("var")

        if (self.check_arg_types("int", values) != 2):
            err_print(f'[ERR] {self.opcode} needs two int args')
            raise BadOperandsException

        val = int(values[0]) * int(values[1])
        self.frames.move(self.operands[0].value, "int", val)


class Idiv(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        values = []
        self.check_first("var")

        if (self.check_arg_types("int", values) != 2):
            err_print(f'[ERR] {self.opcode} needs two int args')
            raise BadOperandsException

        if (int(values[1]) == 0):
            err_print('[ERR] Cant idiv by zero')
            raise BadValueException

        val = int(int(values[0]) / int(values[1]))
        self.frames.move(self.operands[0].value, "int", val)


class Lt(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        nilCheck = self.check_arg_types("nil", values)

        if nilCheck:
            err_print(f'[ERR] {self.opcode} cannot compare with nil')
            raise BadOperandsException

        for operand in self.operands[1:]:
            if operand.type == "var":
                operand.type = self.frames.get_var_type(operand.value)
                operand.value = self.frames.get_var(operand.value)

        type1 = self.operands[1].get_type(self.frames)
        value1 = self.operands[1].value
        value2 = self.operands[2].value

        if type1 != self.operands[2].get_type(self.frames):
            err_print(f'[ERR] {self.opcode} can only compare same types')
            raise BadOperandsException
        elif type1 == "int":
            value = int(value1) < int(value2)
        elif type1 == "bool":
            value = get_bool(value1) < get_bool(value2)
        else:
            value = parse_string(value1) < parse_string(value2)

        self.frames.move(self.operands[0].value, "bool", str(value).lower())


class Gt(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        nilCheck = self.check_arg_types("nil", values)

        if nilCheck:
            err_print(f'[ERR] {self.opcode} cannot compare with nil')
            raise BadOperandsException

        for operand in self.operands[1:]:
            if operand.type == "var":
                operand.type = self.frames.get_var_type(operand.value)
                operand.value = self.frames.get_var(operand.value)

        type1 = self.operands[1].get_type(self.frames)
        value1 = self.operands[1].value
        value2 = self.operands[2].value

        if type1 != self.operands[2].get_type(self.frames):
            err_print(f'[ERR] {self.opcode} can only compare same types')
            raise BadOperandsException
        elif type1 == "int":
            value = int(value1) > int(value2)
        elif type1 == "bool":
            value = get_bool(value1) > get_bool(value2)
        else:
            value = parse_string(value1) > parse_string(value2)

        self.frames.move(self.operands[0].value, "bool", str(value).lower())


class Eq(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        nilCheck = self.check_arg_types("nil", values)

        if nilCheck == 2:
            self.frames.move(self.operands[0].value, "bool", str(True).lower())
            return
        if nilCheck == 1:
            self.frames.move(self.operands[0].value,
                             "bool", str(False).lower())
            return

        for operand in self.operands[1:]:
            if operand.type == "var":
                operand.type = self.frames.get_var_type(operand.value)
                operand.value = self.frames.get_var(operand.value)

        type1 = self.operands[1].get_type(self.frames)
        value1 = self.operands[1].value
        value2 = self.operands[2].value

        if type1 != self.operands[2].get_type(self.frames):
            err_print(f'[ERR] {self.opcode} can only compare same types or nil')
            raise BadOperandsException
        elif type1 == "int":
            value = int(value1) == int(value2)
        elif type1 == "bool":
            value = get_bool(value1) == get_bool(value2)
        else:
            value = parse_string(value1) == parse_string(value2)

        self.frames.move(self.operands[0].value, "bool", str(value).lower())


class And(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        if (self.check_arg_types("bool", values) != 2):
            err_print(f'[ERR] {self.opcode} needs both operands bool')
            raise BadOperandsException

        if len(values) != 2:
            err_print(f'[ERR] {self.opcode} needs set vars')
            raise UnknownValueException

        self.frames.move(self.operands[0].value,
                         "bool",
                         str(get_bool(values[0])
                             and get_bool(values[1])).lower())


class Or(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        if (self.check_arg_types("bool", values) != 2):
            err_print(f'[ERR] {self.opcode} needs both operands bool')
            raise BadOperandsException

        if len(values) != 2:
            err_print(f'[ERR] {self.opcode} needs set vars')
            raise UnknownValueException

        self.frames.move(self.operands[0].value, "bool",
                         str(get_bool(values[0])
                             or get_bool(values[1])).lower())


class Not(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        values = []
        boolCheck = self.check_arg_types("bool", values)
        if not boolCheck:
            err_print(f'[ERR] {self.opcode} requires bool operand')
            raise BadOperandsException
        if len(values) != 1:
            err_print(f'[ERR] {self.opcode} requires set var')
            raise UnknownValueException

        value = get_bool(values[0])
        self.frames.move(self.operands[1].value, "bool",
                         str(not value).lower())


class Int2char(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        values = []
        if (self.check_arg_types("int", values) != 1):
            err_print(f'[ERR] {self.opcode} needs int arg')
            raise BadOperandsException

        value = int(values[0])
        if 0 > value or value > 1_114_110:
            err_print(f'[ERR] Value not in valid range for {self.opcode}')
            raise BadStringException

        self.frames.move(self.operands[0].value, "string", chr(value))


class Stri2int(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")

        type1 = self.operands[1].get_type(self.frames)
        if type1 == "":
            err_print(f'[ERR] {self.opcode} needs set vars')
            raise UnknownValueException
        if type1 != "string":
            err_print(f'[ERR] {self.opcode} needs string as second operand')
            raise BadOperandsException

        string = self.operands[1].get_val(self.frames)

        type2 = self.operands[2].get_type(self.frames)
        if type2 == "":
            err_print(f'[ERR] {self.opcode} needs set vars')
            raise UnknownValueException
        if type2 != "int":
            err_print(f'[ERR] {self.opcode} needs int as third operand')
            raise BadOperandsException

        index = int(self.operands[2].get_val(self.frames))

        if 0 > index or index > len(string) - 1:
            err_print('[ERR] String index out of range')
            raise BadStringException

        self.frames.move(self.operands[0].value, "int", ord(string[index]))


class Read(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        read = self.frames.next_read()

        if read is None:
            self.frames.move(self.operands[0].value, "nil", "nil@nil")
            return

        match self.operands[1].value:
            case "int":
                try:
                    read = int(read)
                    self.frames.move(self.operands[0].value, "int", read)
                    return
                except:
                    self.frames.move(self.operands[0].value, "nil", "nil@nil")
                    return

            case "string":
                self.frames.move(self.operands[0].value, "string", read)
                return

            case "bool":
                if read.lower() == "true":
                    read = "bool@true"
                else:
                    read = "bool@false"
                self.frames.move(self.operands[0].value, "bool", read)
                return

            case _:
                self.frames.move(self.operands[0].value, "nil", "nil@nil")
                return


class Write(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        type_list = []
        nil_check = self.check_arg_types("nil", type_list, 0)

        if (nil_check):
            return

        value = self.operands[0].get_val(self.frames)

        if value is None:
            err_print("[ERR] Cant write undefined var")
            raise UnknownValueException

        print(value, end="")


class Concat(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 3)

    def execute(self):
        self.check_first("var")
        values = []
        if (self.check_arg_types("string", values) != 2):
            err_print(f'[ERR] {self.opcode} needs two strings vars')
            raise BadOperandsException

        if len(values) != 2:
            err_print('[ERR] {self.opcode} needs set vars')
            raise UnknownValueException

        self.frames.move(self.operands[0].value, "string", "".join(values))


class Strlen(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        values = []
        stringCheck = self.check_arg_types("string", values)

        if stringCheck != 1:
            err_print(f'[ERR] {self.opcode} requires string operand')
            raise BadOperandsException

        if len(values) != 1:
            err_print(f'[ERR] {self.opcode} requires set var')
            raise UnknownValueException

        self.frames.move(self.operands[0].value, "int", len(values[0]))


class Getchar(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Setchar(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Type(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 2)

    def execute(self):
        self.check_first("var")
        if self.operands[1].type.lower() == "var":
            ret_type = self.frames.get_var_type(self.operands[1].value)
        else:
            ret_type = self.operands[1].type.lower()

        self.frames.move(self.operands[0].value, "string", ret_type)


class Label(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Jump(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Jumpifeq(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Jumpifneq(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Exit(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        value = self.operands[0].get_val(self.frames)
        if value is None:
            err_print('[ERR] Cant exit with uninitialised var')
            raise UnknownValueException

        try:
            value = int(value)
        except ValueError:
            err_print('[ERR] Exit needs integer as operand')
            raise BadOperandsException

        if value < 0 or value > 49:
            err_print("[ERR] Exit only accepts values from 1 to 49")
            raise BadValueException

        exit(value)


class Dprint(Instruction):
    def __init__(self, element, frames):
        super().__init__(element, frames, 1)

    def execute(self):
        type_list = []
        nil_check = self.check_arg_types("nil", type_list, 0)

        if (nil_check):
            return

        value = self.operands[0].get_val(self.frames)

        if value is None:
            err_print("[ERR] Cant write undefined var")
            raise UnknownValueException

        err_print(value, end="")


class Break(Instruction):
    def __init__(self, element, frames):
        raise NotImplementedError


class Instructionaa:
    def execute(self):
        match self.opcode:
            case "getchar":
                raise NotImplementedError
                pass
            case "setchar":
                raise NotImplementedError
                pass
            case "jumpifeq":
                raise NotImplementedError
                pass
            case "jumpifneq":
                raise NotImplementedError
                pass
            case "read":
                raise NotImplementedError
                pass
            case "call":
                raise NotImplementedError
                pass
            case "label":
                raise NotImplementedError
                pass
            case "jump":
                raise NotImplementedError
                pass
            case "pops":
                raise NotImplementedError
                pass
            case "pushs":
                raise NotImplementedError
                pass
            case "return":
                raise NotImplementedError
                pass
            case "break":
                raise NotImplementedError
                pass
