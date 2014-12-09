# Documentation:

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

Roll five ten-faced die.


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

The option sorts the resulting die list and select the X better.

### Keep And Explose

> KX

Dice explose if their value are at the die maximum, the option sorts the resulting die list, the it selects the X better.

> 3D10s

The dice list is sorted in descending order.


> 3D10c[Validator]

Count how many dice respect the condition and display the number (See Validator for more details about syntax)

> 3D10r[Validator]

Reroll the die if the previous value fits the validator (See Validator for more details about syntax).

> 3D10e[Validator]

Explose while the value fits the Validator (See Validator for more details about syntax).

> 3D10a[Validator]

Reroll the die if its value fits the Validator and add the new value to the previous one. It does that only once.


## Arithmetic

Rolisteam Dice Parser is able to compute primary arithmetic operation such as: +, -, /, * and it also manages those operator priority and it can also manage parenthesis. 

> 8+8+8

Result: 24

> 24-4

Result: 20



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

> 100/28*3



