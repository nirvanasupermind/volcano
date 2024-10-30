start = os.clock()
x = {val = 0.0}
for i=0,1000000 do
    x.val = x.val + 1.0
end
end_ = os.clock()
print(end_ - start)