import time
start = time.time()
x = 0.0
for i in range(0, 1000000):
    x += 1.0
end = time.time()
print(end - start)