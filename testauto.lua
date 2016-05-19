str = "123"
print(str)
arr = {1,2,3,4}
print(#arr, arr[1])

var = 45
while var <=50 do
  print(50-var)
  
  for variable=1,50 do
    print(variable)
  end
  
  var = var+1
end





print(13+(22-11)/1*44)
x = 7
y = (10+x)*2
z = y - x
print(z)
print "enter a number:"
n = 10
factorial = 1
x=1
for i = 2,n do
  x = x * i
end
io.write("factorial of ")
io.write(n)
io.write(" is ")
print(x)
print "enter a number:"
n = 10000
for i = 2,n do
  if n%i==0 then
    io.write(i)
    io.write(" is a factor of ")
    print(n)
  end
end
list = { 5, 6, 1, 2, 9, 14, 2, 15, 6, 7, 8, 97 }
itemCount=#list
repeat
  hasChanged = false
  itemCount=itemCount - 1
  for i = 1, itemCount do
    if list[i] > list[i + 1] then
      list[i], list[i + 1] = list[i + 1], list[i]
      hasChanged = true
    end
  end
until hasChanged == false
for i = 1,itemCount do
  print (list[i])
end
function fibonacci(n)
    if n<3 then
        return 1
    else
        return fibonacci(n-1) + fibonacci(n-2)
    end
end

for n = 1, 16 do
    io.write(fibonacci(n), ", ")
end
io.write("...\n")