[![Logo](https://raw.githubusercontent.com/Rolisteam/rolisteam/master/resources/logo/1000-rolisteam.png)](http://www.rolisteam.org)

# DiceParser

Rolisteam Dice Parser run dice commands. It is available on several platforms. 
The syntax is simple and powerful.

[Full documentation here](https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md)


## [Invite to Your Discord Server](https://discordapp.com/oauth2/authorize?&client_id=279722369260453888&scope=bot&permissions=0)

[![Discord Bots](https://discordbots.org/api/widget/279722369260453888.svg)](https://discordbots.org/bot/279722369260453888)


## Features

* `99.9%` uptime
* Roll any kind of dice
* Customizable prefix
* Custom alias/macro to improve game experience
* Manage colorized dice
* Many operators

## Examples:

### 3D100
Roll 3 dice with 100 faces

### 10D10e[=10]s
Roll 10 dice with 10 faces, 10 explodes, and sort the result.

### 100291D66666666s
roll 100291 dice with 66666666666 faces and sort result

### 15D10c[>7]
roll 15 dice with 10 faces and it counts number of dice which are above 7

### 1D8+2D6+7
roll 1 die with 8 faces and add the result to 2 dice with 6 faces and add 7. 

### D25
roll 1 die with 25 faces

### 88-1D20
88 minus the value of 1 die of 20 faces

### 8+8+8
compute: 24

### 100/28*3
compute: 100/28 = 3
3*3 = 9


More examples at : https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md

## Grammar
The grammar is something like this:

```
Command =: Instruction [';',Instruction]*
Instruction =: Expression
Expression =: number | number Dice DiceOperation | ScalarOperator Expression | string | variable Expression
Dice =: DiceOperator Number(faces) | DiceOperator ListOfValues
DiceOperator =: D | L
DiceOperation =: Keep | KeepAndExplode | Sort | Count | Reroll | RerollUntil | If | Explode | Jumpbackward | Merge | Filter | Split | Parenthese | Count | Paint | Group
ScalarOperator =: [x,-,*,x,/]
number =: [0-9]+ | constantValue
constantValue =: ${id | label}
id=[_,a-z][_,A-z,0-9]*
label=[_,a-z][_,A-z,0-9,é,è,ç,û,ê,â]*
variable = ${[0-9]+}
Validator =: BooleanValidator | RangeValidator | CompositeValidator
CompositeValidator =: Validator LogicOpetator Validator
LogicOpetator =: = | > | => | < | =<
RangeValidator =: [ number - number ]
BooleanValidator =: number | [operator number] |
ListOfValue=: String[Range],ListOfValue | String[Range]
String =: [A-z0-9]+
Keep =: k Number
KeepAndExplode =: K number
Reroll =: r
RerollUntil =: R
Exploding =: e
RerollOnceAndAdd =: a
RerollAndAdd =: A
Painter =: p
Split =: u
Group =: g
Sort =: s
If =: i compareMethod [Validator] {Expression}[{Expression}]
Paint =: p [ Count : color ]
Group =: number
Explode =: e Validator
Jumpbackward =: @DiceOperation
Merge =: m | m Expression
Filter =: f Validator
Parenthese =: (expression)
Count =: c Validator
```

