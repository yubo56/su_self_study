--
-- Stack
--
data Stack a = Stack a (Stack a) | Empty

-- prints stack element-by-element
_prints :: (a -> String) -> Stack a -> String
_prints _ Empty = ""
_prints f (Stack h s) = f h ++ _printt f s where
    _printt :: (a -> String) -> Stack a -> String
    _printt _ Empty = ""
    _printt ft (Stack ht st) = ", " ++ ft ht ++ _printt ft st

-- surrounds _prints with "Stack: []" formatting
prints :: (a -> String) -> Stack a -> String
prints f s = "Stack: [" ++ _prints f s ++ "]"

-- wraps prints for Int stacks
intPrints :: Stack Int -> String
intPrints = prints show

pushs :: a -> Stack a -> Stack a
pushs = Stack

pops :: Stack a -> (a, Stack a)
pops Empty = error "Popping empty stack"
pops (Stack h s) = (h, s)

--
-- Queue
--
data Queue a = Queue (Stack a) (Stack a)

shiftq :: Queue a -> Queue a
shiftq q@(Queue _ Empty) = q
shiftq (Queue l r) = do
    let (rh, rs) = pops r
    shiftq (Queue (pushs rh l) rs)

-- prints queue element-by-element
printq :: (a -> String) -> Queue a -> String
printq f q = "Queue: [" ++ _printq f q ++ "]" where
    _printq :: (a -> String) -> Queue a -> String
    _printq f' (Queue l Empty) = _prints f' l
    _printq f' q'@(Queue Empty _) = _printq f' (shiftq q')
    _printq f' (Queue l r) =
        _printq f' (Queue l Empty) ++
        ", " ++
        _printq f' (Queue Empty r)

-- wraps printq for Int queues
intPrintq :: Queue Int -> String
intPrintq = printq show

popq :: Queue a -> (a, Queue a)
popq (Queue Empty Empty) = error "Popping empty Queue"
popq q@(Queue Empty _) = popq (shiftq q)
popq (Queue l r) = do
    let (lh, ls) = pops l
    (lh, Queue ls r)

pushq :: a -> Queue a -> Queue a
pushq val (Queue l r) = Queue l (pushs val r)

--
-- Priority Queue
--

intCmp :: Int -> Int -> Bool
intCmp a b = a > b

data PQueue a =
    PQueue a (PQueue a) (PQueue a) |
    Leaf a |
    EmptyPQ

_getRoot :: PQueue a -> a
_getRoot EmptyPQ = error "No root for empty Priority Queue"
_getRoot (Leaf x) = x
_getRoot (PQueue x _ _) = x

-- counts number of complete levels, rounded down
_occupancy :: PQueue a -> Double
_occupancy EmptyPQ = 0
_occupancy (Leaf _) = 1
_occupancy (PQueue _ l r) = 1 + (_occupancy l + _occupancy r) / 2

-- whether next available slot is left or not
_nextPushLeft :: PQueue a -> Bool
_nextPushLeft EmptyPQ = error "_nextPushLeft called on empty Priority Queue"
_nextPushLeft (Leaf _) = error "_nextPushLeft called on Leaf"
_nextPushLeft (PQueue _ l r)
    | _occupancy l == _occupancy r = True
    | fromInteger (round (_occupancy l)) == _occupancy l = False
    | otherwise = True

-- whether last used slot is left or not
_nextPopLeft :: PQueue a -> Bool
_nextPopLeft EmptyPQ = error "_nextPopLeft called on empty Priority Queue"
_nextPopLeft (Leaf _) = error "_nextPopLeft called on Leaf"
_nextPopLeft (PQueue _ l r)
    | _occupancy l == _occupancy r = False
    | fromInteger (round (_occupancy r)) == _occupancy r = True
    | otherwise = False

_popLast :: PQueue a -> (a, PQueue a)
_popLast EmptyPQ = error "_popLast called on empty Priority Queue"
_popLast (Leaf x) = (x, EmptyPQ)
_popLast (PQueue x (Leaf y) EmptyPQ) = (y, Leaf x)
_popLast pq@(PQueue x l r)
    | _nextPopLeft pq = do
        let (y, new_l) = _popLast l
        (y, PQueue x new_l r)
    | otherwise = do
        let (y, new_r) = _popLast r
        (y, PQueue x l new_r)

_swapRoot :: a -> PQueue a -> (a, PQueue a)
_swapRoot v EmptyPQ = (v, EmptyPQ) -- cannot swap into an empty PQ, edge case
_swapRoot v (Leaf x) = (x, Leaf v)
_swapRoot v (PQueue x l r) = (x, PQueue v l r)

