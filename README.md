[![Logo](https://raw.githubusercontent.com/Rolisteam/rolisteam/master/resources/logo/1000-rolisteam.png)](http://www.rolisteam.org)

# DiceParser

Rolisteam Dice Parser run dice commands. It is available on several platforms.
The syntax is simple and powerful.

[Full documentation here](https://invent.kde.org/rolisteam/rolisteam-diceparser/-/blob/master/HelpMe.md)


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


More examples at : https://invent.kde.org/rolisteam/rolisteam-diceparser/-/blob/master/HelpMe.md

## Grammar
The grammar is something like this:

```
Program =: Instruction [InstructionSeparator, Instruction]* Comment
InstructionSeparator = ;
Instruction =: Expression ([Operator, Expression]* | [Option]*)
Operator =: ScalarOperator
Expression =: OpenParenthesis Expression closeParenthesis
| Option*
| [Operator, Expression]*
| Operand Dice
| Command
| function
| NodeOperator [Option]*
| ValuesList
| Dice (Operand == 1)
Operand =: DynamicVariable | Number | String
OpenParenthesis = (
closeParenthesis = )
OpenList = [
CloseList = ]
ListSeparator = ,
ValuesList=: OpenList (DynamicVariable | Number)? [ ListSeparator,(DynamicVariable | Number)]*  CloseList
Dice =: DiceOperator [uniqueValue] DiceParameter
DiceOperator =: D ParameterDice | L ParameterList
DiceParameter =: ParameterDice | ParameterList
ParameterDice =: Number|Range
ParameterList =: List
List=: OpenList String[Probability] [ListSeparator,String[Probability]]* CloseList
Probability=: OpenList (Range|Percentage) CloseList
Percentage =: number
function =: functionName OpenParenthesis [function_args] closeParenthesis
function_args =: Instruction [InstructionSeparator, Instruction] | Operand | ValidatorList
functionName =: repeat
Option =: Keep
| KeepAndExplode
| Filter
| Sort
| Count
| Reroll
| RerollUntil
| RerollAndAdd
| Explode
| Merge
| Bind
| Occurences
| Unique
| Paint
| If
| Split
| Group
Range =: OpenList Number RangeSeparator Number CloseList
RangeSeparator =: ..
ScalarOperator =: [x,-,*,x,/,**]
number =: [-] [0-9]+ | constantValue
OpenVaribale=: ${
CloseVariable=: }
constantValue =: OpenVaribale (id | label) CloseVariable
id=[_,a-z][_,A-z,0-9]* # must respect rules of QML id
label=.*
variable = OpenVaribale [0-9]+ CloseVariable
ValidatorList =: OpenList CompareMethod Validator [LogicOpetator CompareMethod Validator]* CloseList
LogicOpetator =: AND | XOR |  OR
CompareMethod =: Each |  All | Scalar | ANY
Each=:
All=: *
Scalar=: :
ANY=: .
AND =: &
XOR =: ^
OR =: |
Ascendant=:l
Validator =: BooleanValidator | RangeValidator | OperationValidator
CompareOpetator =: = | > | >= | < | <= | !=
RangeValidator =: Range
OperationValidator =: Modulo operandNode BooleanValidator
Modulo =: %
BooleanValidator =: [=]Operand | [CompareOpetator Operand]
ListOfValue=: String[Range],ListOfValue | String[Range]
String =: .*[^ListSeparator]
Keep =: k[Ascendant] Number
KeepAndExplode =: K[Ascendant] number
Filter =: f ValidatorList
Sort =: s[Ascendant]
Count =: c ValidatorList
Reroll =: r ValidatorList
RerollUntil =: R ValidatorList
RerollAndAdd =: a ValidatorList
Merge =: m
Bind =: b
Occurences =: OccurencesWidth ( ListSeparator  number | ValidatorList)
OccurencesWidth =: number
unique =: u
Painter =: p PainterParameters
PainterParameters =: OpenList PairColorOccurence [ListSeparator , PairColorOccurence]* CloseList
PairColorOccurence =: Color PairSeparator Number
PairSeparator =: :
If =: i [compareMethod] ValidatorList Bloc[Bloc]
compareMethod =: OnEach | OneOfThem | AllOfThem | onScalar
OnEach =: ''
OneOfThem = '.'
AllOfThem = '*'
onScalar = ':'
Bloc =: OpenBranch Expression CloseBranch
OpenBloc =: {
CloseBloc =: }
Split =: y
Group =: g Number
Sort =: s
Group =: number
Explode =: e ValidatorList
NodeOperator = Jumpbackward
Jumpbackward =: @
Merge =: m | m Expression
Command =: help | la
uniqueValue = u
Comment =: StartComment String
StartComment =: #
```


# Compilation

```
git clone --recursive https://invent.kde.org/rolisteam/rolisteam-diceparser.git
cd rolisteam-diceparser
mkdir build
cd build
cmake ../ -DBUILD_CLI=ON
make
sudo make install
```
