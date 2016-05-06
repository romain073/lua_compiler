b = {1,2,3}
print "enter a number:"
n = 42
for i = 2,n do
  if n%i==0 then
    io.write(i)
    io.write(" is a factor of ")
    print(n)
  end
end

a = 1
repeat 
a = 1+a
until a==5
print(a)