start = os.clock()
x = 0.0
for i=0,1000000 do
    x = x + 1
end
end_ = os.clock()
print(end_ - start)