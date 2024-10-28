local start = os.clock()
local x = 0.0
for i = 0, 999999 do
    x = x + 1.0
end
local finish = os.clock()
print(finish - start)