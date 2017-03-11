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
-- Priority Queue (TODO)
--

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
