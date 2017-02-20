-- Implement some simple algorithms
merge :: [Integer] -> [Integer] -> [Integer]
merge l [] = l
merge [] l = l
merge l1@(h1:t1) l2@(h2:t2) =
    if (h1 < h2)
        then [h1] ++ merge t1 l2
        else [h2] ++ merge l1 t2

msort :: [Integer] -> [Integer]
msort [x] = [x]
msort l = do
    let halfSize = (div (length l) 2)
    merge (msort (take halfSize l)) (msort (drop halfSize l))

-- Euler: ((dy/dx x0 y0), (x0, y0), step size, num steps) -> estimate
euler ::
    (Double -> Double -> Double)
    -> (Double, Double)
    -> Double
    -> Integer
    -> Double
euler _ (_, y) _ 0 = y
euler f (x0, y0) size steps = euler
    f
    (x0 + size, y0 + ((f x0 y0) * size))
    size
    (steps - 1)

-- General 2-number fibonacci: given 2 seed numbers, returns fib sequence
genFibTwo :: Integer -> Integer -> [Integer]
genFibTwo a b = do
    let fib@(_:tfib) = a : b : [ i + j | (i, j) <- zip fib tfib ]
    fib

main :: IO ()
main = do
    print (merge [1, 3, 5] [2, 4, 6])
    print (msort [1, 7, 5, 3, 7, 8, 4, 2, 1, 6])

    -- euler's algorithm on dy/dx = y
    let natExp = euler (\_ y -> y) (0, 1)
    print "Euler's algorithm"
    print [(natExp (1 / steps) (round steps)) | steps <- [10, 100, 1000]]
    print (take 10 (genFibTwo 2 3))
