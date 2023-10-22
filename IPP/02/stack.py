class Stack():
    """Basic stack implementation."""
    def __init__(self):
        self.stack = []

    def empty(self):
        if len(self.stack):
            return True
        else:
            return False

    def size(self):
        return len(self.stack)

    def top(self):
        if len(self.stack) == 0:
            return None
        else:
            return self.stack[-1]

    def push(self, value):
        return self.stack.append(value)

    def pop(self):
        return self.stack.pop()
