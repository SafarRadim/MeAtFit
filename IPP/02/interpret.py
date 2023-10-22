#!/usr/bin/env python3.10
import argparse
import os.path

from xml_parser import *
from funcs import *
from errors import *
from instruction import *
from frame_stack import *

arg_parser = argparse.ArgumentParser(
    prog="Interpret",
    description="Interpret for second part of IPP project"
)

arg_parser.add_argument(
    '--source',
    nargs='?',
    default=None,
    help="File to read XML input from",
    type=file_check
)
arg_parser.add_argument(
    '--input',
    nargs='?',
    default=None,
    help="File to read inputs to READs from",
    type=file_check
)


def main_loop(parser, frames):
    while True:
        instruction = parser.next_instruction()
        if instruction is None:
            break

        instruction.parse_args()
        instruction.set_frames(frames)
        instruction.get_opcode()
        instruction.execute()


if __name__ == "__main__":
    args = arg_parser.parse_args()

    if (args.source is None and args.input is None):
        err_print("[ERR] Both source and input can not be none")
        exit(ErrCode.param)

    parser = Parser(args.source)

    frames = Frame_Stack(args.input)
    frames.check_input()
    parser.check_files()

    try:
        parser.parse_source()
        # This is so I save few indents
        main_loop(parser, frames)

    except InputFileException:
        exit(ErrCode.inputFile)
    except BadXmlException:
        exit(ErrCode.badXml)
    except UnknownXmlException:
        exit(ErrCode.unknownXml)
    except SemanticException:
        exit(ErrCode.semantic)
    except BadOperandsException:
        exit(ErrCode.operands)
    except UnknownVariableException:
        exit(ErrCode.unknownVar)
    except UnknownFrameException:
        exit(ErrCode.unknownFrame)
    except UnknownValueException:
        exit(ErrCode.unknownValue)
    except BadValueException:
        exit(ErrCode.badValue)
    except BadStringException:
        exit(ErrCode.badString)
    except InternalException:
        exit(ErrCode.internal)
    except NotImplementedError:
        err_print("Not implemented")
        exit(420)
