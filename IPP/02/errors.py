from enum import IntEnum


class ErrCode(IntEnum):
    param = 10
    inputFile = 11
    badXml = 31
    unknownXml = 32
    semantic = 52
    operands = 53
    unknownVar = 54
    unknownFrame = 55
    unknownValue = 56
    badValue = 57
    badString = 58
    internal = 99


class ParamException (Exception):
    pass  # 10


class InputFileException (Exception):
    pass  # 11


class BadXmlException (Exception):
    pass  # 31


class UnknownXmlException (Exception):
    pass  # 32


class SemanticException (Exception):
    pass  # 52


class BadOperandsException (Exception):
    pass  # 53


class UnknownVariableException (Exception):
    pass  # 54


class UnknownFrameException (Exception):
    pass  # 55


class UnknownValueException (Exception):
    pass  # 56


class BadValueException (Exception):
    pass  # 57


class BadStringException (Exception):
    pass  # 58


class InternalException (Exception):
    pass  # 99
