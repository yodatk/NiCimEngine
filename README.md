# NiCim Engine

IMPROVED 3rd version of Nissim engine: NiCim. Working with UCI protocol and Bitboards. Written in C++
You can play against NiCim here: https://lichess.org/@/NissimEngine

## Features:
 * Efficient Bitboards representation of Chess Board (https://www.chessprogramming.org/Bitboards)
 * Pre-Calculated Moves and Attacks, using MagicBitboards (https://www.chessprogramming.org/Magic_Bitboards)
 * Alpha Beta pruning based search, with:
    - Quiescence Search (https://www.chessprogramming.org/Quiescence_Search)
    - Principal Variation Search (https://www.chessprogramming.org/Principal_Variation_Search)
    - Move ordering by  (https://www.chessprogramming.org/Move_Ordering):
        - Principal variation Node (https://www.chessprogramming.org/PV-Move)
        - Killer moves (https://www.chessprogramming.org/Killer_Heuristic)
        - History moves (https://www.chessprogramming.org/History_Heuristic)
    - Late Move Reduction (https://www.chessprogramming.org/Late_Move_Reductions)
    - Null Move Pruning (https://www.chessprogramming.org/Null_Move_Pruning)
    - Razoring (https://www.chessprogramming.org/Razoring)
 * NNUE Evaluation! (by https://github.com/dshawul/nnue-probe)
 * Transposition Table (https://www.chessprogramming.org/Transposition_Table)


## Thanks A Lot to these Wonderful guides: 
- Chess Programming : https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&ab_channel=ChessProgramming
- Logic Crazy Chess: https://www.youtube.com/watch?v=a5IGltn95Bk&ab_channel=LogicCrazyChess

## Additional Credits and Thanks:
- Noga Regev for the AMAZING artwork of Nicim Icon. 
![alt text](https://github.com/yodatk/NiCimEngine/blob/master/resources/NiCim_Logo.png?raw=true)
