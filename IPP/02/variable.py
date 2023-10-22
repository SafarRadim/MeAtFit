class Variable:
    """Class for variables"""
    def __init__(self, name, var_type, value):
        self.name = name
        self.var_type = var_type
    
        if value is None and var_type == "string":
            self.value = ""
        else:
            self.value = value
