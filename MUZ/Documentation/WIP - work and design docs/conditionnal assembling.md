# Conditionnal assembling

***Work in progress - Internal document***

## Conditionnal modes

Conditional embedded assembling is made by maintaining a stack of modes. Each level concerns a whole IF/ENDIF block, and use four different modes depending on the context:

* SKIPTOEND means the lines of code will be ignored until the next ENDIF
* SKIPTOELSE means the lines of code will be ignored until the next ELSE or ENDIF
* DOTOEND means the lines of code will be treated until the next ENDIF
* DOTOELSE means the lines of code will be treated until the next ELSE or ENDIF

An IF always starts a block and pushes DOTOELSE or SKIPTOELSE on the stack depending on the condition. During a SKIP sequence, an IF always enters the mode SKIPTOEND which basically ignores all the lines until the ENDIF of same level, although IF and ENDIF are still checked.

An ENDIF always closes the block and pop the current mode off the stack, if the current mode is at the same level.

In the table below:

* *enter* means a mode is pushed on the stack and the running level is incremented
* *end* means if the current level is the same as the stack size, then the mode is popped off the stack and level is decremented

|Directive found|in root mode|during SKIPTOELSE|during DOTOELSE|during SKIPTOEND|during DOTOEND|
|---|---|---|---|---|---|
|`code`|(assemble)|(ignore)|(assemble)|(ignore)|(assemble)
|`IF false`|enter SKIPTOELSE|enter SKIPTOEND (ignore block)|enter SKIPTOELSE|enter SKIPTOEND (ignore block)|enter SKIPTOELSE
|`IF true `|enter DOTOELSE|enter SKIPTOEND (ignore block)|enter DOTOELSE|enter SKIPTOEND (ignore block)|enter DOTOELSE
|`#ELSE`|error, no current IF|end SKIPTOELSE, enter DOTOEND|end DOTOELSE, enter SKIPTOEND|(ignore)|error, no IF for this ELSE|
|`#ENDIF`|error, no current IF|end SKIPTOELSE|end DOTOELSE|end SKIPTOEND|end DOTOEND

## Expression evaluation

Affter an #IF, the rest of the arguments are defining a boolean expression which will be true if non nul, and faflse if nul.

Evaluation is made using 2-operands operators. 

Syntax is: 

<expression> := <expression> <operator> <expression>




