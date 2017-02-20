-- lists
arrLen :: [a] -> Integer
arrLen [] = 0
arrLen (x:xs) = 1 + arrLen(xs)
-- main = print (arrLen [1, 2, 3])

-- recursion
collInt :: ([Char], Integer) -> ([Char], Integer)
collInt (str, 1) = (str ++ "1", 1)
collInt (str, val) =
  if (mod val 2) == 0
  then collInt (str ++ show val ++ ", ", (div val 2))
  else collInt (str ++ show val ++ ", ", (3 * val + 1))

collatz :: Integer -> [Char]
collatz val = fst (collInt ("", val))
-- main = print (collatz 11)

-- recursive types
data Tree a = Leaf a | Branch (Tree a) (Tree a)
minTree :: Tree a -> (a -> a -> a) -> a
minTree (Leaf x) cmp = x
minTree (Branch x y) cmp = cmp (minTree x cmp) (minTree y cmp)

intTree = Branch
    (Leaf 3)
    (Branch
        (Branch
            (Leaf 3)
            (Leaf 5))
        (Branch
            (Leaf 2)
            (Leaf 3)))
-- main = print (minTree intTree min)
