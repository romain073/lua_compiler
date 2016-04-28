a = 1
b, a = a, b
while a%2==b%2 do
    if a>=2 then
        a = a^2
    else
        b = b+1
    end
end