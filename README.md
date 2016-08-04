# LUA Compiler

A small x86_64 compiler supporting a subset of the LUA programming language.

## Tools
This project is using [flex](http://flex.sourceforge.net/) for lexing and [Bison](https://www.gnu.org/software/bison/) for parsing. 

## Build
### Dependencies
```
apt-get install bison
apt-get install flex
```
### Building the project
```
make
```

## Run
```
./compiler file.lua # Compile file.lua to the file target
./target            # Execute it
```

## Visualizing the intermediate representation
The parse tree can be visualized with [Graphviz](http://www.graphviz.org/) (```apt-get install graphviz```) in the file ```graph.pdf```

## Supported features
- Affectation ```a, b = 3, 4```, ```array[i], array[i+1] = array[i+1], array[i]```
- Control flow ```if ... then ... elseif ... then ... else ...```, ```while```, ```for```
- Input & output  ```io.write()```, ```print()```, ```io.read()```
- Numerical Arrays ```a = {1,2,3}```
- Function definition and (recursive) function calls

## Examples
### Bubble sort:
```
list = { 5, 6, 1, 2, 14, 2, 15, 6, 7, 8, 97 }
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
for i = 1,#list do
  print (list[i])
end
```
### Fibonacci Sequence:
```
function fibonacci(n)
    if n<3 then
        return 1
    else
        return fibonacci(n-1) + fibonacci(n-2)
    end
end

io.write(fibonacci(16))
```
