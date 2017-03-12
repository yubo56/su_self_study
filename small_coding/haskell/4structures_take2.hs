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

-- wraps prints for Integer stacks
int_prints :: Stack Integer -> String
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

-- wraps printq for Integer queues
intPrintq :: Queue Integer -> String
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

intCmp :: Integer -> Integer -> Bool
intCmp a b = a > b

data PQueue cmp a =
    PQueue cmp a (PQueue cmp a) (PQueue cmp a) |
    Leaf a |
    EmptyQ

peekPq :: PQueue cmp a -> a
peekPq EmptyQ = error "Peeking empty Priority Queue"
peekPq (Leaf x) = x
peekPq (PQueue _ x _ _) = x

sizePq :: PQueue cmp a -> Integer
sizePq EmptyQ = 0
sizePq (Leaf _) = 1
sizePq (PQueue _ _ l r) = sizePq l + sizePq r

printPq :: (a -> String) -> PQueue cmp a -> String
printPq _ EmptyQ = "Priority Queue: (Empty)"
printPq f pq = "Priority Queue: " ++ _print_pq f pq where
    _print_pq _ EmptyQ = ""
    _print_pq f' (Leaf val) = f' val
    _print_pq f' (PQueue _ _ l r) = "(" ++ _print_pq f' l ++ ", " ++ _print_pq f' r ++ ")"

_popLast :: PQueue cmp a -> (a, PQueue cmp a)
_popLast EmptyQ = error "_popLast called on empty Priority Queue"
_popLast (Leaf x) = (x, EmptyQ)
_popLast (PQueue cmp a l r)
    | sizePq l > sizePq r = do
        let (x, new_l) = _popLast l
        (x, PQueue cmp a new_l r)
    | sizePq l == sizePq r = do
        let (x, new_r) = _popLast r
        (x, PQueue cmp a l new_r)
    | otherwise =
        error "Corrupted Priority Queue!"

main :: IO()
main = do
    let x = pushs 1 (pushs 2 (pushs 3 Empty))
    print (int_prints x)
    print (int_prints (snd (pops x)))
    print (int_prints (pushs 4 x))

    let q = Queue x x
    print (int_printq q)
    print (int_printq (snd (popq q)))
    print (int_printq (pushq 7 q))

    print (fst (_popLast (Leaf 5)) :: Integer)
