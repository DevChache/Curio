## Frontend Model of Curio
\#\# (Source Code)

=> **Lexical Analysis** -> Lexical Element (Unit)

=> **Syntax Analysis** -> Parse Tree

=> **Intermediate Code Generation** -> Three-address Code 

\#\#

When operating on the **Lexical Analysis**, **Syntax Analysis** and **Intermediate Code Generation**, a symbol table is needed.

#### Lexical Analysis
Lexical analysis is the first step of a compiling process. The lexical analysis, or scanning, reads the character stream which forms the source program and organizes them into meaningful sequence of lexemes.

The lexical analysis uses ***token*** as its output production:

```
&lt;token-name, attribute-value&gt;
```
The token has another name as Lexical Element (Unit).

The whitespace that seperate the tokens will be ignored by the scanning program.
