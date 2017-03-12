--
-- Stack
--
data Stack a = Stack a (Stack a) | Empty

-- prints stack element-by-element
_prints :: (a -> String) -> Stack a -> String
_prints _ Empty = ""
_prints f (Stack h s) = (f h) ++ (_printt f s) where
    _printt _ Empty = ""
    _printt ft (Stack ht st) = ", " ++ (ft ht) ++ (_printt ft st)

-- surrounds _prints with "Stack: []" formatting
prints :: (a -> String) -> Stack a -> String
prints f s = "Stack: [" ++ (_prints f s) ++ "]"

-- wraps prints for Integer stacks
int_prints :: Stack Integer -> String
int_prints s = prints show s

pushs :: a -> Stack a -> Stack a
pushs el s = Stack el s

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
_printq :: (a -> String) -> Queue a -> String
_printq f (Queue l Empty) = (_prints f l)
_printq f q@(Queue Empty _) = (_printq f (shiftq q))
_printq f (Queue l r) =
    (_printq f (Queue l Empty)) ++
    ", " ++
    (_printq f (Queue Empty r))

-- surrounds _printq with "Queue: []" formatting
printq :: (a -> String) -> Queue a -> String
printq f q = "Queue: [" ++ (_printq f q) ++ "]"

-- wraps printq for Integer queues
int_printq :: Queue Integer -> String
int_printq q = printq show q

popq :: Queue a -> (a, Queue a)
popq (Queue Empty Empty) = error "Popping empty Queue"
popq q@(Queue Empty _) = popq (shiftq q)
popq (Queue l r) = do
    let (lh, ls) = pops l
    (lh, (Queue ls r))

pushq :: a -> Queue a -> Queue a
pushq val (Queue l r) = (Queue l (pushs val r))

--
-- Priority Queue
--

int_cmp :: Integer -> Integer -> Bool
int_cmp a b = a > b

data PQueue cmp a =
    PQueue cmp a (PQueue cmp a) (PQueue cmp a) |
    Leaf a |
    EmptyQ

peek_pq :: PQueue cmp a -> a
peek_pq (EmptyQ) = error "Peeking empty Priority Queue"
peek_pq (Leaf x) = x
peek_pq (PQueue _ x _ _) = x

size_pq :: PQueue cmp a -> Integer
size_pq (EmptyQ) = 0
size_pq (Leaf _) = 1
size_pq (PQueue _ _ l r) = size_pq(l) + size_pq(r)

_pop_last :: PQueue cmp a -> (a, PQueue cmp a)
_pop_last (EmptyQ) = error "_pop_last called on empty Priority Queue"
_pop_last (Leaf x) = (x, EmptyQ)
_pop_last (PQueue cmp a l r)
    | (size_pq l) > (size_pq r)     = do
        let (x, new_l) = _pop_last l
        (x, (PQueue cmp a new_l r))
    | (size_pq l) == (size_pq r)     = do
        let (x, new_r) = _pop_last r
        (x, (PQueue cmp a l new_r))

main :: IO()
main = do
    let x = (pushs 1 (pushs 2 (pushs 3 Empty)))
    print (int_prints x)
    print (int_prints (snd (pops x)))
    print (int_prints (pushs 4 x))

    let q = (Queue x x)
    print (int_printq q)
    print (int_printq (snd (popq q)))
    print (int_printq (pushq 7 q))

    print (fst (_pop_last (Leaf 5)) :: Integer)
