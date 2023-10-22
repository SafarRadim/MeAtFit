import sys
import os

from errors import *

"""Useful functions in one place"""

def err_print(*args, **kwargs):
    """Prints on stderr"""
    print(*args, file=sys.stderr, **kwargs)

def file_check(file_name):
    """Used in argparse, checks if file exists"""
    if not os.path.isfile(file_name):
        err_print(f"[ERR] Invalid file {file_name}")
        exit(11)
    else:
        return file_name

def parse_string(string):
    """Replaces escape sequences with corresponding characters"""
    buff = ""
    tmp = ""
    flag = False

    for character in string:
        if not flag:
            if character == "\\":
                flag = True
                continue
            buff += character
        else:
            if not character.isdigit():
                flag = False
                tmp = ""
                continue
            tmp += character
            
            if len(tmp) == 3:
                buff += chr(int(tmp))
                flag = False
                tmp = ""
    
    return buff

def get_bool(arg):
    """Gets bool value from string"""
    if arg.lower() == "false":
        return False
    else:
        return True