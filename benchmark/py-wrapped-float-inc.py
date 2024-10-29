import time
start = time.time()
class Wrapper:
    def __init__(self, val):
        self.val = val
x = Wrapper(0.0)
for i in range(0, 1000000):
    x.val += 1.0
end = time.time()
print(end - start)