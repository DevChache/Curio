## Symbol Table

```
\#  | Character | Type          | Alias         | Explanation
1   | numberV   | VariableType  | NUM           | number
2   | ID        | Identifier    | ID            | identifier
3   | number    | Keyword       | BASIC_NUM     | basic type number
4   | bool      | Keyword       | BASIC_BOOL    | basic type bool
5   | true      | Keyword       | TRUE          | bool value true
6   | false     | Keyword       | FALSE         | bool value false
7   | while     | Keyword       | WHILE         | keyword while
8   | if        | Keyword       | IF            | keyword if
9   | else      | Keyword       | ELSE          | keyword else
10  | +         | Operator      | ADD           | add B and A
11  | -         | Operator      | SUB           | subtract B from A
12  | *         | Operator      | MUL           | multiply A by B
13  | /         | Operator      | DIV           | divide A by B
14  | =         | Operator      | ASN           | assignment
15  | ==        | Operator      | EQU           | judge if A equals B
16  | >         | Operator      | GT            | left value is greater than the right one
17  | <         | Operator      | LT            | left value is less than the right one
18  | <=        | Operator      | LE            | left value is less than or equal to the right one
19  | >=        | Operator      | GE            | left value is greater than or equal to the right one
20  | !=        | Operator      | NE            | left value is not equal to the right one
21  | &&        | Operator      | AND           | return true only when both side equals true
22  | ||        | Operator      | OR            | return true when any one side equals true
23  | !         | Operator      | NOT           | return the opposite value of the following expression
24  | ;         | Delimiter     | EDS           | end of statement
25  | (         | Delimiter     | LB            | left bracket
26  | )         | Delimiter     | RB            | right bracket
27  | {         | Delimiter     | BGN           | begin of block
28  | }         | Delimiter     | END           | end of block
29  | program   | Start         | START         | beginning of a program
30  | do        | Keyword       | DO            | do-while loop style
31  | then      | Keyword       | THEN          | if (...) then ... else ...
32  | var       | Keyword       | VAR           | definition
33  | ,         | Delimiter     | COMMA         | comma
34  | :         | Delimiter     | COLON         | colon
```