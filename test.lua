print "enter a number:"
n = 42
for i = 2,n do
  if n%i==0 then
    io.write(i, n)
    io.write(" is a factor of ")
    print(n)
  end
end