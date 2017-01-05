## Production of Curio

#### NOTES
- The italic styled text refers to a 'statement' or 'expression' in the productions.
- The bold styled text refers to a 'keyword'.
- The normal styled characters such as '(' refers to a terminal symbol.

#### Productions


program → block 
> program is the largest container of a program.

block   → { dcls stmts } 
> block is a block of codes.

dcls    → dcls dcl | ɛ 
> ɛ is the end of production.

dcl     → type **id** ; 
> dcl is abbr. of declaration.

type    → type [ **num** ] | **basic** 
> basic is one of 'string', 'number' and 'boolean'.
  num is numbers from 0 to 9.

stmts   → stmts stmt | ɛ

stmt    → loc = bool ;
        | **if** ( bool ) stmt
        | **if** ( bool ) stmt **else** stmt
        | **while** ( bool ) stmt
        | block

loc     → loc [ bool ] | **id**

