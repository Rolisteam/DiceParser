
[![Build Status](https://raw.githubusercontent.com/Rolisteam/rolisteam/master/resources/logo/1000-rolisteam.png)](http://www.rolisteam.org)

# DiceParser

Dice Parser merges several tool to roll any kind of dice to play at role playing games. 
It can be use as command line application on GNU/Linux
It also provides IRC bot, discord bot, twitter bot and webservice.

[More informations](http://www.rolisteam.org/)

## Grammar
The grammar is something like this:

```
Command =: Expression
Expression =: number | number Dice DiceOperation | ScalarOperator Expression | string
Dice =: DiceOperator Number(faces) | DiceOperator ListOfValues
DiceOperator =: D | L
DiceOperation =: Keep | KeepAndExplose | sort | if | explose | jumpbackward | merge | filter | parenthese | count
ScalarOperator =: [x,-,*,x,/]
number =: [0-9]+
Validator =: BooleanValidator | RangeValidator | CompositeValidator
CompositeValidator =: Validator LogicOpetator Validator
LogicOpetator =: = | > | => | < | =<
RangeValidator =: [ number - number ]
BooleanValidator =: number | [operator number] |
ListOfValue=: String[Range],ListOfValue | String[Range]
String =: [A-z0-9]+
Keep =: k Number
KeepAndExplose =: K number
sort =: s
if =: i compareMethod [Validator] {Expression}
explose =: e Validator
jumpbackward =: @DiceOperation
merge =: m | m Expression
filter =: f Validator
parenthese =: (expression)
count =: c Validator
```

## Build

```
git clone git@github.com:Rolisteam/DiceParser.git
cd DiceParser
mkdir build
cd build
cmake ../
make
make install
```
# Downloads

-DiceParser is part of rolisteam : http://www.rolisteam.org/download but it can be use as standalone tool.

## Example:


### 3D100
Roll 3 dice with 100 faces

### 10D10e[=10]s
Roll 10 dice with 10 faces, 10 exploses, and sort the result.

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
