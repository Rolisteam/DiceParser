DiceParser
==========

Rolisteam Dice Parser

The grammar is something like this:

Command =: Expression
Expression =: number | number Dice | ScalarOperator Expression
Dice =: DiceOperator Number(faces)
DiceOperator =: D
ScalarOperator =: [x,-,*,x,/]
number =: [0-9]+
