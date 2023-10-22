from funcs import *

class Argument:
    """Class to hold instruction arguments"""
    def __init__(self, element):
        self.element = element

        self.type = None
        self.value = None

    def parse_argument(self):
        self.type = self.element.get("type")
        if self.element.text is None and self.type == "string":
            self.value = ""
        else:
            self.value = self.element.text

    def get_val(self, frames):
        """Gets argument value, if it is var then it tries to get it from frame"""
        outType = self.type
        if outType == "string":
            return parse_string(self.value)
        if outType == "var":
            var_type = frames.get_var_type(self.value)
            value = frames.get_var(self.value)
            if var_type == "string":
                return parse_string(value)
            else:
                return value
        else:
            return self.value

    def get_type(self, frames):
        """Gets argument type, if it is var then it tries to get it from frame"""
        outType = self.type
        if outType == "var":
            var_type = frames.get_var_type(self.value)
            return var_type
        else:
            return outType
