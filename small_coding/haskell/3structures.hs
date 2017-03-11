type Stack = [Integer]

pops :: Stack -> (Integer, Stack)
pops [] = error "Popping empty Stack"
pops (xh:xs) = (xh, xs)

pushs :: Integer -> Stack -> Stack
pushs xh xs = (xh:xs)

_prints :: Stack -> String
_prints [x] = show x
_prints x = do
    let (xh, xs) = pops x
    (show xh) ++ ", " ++ _prints xs

prints :: Stack -> String
prints x = "Stack: [" ++ (_prints x) ++ "]"

type Queue = (Stack, Stack)

shiftq :: Queue -> Queue
shiftq (x, []) = (x, [])
shiftq (l, r) = do
    let (rh, rs) = pops r
    shiftq ((pushs rh l), rs)

popq :: Queue -> (Integer, Queue)
popq ([], []) = error "Popping empty Queue"
popq ([], r) = popq (shiftq ([], r))
popq (l, r) = do
    let (lh, ls) = pops l
    (lh, (ls, r))

pushq :: Integer -> Queue -> Queue
pushq val (l, r) = (l, (pushs val r))

_printq :: Queue -> String
_printq ([], [x]) = show x
_printq ([], (xh:xs)) = (_printq ([], xs)) ++ ", " ++ (show xh)
_printq (x, y) = (_prints x) ++ ", " ++ (_printq ([], y))

printq :: Queue -> String
printq q = "Queue: [" ++ (_printq q) ++ "]"

main :: IO()
main = do
    let x = [1, 2, 3]
    print (prints x)
    print (prints (snd (pops x)))
    print (prints (pushs 4 x))

    let q = (x, x)
    print (printq q)
    print (printq (snd (popq q)))
    print (printq (pushq 7 q))
