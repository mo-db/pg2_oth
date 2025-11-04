-- first example
a = 1 + 2 + math.sin(1)
a = 2 + 8 + math.sin(1)

-- second example (calling a function from C)
function Foo(a, b)
  return a / b
end

-- third example (calling a C function)
function Bar(a, b)
  print("Bar has been called")
  print("test")
  c = HostFunction(a * 2, b * 2)
  return c * 2
end