-- will only recurse into a child if its root has been modified
_reheap :: (a -> a -> Bool) -> PQueue a -> PQueue a
_reheap _ EmptyPQ = EmptyPQ
_reheap _ (Leaf x) = Leaf x
_reheap cmp pq@(PQueue x l EmptyPQ) -- l must be a Leaf
    | cmp x (_getRoot l) = pq
    | otherwise = do
        let (x', l') = _swapRoot x l
        PQueue x' l' EmptyPQ
_reheap cmp pq@(PQueue x l r) -- both children are guaranteed non-empty
    | cmp x (_getRoot l) && cmp x (_getRoot r) = pq
    | cmp (_getRoot r) (_getRoot l) = do
        let (x', r') = _swapRoot x r
        PQueue x' l (_reheap cmp r')
    | otherwise = do
        let (x', l') = _swapRoot x l
        PQueue x' (_reheap cmp l') r

-- pushPQ is responsible for reheaping as it pops out of its stack since
-- _reheap assumes both children are already heaps, which isn't the case
-- immediately after pushing a new element
--
-- Note that each application of _reheap descends at most one level since
-- everything beneath it already satisfies heap properties, so we are
-- guaranteed only O(log n) calls to _reheap for PQ of size n
pushPQ :: (a -> a -> Bool) -> a -> PQueue a -> PQueue a
pushPQ _ v EmptyPQ = Leaf v
pushPQ cmp v (Leaf x) = _reheap cmp (PQueue x (Leaf v) EmptyPQ)
pushPQ cmp v pq@(PQueue x l r)
    | _nextPushLeft pq = _reheap cmp (PQueue x (pushPQ cmp v l) r)
    | otherwise = _reheap cmp (PQueue x l (pushPQ cmp v r))

popPQ :: (a -> a -> Bool) -> PQueue a -> (a, PQueue a)
popPQ cmp pq = do
    let (newRoot, pq') = _popLast pq
    let (root, pq'') = _swapRoot newRoot pq'
    (root, _reheap cmp pq'')

printPQ :: (a -> String) -> PQueue a -> String
printPQ _ EmptyPQ = "Priority Queue: (Empty)"
printPQ f pq = "Priority Queue: (" ++ _print_pq f pq ++ ")" where
    _print_pq _ EmptyPQ = ""
    _print_pq f' (Leaf x) = f' x
    _print_pq f' (PQueue x l EmptyPQ) = f x ++ ": (" ++ _print_pq f' l ++ ")"
    _print_pq f' (PQueue  x l r) = f x ++ ": (" ++ _print_pq f' l ++ ", " ++ _print_pq f' r ++ ")"

printOrderedPQ :: (a -> String) -> (a -> a -> Bool) -> PQueue a -> String
printOrderedPQ _ _ EmptyPQ = ""
printOrderedPQ toStr _ (Leaf x) = toStr x
printOrderedPQ toStr cmp pq = do
    let (root, pq') = popPQ cmp pq
    toStr root ++ ", " ++ printOrderedPQ toStr cmp pq'

intPushPQ :: Int -> PQueue Int -> PQueue Int
intPushPQ = pushPQ intCmp
intPopPQ :: PQueue Int -> (Int, PQueue Int)
intPopPQ = popPQ intCmp
intPrintPQ :: PQueue Int -> String
intPrintPQ = printPQ show
intPopAllPQ :: PQueue Int -> String
intPopAllPQ = printOrderedPQ show intCmp

main :: IO()
main = do
    let s = pushs 1 (pushs 2 (pushs 3 Empty))
    print (intPrints s)
    print (intPrints (snd (pops s)))
    print (intPrints (pushs 4 s))

    let q = Queue s s
    print (intPrintq q)
    print (intPrintq (snd (popq q)))
    print (intPrintq (pushq 7 q))

    print (fst (intPopPQ (intPushPQ 5 EmptyPQ)))
    print (intPrintPQ (snd (intPopPQ (intPushPQ 5 EmptyPQ))))

    let quasiRandom :: [Int]
        quasiRandom = [round (1000 * sin x) | x <- [0..100] :: [Double]]
    -- print ("Not really random numbers: " ++ show quasiRandom)

    let pq = foldr intPushPQ EmptyPQ quasiRandom
    -- print (intPrintPQ pq)
    print (intPopAllPQ pq)
