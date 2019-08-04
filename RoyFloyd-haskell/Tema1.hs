module Tema1 (
        solveSimple,
        solveCosts
        ) where

import Data.Array

solveSimple :: (Int, [(Int, Int, Int)]) -> Maybe ([Int], Int)
solveSimple (n, l) = let

              dist :: (Int -> Int -> [(Int, Int, Int)] -> Int) -- returns the distance from i to j if there is an edge from node i to node j
              dist i j [] = 922337203685477580    --  else, a big number (infinite distance)
              dist i j ((x,y,z):list) = if ((x == i) && (y == j)) then z
                                        else dist i j list

              boundsdp :: ((Int, Int, Int), (Int, Int, Int))
              boundsdp = ((1, 1, 0), (n, n, n))
              dp = listArray boundsdp [shortestpath i j k | (i, j, k) <- range boundsdp]

              shortestpath :: Int -> Int -> Int -> Int
              shortestpath i j k -- distance from node i to node j via node k
                    | k == 0 = dist i j l -- direct distance (there is no node 0)
                    | otherwise = min (dp ! (i, j, (k-1))) ( (dp ! (i, k, (k-1))) + (dp ! (k, j, (k-1))) )

              path i j 0 = []
              path i j k  -- constructs the intermediate nodes path
                    | directpath < viaKpath = path i j (k - 1)
                    | otherwise = (path i k (k - 1)) ++ [k] ++ (path k j (k - 1))
                    where directpath = dp ! (i, j, k - 1)
                          viaKpath = (dp ! (i, k, k - 1)) + (dp ! (k, j, k - 1))

              in if ((dp ! (1, n, n)) == 922337203685477580) then Nothing
                 else Just (1:(path 1 n n)++[n], dp ! (1, n, n))

-- checks if the path found at solveSimple (the shortest one) is affordable:
solveCosts :: (Int, Int, [Int], [(Int, Int, Int)]) -> Maybe ([(Int, Int)], Int)
solveCosts (n, m, l1, l2) = let

              cost :: (Int -> [Int] -> Int) -- returns the tax for town i
              cost 1 list = 0 -- first town always has cost 0; you don't pay because you are already there
              cost i list = head (drop (i-1) list)
              
              Just (path, dist) = solveSimple (n, l2)
              sum = map (\x -> (x, m - (sumx x path 0))) path
                    where sumx x (a:list) s = if (x == a) then s + cost a l1
                                              else (sumx x list (s + cost a l1))
              -- sumx - calculates the sum of costs of the path from the first to x
              -- m - sumx = the money left after town x ; if this number is > 0, then the path is affordable
              (a, b) = last sum
              
              -- if the path is affordable, is returned; else, Nothing
              in if (b >= 0) then Just (sum, dist) else Nothing
