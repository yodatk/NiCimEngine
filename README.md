# NiCim Engine
![alt text](https://github.com/yodatk/NiCimEngine/blob/master/resources/NiCim_Logo_Small.png?raw=true)

IMPROVED 3rd version of Nissim engine: NiCim. Working with UCI protocol. Written in C++.

**You can play against NiCim here:** https://lichess.org/@/NissimEngine

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
 * Opening Book support with polyglot format. (as described in this toturial: https://www.youtube.com/watch?v=HjZtevzCa5Y&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=88&ab_channel=BluefeverSoftware)
 
 ## How to install and Use NiCim:
 currently, NiCim can compile only to windows machines. there are two ways to compile niCim:
  * you can use Cmake with the provided CMakeLists.txt file. (really easy to do with CLion)
  * you can use the provided makefile.
 ### IMPORTANT NOTES:
   * NiCim executable must be next to the provided .nnue file, otherwise, it's evaluation won't work. 
   * NiCim can support polyglot format opening books, and provided by one. if you wish to change the book you need to chage the default NAME_OF_BOOK in "PolyBook.h". If you with to turn off this option, you can through the uci protocol.
   * If you wish to use NiCim as a bot Player on lichess (using this awsome code by ShailChoksi : https://github.com/ShailChoksi/lichess-bot) it is very recommanded to change the delay time of NiCim so it won't accidently lose on time. to do that, simply add txt file named (EXACTLY) "ONLINE_TIME_CONFIG.txt" to the working directory\ same directory where NiCimEngine.exe file is in. you don't have to write any thing in it, NiCim will automatically change it's delay time accordingly


## Thanks A Lot to these Wonderful guides: 
- Chess Programming : https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&ab_channel=ChessProgramming
- Logic Crazy Chess: https://www.youtube.com/watch?v=a5IGltn95Bk&ab_channel=LogicCrazyChess
- Bluefever Software: https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&ab_channel=BluefeverSoftware

## Additional Credits and Thanks:
- Noga Regev for the AMAZING artwork of Nicim Icon. 

