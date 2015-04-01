[![Logo](http://www.rolisteam.org/sites/default/files/pixture_reloaded_logo.png)](http://www.rolisteam.org)
# Documentation:

## Irc and Chat

Please, remember it is important to preffix all you command by `!`. This will allow the system to identify your command. To clarify the documentation, the `!` is not repeated before all commands.

## How to roll a die 

It is real simple. you have to call:
> 1d6

The first number is the count of dice you want to roll. The second number should be die's faces count.

### Examples
> 1d6

Roll one six-faced die.

> 1d10

Roll one ten-faced die.

> 5d10

Roll five ten-faced die.

> 777d6

Roll 777 six-faced die.


Thanks of several operations and option, you can tune a bit you rolling command.

## List of operator
* k : Keep
* K : Keep And Explose
* s : Sort
* c : Count
* r : Reroll
* e : Explose
* a : Reroll and add

### Keep

> kX

The option sorts the resulting die list and select the X higher dice.

### Keep And Explose

> KX

Dice explose if their value are at the die maximum, the option sorts the resulting die list, the it selects the X higher dice.

### Keep Lower dice

> klX

The option sorts the resulting die list and select the X lowest dice.

### Keep Lower dice

> klX

Dice explose if their value are at the die maximum, the option sorts the resulting die list, the it selects the X lowest dice.

### Sorting

> 3D10s

The dice list is sorted in descending order.

### Counter

> 3D10c[Validator]

Count how many dice respect the condition and display the number (See Validator for more details about syntax)

### Reroll

> 3D10r[Validator]

Reroll the die if the previous value fits the validator (See Validator for more details about syntax).

### Explose

> 3D10e[Validator]

Explose while the value fits the Validator (See Validator for more details about syntax).

### Add

> 3D10a[Validator]

Reroll the die if its value fits the Validator and add the new value to the previous one. It does that only once.


## Arithmetic

Rolisteam Dice Parser is able to compute primary arithmetic operation such as: +, -, /, * and it also manages those operator priority and it can also manage parenthesis. 

> 8+8+8

Result: 24

> 24-4

Result: 20

> (3+4)*2

Result: 14

> 7/2

Result: 3.5

> (3+2D6)D10

Roll 2 dice and add 3 to the sum of those dice. Then the result is used for rolling dice.


## Arithmetic and Dice

It is possible to use arithmetic opearation on dice. Please pay attention that the default operation to translate a 
dice list to scalar is the sum. So if you roll `3d6`, the result will be a list with 3 values {2, 5 ,1}. Now, we 
change a bit the command `3d6+4`: It is resolved like this: {2, 5 ,1} = 8; 8+4 = 12. The final result is 12.

> 3d6+4

Roll 3 dice; sum the result; and add 4

> 10D10-2

Roll 10 dice; sum the result; and then substract 2

> 87-1D20

Substract the result of 1 die to 87

> (6-4)D10

Substract 4 to 6 and then roll two dice. 

> 1D10/2

Divide by 2 the result of 1 die.

## Roll two (or more) kind of dice at once.

To make it, you have to separate all dice commands by `;`

> 1d10;1d6

or

> 5d6;1d10;4d100;3d20

## Validator

There are three kind of Validator:
-Scalar
-Range
-Boolean expression


Any operator which requires validator (such as `a,r,e,c') can use those three kind.

### Scalar 

The scalar value sets the validator on eguality between the dice value and the validator

> 4d10e10

This command means : roll 4 dice and they explose on 10.

### Range

The range is defined as two bounds. You have to use square brackets and the two bounds are separated by `-`.

> 4d10c[8-10]

### Boolean Condition

The command counts how many dice have values between >=8 and <=10.

> 4d10c[>7]

The command counts how many dice are aboved 7.

#### Logic Operator

The Rolisteam Dice Parser allows you to use several logic operator:
* Egual : =
* Greater or egual :  >=
* Lesser or egual : <=
* Lesser : <
* Greater : >

## Examples

> 3D100

Roll 3 dice with 100 faces

> 10D10e[=10]s

Roll 10 dice with 10 faces, 10 exploses, and sort the result.

> 100291D66666666s

Roll 100291 dice with 66666666666 faces and sort result

> 15D10c[>7]

roll 15 dice with 10 faces and it counts number of dice which are above 7

> 1D8+2D6+7

roll 1 die with 8 faces and add the result to 2 dice with 6 faces and add 7.

> D25

roll 1 die with 25 faces

> 88-1D20

88 minus the value of 1 die of 20 faces

> 8+8+8

compute: 24

> 1L[sword,bow,knife,gun,shotgun]

One of this word will be picked. 

> 8D10c[Validator1]-@c[validator2]

Roll 8 dice with 10 faces then it counts how many dice respect the condition Validator1 and substract the number of dice which respect the validator2 and display the number (See Validator for more details about syntax)

> 8D10c[>=6]-@c[=1]

Old World in darkness system.

> 8D10c[>=7]+@c[=10]

Exalted 2nd edition system. 


