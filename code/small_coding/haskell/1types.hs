-- lists
arrLen :: [a] -> Integer
arrLen = foldr (\_ -> (+) 1) 0

-- recursion
collInt :: (String, Integer) -> (String, Integer)
collInt (str, 1) = (str ++ "1", 1)
collInt (str, val) =
  if mod val 2 == 0
  then collInt (str ++ show val ++ ", ", div val 2)
  else collInt (str ++ show val ++ ", ", 3 * val + 1)

collatz :: Integer -> String
collatz val = fst (collInt ("", val))

-- recursive types
data Tree a = Leaf a | Branch (Tree a) (Tree a)
minTree :: Tree a -> (a -> a -> a) -> a
minTree (Leaf x) _ = x
minTree (Branch x y) cmp = cmp (minTree x cmp) (minTree y cmp)

main :: IO()
main = do
    print (arrLen ([1, 2, 3] :: [Integer]))

    print (collatz 11)

    let intTree = Branch
            (Leaf (3 :: Integer))
            (Branch
                (Branch (Leaf 3) (Leaf 5))
                (Branch (Leaf 2) (Leaf 3))
            )
    print (minTree intTree min)
