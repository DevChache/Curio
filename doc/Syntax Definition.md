## Production of Curio

#### NOTES
- The italic styled text refers to a 'statement' or 'expression' in the productions.
- The bold styled text refers to a 'keyword'.
- The normal styled characters such as '(' refers to a terminal symbol.

#### Productions

##### The assignment case:

*stmt* → *var* = *expr* ;
##### The `if-else` case:

*stmt* → **if** ( *expr* ) *stmt* **else** *stmt* | **if** ( *expr* ) *stmt*
