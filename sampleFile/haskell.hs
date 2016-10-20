main = do
  n <- readLn
  print $ sum' n

sum' :: Int -> Int
sum' 0 = 0
sum' n = n + sum'(n-1)

{-|
===
5
---
15
===
10
---
55
===
-}
