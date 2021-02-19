[![Logo](https://raw.githubusercontent.com/Rolisteam/rolisteam/master/resources/logo/1000-rolisteam.png)](http://www.rolisteam.org)

# Table of Contents

* [DiceParser](#diceparser--what-is-it-)
* [Roll a die](#how-to-roll-a-die)
* [List of operator](#list-of-operator)
    * [Keep](#keep)
    * [Explode and Keep](#explode-and-keep)
    * [Keep Lower dice](#keep-lower-dice)
    * [Sort](#sort)
    * [Count](#count)
    * [Reroll](#reroll)
    * [Reroll until](#reroll-until)
    * [Explode](#explode)
    * [Add](#add)
    * [Occurence](#occurence)
    * [Backward Jump](#backward-jump)
    * [Paint](#paint)
    * [Merge](#merge)
    * [Filter](#filter)
    * [if](#if)
    * [Group](#group)
    * [Spread](#spread)
    * [Unique](#unique)
    * [All The same](#allsame)
    * [Value list](#Value-list)
    * [Comment (\#)](#comment-)
* [Functions](#Functions)
* [Managing the output](#the-output)
    * [Shortcuts](#shortcuts)
    * [Final Result](#final-result)
    * [Dice Result](#dice-result)
* [Arithmetic](#arithmetic)
* [Arithmetic and Dice](#arithmetic-and-dice)
* [Validator](#validator)
    * [Scalar](#scalar)
    * [Boolean Condition](#boolean-condition)
    * [Operation Condition](#operation-condition)
    * [Composite Validator](#composite-validator)
* [List operator](#list-operator)
    * [Text](#text-values)
    * [Number](#number-values)
    * [Change the odd](#change-the-odd)
* [Miscellaneous examples](#examples)
* [Best Practices](#best-practices)
* [Platforms](#roll-dice-on-each-platform)
* [Discord bot](#discord-bot)
* [Bug report](#bug-report-and-new-features)

# Documentation:

## DiceParser: What is it ?

DiceParser is a software component dedicated to roll dice through simple commands. This software component is available on different platform.  
Such as: Irc bot, discord bot, included in Rolisteam, web server, on twitter etc.

## About examples in this documentation

To make it clear, all examples in this documentation do not show the start up prefix.
Please, remember to add the proper prefix given where you run dice command: Rolisteam, discord, IRC…
If you don't know, try `!`.
The prefix allows the system to identify your command.

## How to roll a die

It is real simple. you have to call:
```
1d6
```

The first number is the count of dice you want to roll. The second number should be die's faces count.

### Examples

```
1d6
```

Roll one six sided die.

```
1d10
```

Roll one ten sided die.

```
5d10
```

Roll five ten sided die.

```
777d6
```

Roll 777 six sided die.

Thanks of several operations and options, you can tune a bit your rolling command: see [List of operator](#list-of-operator).

### Roll dice in Range

```
4d[-1..1]
```

Rolling 4 dice with value between -1 to 1. (Fudge/Fate system)

```
3d[0..9]
```
Rolling 3 dice with 10 faces starting at 0.

```
3d[-20..-9]
```
Rolling 3 dice, values are between -20 and -9.


### Instruction: Roll two (or more) kinds of dice at once

Adding (or any arithmetic operations) results from two (or more) kinds of dice is easy:

```
1d10+1d8
 ```

To display all results without making any operations with those results. Use `;` to mark several instructions.

```
1d10;1d6 # 2 instructions
```

or

```
5d6;1d10;4d100;3d20  # 4 instructions
```


### Merge

It is possible to merge every instruction inside a huge one.
The operator merge is dedicated to that.
It is useful when you need to manage all diceresult as the same result.

For example, if you need to keep the higher dice between a d6 and d8.

```
d6;d8mk1
```

More details about k operator in [List of operator](#list-of-operator) .

### Computation between instructions

Thanks to variable system, it is possible to reference the result of a specific instruction.
- To reference the first instruction: `$1`
- To reference the second instruction: `$2`
- To reference the third instruction: `$3`
etc…
the number of instruction is not limited.

```
8d10;$1c[>6];$1c1;$2-$3
```

* The first instruction rolls 8 (10 sided) dice
* The second instruction counts how many dice are higher than 6.
* The third instruction counts how many dice are equal to 1.
* The fourth instruction subtracts the result of the third instruction to the result of seconde one.


## List of operator

* k: Keep
* K: Explode and keep
* kl: Keep lower
* s: Sort
* c: Count
* r: Reroll
* R: Reroll until
* e: Explode
* a: Reroll and add
* @: Backward Jump
* p: Paint dice
* m: Merge
* i: if
* ;: Next instruction
* g: Group
* b: bind
* \#: Comment

### Keep

```
kX
```

The option sorts the resulting die list and select the X higher dice.

### Explode and Keep

```
KX
```

Dice explode if their value are at the die **maximum**, the option sorts the resulting die list, the it selects the X higher dice.

#### Examples

```
6d10K4
```

Roll 6 10-sided dices, each 10 explodes. So the value of exploded dices are greater than 10.
Result: 40 details: 23 [10,10,3],9,5,3,1,1

Another way to write this command is:
```
6d10e10k4
```
This way allows you to change the explode threshold.

### Keep Lower dice

```
klX
```

The option sorts the resulting die list, the it selects the X lowest dice.

### Sort

```
3D10s
```

The dice list is sorted in descending order.

```
10d6sl
```

It rolls 6 dice at 6 faces and then they are sorted in ascending order

### Count

```
3D10c[Validator]
```

Count how many dice respect the condition and display the number (See Validator for more details about syntax)

### Reroll

```
3D10r[Validator]
```

Reroll the die if the previous value fits the validator (See Validator for more details about syntax).

### Reroll until

```
3D10R[Validator]
```

Works like "Reroll", but continue to roll the dice until the condition is false.

### Explode

```
3D10e[Validator]
```

Explode while the value fits the Validator (See Validator for more details about syntax).

### Examples

```
3D10e10
```
While dice value is equal to 10, the dice is rolled again and its result is added to the previous dice value.
> Result: **49 details: 8, 12 [10,2], 29 [10,10,9]**

```
3D10e[>8]
```
While the dice value is greater than 8, the dice is rolled again and its result is added to the previous dice value.
> Result: **35 details: 3, 27 [9,10,8], 5**

### Add

```
3D10a[Validator]
```

Reroll the die if its value fits the Validator and add the new value to the previous one. It does that only once.

### Occurence

```
10d10o
```

Count and sort occurences of each value.
Result: `3x1,1x2,3x4,2x9,1x10 - [1,1,1,2,4,4,4,9,9,10]`

```
10d10o2,7
```

Count and sort occurence when they occur at least 2 times, the value is 7 or more.
Result: `2x8 - [1,2,3,5,6,7,8,8,9,10]`

```
10d10o2[<6]
```

Count and sort occurence when they occur at least 2 times, the value should respect the validator (here less than 6).
Result: `2x3,2x5 - [3,3,5,5,6,6,6,7,7,8]`


#### Errors

```
10d10o[<6]
```

This command is triggering a warning. As occurence operator can have 0 or 2 parameters. But only one validator is unsupported yet.



### Backward Jump

This operator is dedicated to apply its next operator to the second to last result.
For example:

```
8D10c[>=7]+@c[=10]
```

c[=10] in this command is counting the number of 10 in the result of 8D10, if you remove the @, it will try to count the number of 10 in the result of c[>=7]. The result of c[>=7] is a scalar number (1 or 2 ... (max value 8)); it is not dice list.

### Paint

```
8D10p[1:blue]
```

Paint the first die in the list in blue

```
8d10p[2:blue]
```

Paint the two first dice in the list in blue.

https://gist.github.com/obiwankennedy/62101383d411e55d205e44df78aa5299


The amount of color is depending of client application of DiceParser.
- With Rolisteam, you may set any Qt color's name or set the Hexcode of your color: #ff28AC.
- The cli application supports few colors: black, white, blue, red, black, green, yellow.


### Filter

Filter operator allows you to ignore some dice result given a validator.

```
4d6f[!=6]
```

Result:
> total: 11 - details[5 2 6 4]

the 6 is ignored.

### Merge

Merge operator is used for gathering several dice rolls from different die type into one dice result and then you may apply any kind of operator.

```
1d6;1d8mk1
```

This command merges together the result from the d6 and the d8. Then, it applied the k operator on both result to keep the best.
Be careful, this operator merges the instruction list. Instruction reference (such as $1 etc..) won't work after merge operator.

### Spead

It makes exploded dice as new dice.
The operator is trigged by *y*.

```
4d6e6y6k3
```

First Result: `10 [6, 4], 3, 3, 2`  
Result after spead: `6, 4, 3, 2`  
Final result: `6+4+3 = 13`  

### All the same

This operator is temporary. It is dedicated to answer issue about Tunnels and Trolls system. It is why the marker operator is `t`.
Dice explode when all dice has the same value.

```
2d6t
```

> # Explode twice because 2,1
Result: 12 - details:[2d6t (5 [2,1,2] 7 [2,1,4])]

> # Nothing happened
Result: 10 - details:[2d6t (4 6)]


### Unique

It makes exploded dice as new dice.

```
4d6e6u
```
Result: 6 4 3 3 2
Final result: 6+4+3 = 13

### Value list

Build your own value list and apply any dice operator.

```
[10,25,43,8]k1
```

Get the higher score from several instruction:

```
1d10;2d6+9;1d20;[$1,$2,$3,$4]k1
```

Each value is transformed into a die.

### Bind

Bind works exactly as merge but it keeps instruction array untouched.

```
!2d8;2d12b;$2k2;$2k2kl1;"your total is $3 with lowest: $4"
```

Roll two 8-sided dice and two 12-sided dice then bind their results. using this final result, we keep the 2 higher dice and then we isolate the lowest of the two highest.
At the end, we display the result inside a sentence.

### if

If operator means to allow you to do one thing if some conditions are true.
The if operator has 2 mandatory parameters:
* The condition (see validator)
* the instruction to do when it is true.

There is also 2 optional parameters
* the compare method
* the instruction to do when it is false.

> i*[]{}{}

* \*: the compare method
* []: the validator
* {}: the true instruction
* {}: the false instruction

#### Compare method

There are 4 different methods.
* **On Each**: the condition is tested on each die from the previous part of the command. \[Default method\]
* **On Each value** `?`: the condition is tested on each final value of die from the previous part of the command.
* **All Of Them** `*`: All dice must fit the condition to trigger the true instruction. If all dice do not fit the condition the false instruction is run.
* **One Of Them** `.`: at least one die must fit the condition to trigger the true instruction. If no dices fit the condition the false instruction is run.
* **On Scalar** `:`: the condition is evaluated on the scalar result of the dice roll.

#### Examples:

```
1d6i[<4]{3}
```

If the value of the die is less than 4, the die value is 3. (So 1, 2 , 3 become 3).

```
4d6e6i[=4]{-4}
```

If die has 4 as value, it remove it. \[Kuro System\]

```
4d6i.[=6]{+1d6}
```

if at least one die is equal to 6, then roll another d6 and add it to the result.

```
4d6i*[=6]{+1d6}
```

if all dice are equal to 6, then roll another d6 and add it to the result.

```
2d10i:[>15]{"Success"}{"Fail"}
```

if the sum of two dice is greater than 15, It displays "Success", otherwise it displays "Fail".

```
2d10i:[>15]{"Success %1 "}{"Fail %1"}
```

Same as above, but the final result is displayed beside Success or Fail.

```
 2d10i:[>15]{"Success %1 [%2]"}{"Fail %1 [%2]"}
```

Same as above, but the result of each die is displayed inside square brackets.


### Group

Group dices, then count the number of group (7th sea system).

#### Complex output

Group operator can take a parameter to active the complex output.
This output will show the each group and any left aside values if any.
To active this output, it is required to add a `s` just after the `g`. See example below:

#### Example

```
3d20g10
```

This will roll 3 dices and then try to group them to make groups of 10. If you get `9 9 2`, you can only create one group whose value is more or equal to ten (`{9,2}`, the second `9` being "wasted").

The `g` operator is allowed to re-order dices to create groups. When rolling `4d20g10`, if you get `7 4 3 6`, the result will be `2` (`{7,3}` and `{6,4}`).

```
5d10gs10
```

Then, the final output will be:
```
2 ({7,3}, {6,4} - [2])
```

`{7,3}` and `{6,4}` are group, and `[2]` is left aside.

### Comment (\#)

```
2D6 # Sword attack
```

Display "Sword attack" and the result of the two dice.
DiceParser ignore everything after the \#. The whole part is treated as one comment.
So DiceParser can answer question:

```
1L[yes,no] # Am I evil ?
```

> Am I evil ?
yes


## Functions

DiceParser provides function to deal with instructions.
Some functions will come soon (e.g: max, min). It will allows to manage several commands at once.

### Repeat

> repeat(1d10,5)

Output:
```
2 - Details [2]
8 - Details [8]
3 - Details [3]
1 - Details [1]
10 - Details [10]
```

Attention! Be careful, `repeat` works badly with multiple instruction commands

## The output

DiceParser provides features to let you control the command output.
The final instruction must be a string instruction.
String instruction starts with `"` and ends with `"`.

Rolling:
> `"result"`

Output:
`result`

You can set string instruction inside if operator:

> 1d6i:[>3]{"Success"}{"Fail"}

Output:
`Success` or `Fail`


It offers a quick answer but sometimes you need to see the rolled values.
DiceParser can replace some special tags in order to see values, computation result and whatever.

### Shortcuts

There are 3 shortcut tags.

* `%1`: last scalar result from each instruction.
* `%2`: all dice results.
* `%3`: last scalar result from the last instruction.

The default output is `%1 details[%2]`.
So, it shows the last scalar result of each instruction and dice result.

`%1` and `%3` are equivalent when there is only one instruction (no \;).

They are really useful but if you have many instructions that can become a bit messy.

### Final result

It is also possible to set reference to the final value of specific instruction (the result should be a number or a string)

- To reference the first instruction: `$1`
- To reference the second instruction: `$2`
- To reference the third instruction: `$3`

There is no limit on instruction number.


#### String as final result

You can reference the sub result of a string result by adding `[x]` after the instruction reference.
Let see an example, it will be easier to understand.

```
2Lu[cats,dogs,rats,rabbits,bats,squirrels]
```

The default result looks like this:

> cats,bats

Now we want to make a sentence with this text:

```
2Lu[cats,dogs,rats,rabbits,bats,squirrels];"You love $1 and you are afraid of $1"
```

As $1 refers to "**cats,bats**", it will show:

> You love cats,bats and you are afraid of cats,bats


So, it is not really useful. In order to make it a bit better, we must add some sub-indexes.

```
2Lu[cats,dogs,rats,rabbits,bats,squirrels];"You love $1[0] and you are afraid of $1[1]"
```

then we have a proper output.

> You love cats and you are afraid of bats


#### Let see some examples:

```
8d10;$1c[>6];$1c1;$2-$3
```

The default output displays: `45,4,0,4 details[4,3,10,7,2,2,7,10]`

```
8d10;$1c[>6];$1c1;$2-$3i:[>0]{"%3 Success[%2]"}{i:[<0]{"Critical fail %3 [%2]"}{"Fail %3 [%2]"}}
```

Here, some output example:
* `4 Success[4,3,10,7,2,2,7,10]`
* `Fail 0 [10,3,1,1,2,2,7,5]` (2 success - 2 fails = 0)
* `Critical fail -2 [1,3,1,1,2,2,7,5]` (1 success - 3 fails = -2)

In this example, the critical fail happens when there are more fails than success.
In the next example, the critical fail happens when there was no success and a least one fail.

```
8d10;$1c[>6];$1c1;$2-$3;$4i:[=0]{"Fail $4 [%2]"}{$4i:[>0]{"$2 Success[%2]"}{$2i:[=0]{"Critical Fail $4 [%2]"}{"Fail $4 [%2]"}}}
```

Another example, to show how to combine string and dice result.

```
1d6+1;1L[gold coins,spell scrolls,weapons];"You found $1 $2"
```

> You found 5 gold coins



### Dice Result

DiceParser provides tags to display dice result (and each rolled values from a specific instruction).

To show dice values from a specific instruction, just add `@` followed by the instruction's number (e.g: `@1`)

```
2d6;3d8;"Result $2 - d8:[@2] - d6:[@1]"
```

The output:

> Result 15 - d8:[7,4,4] - d6:[3,6]`


### New line

You may need to display your result on several lines. It is really easy:

```
1d100;1d10;"Attack: $1\nDamage: $2"
```

This command will display:

> Attack: 31
Damage: 7

## Arithmetic

Rolisteam Dice Parser is able to compute primary arithmetic operation such as: +, -, /, * and it also manages those operator priority and it can also manage parenthesis.

```
8+8+8
```

> Result: 24

```
24-4
```

> Result: 20

```
(3+4)*2
```

> Result: 14

```
7/2
```

> Result: 3.5

```
(3+2D6)D10
```

> Roll 2 dice and add 3 to the sum of those dice. Then the result is used for rolling dice.

```
15|6
```

> Result: 2

```
15/6
```

> Result: 2.5

## Arithmetic and Dice

It is possible to use arithmetic operation on dice. Please pay attention that the default operation to translate a
dice list to scalar is the sum. So if you roll `3d6`, the result will be a list with 3 values {2, 5 ,1}. Now, we
change a bit the command `3d6+4`: It is resolved like this: {2, 5 ,1} = 8; 8+4 = 12. The final result is 12.

```
3d6+4
```

> Roll 3 dice; sum the result; and add 4

```
10D10-2
```

> Roll 10 dice; sum the result; and then subtract 2

```
87-1D20
```

> Subtract the result of 1 die to 87

```
(6-4)D10
```

> Subtract 4 to 6 and then roll two dice.

```
1D10/2
```

> Divide by 2 the result of 1 die.

```
(2+2)**2
```
> Result: 16

```
1d10**2
```

> Roll 1d10 then multiply the result by itself.

```
15|2
```
> Integer division of 15 by 2. Result: 7

```
15/2
```
> Division of 15 by 2. Result: 7.5

## Validator

There are five kinds of Validator:

* Scalar
* Range
* Boolean expression
* Operation Condition
* Composite

Any operator which requires validator (such as `a,r,e,c`) can use those three kind.

### Scalar

The scalar value sets the validator on eguality between the dice value and the validator

```
4d10e10
```

This command means: roll 4 dice and they explode on 10.

### Range

The range is defined as two bounds. You have to use square brackets and the two bounds are separated by `..`.

```
4d10c[8..10]
1d[-1..8]
```

### Boolean Condition

The command counts how many dice have values between >=8 and <=10.

```
4d10c[>7]
```

The command counts how many dice are aboved 7.

#### Compare Operator

The Rolisteam Dice Parser allows you to use several logic operator:
* Egual: `=`
* Greater or egual:  `>=`
* Lesser or egual: `<=`
* Lesser: `<`
* Greater: `>`
* Different: `!=`


#### Compare methods

As the `if` operator, you can specify the compare method.

* **On Each**: the condition is tested on each die from the previous part of the command. \[Default method\]
* **On Each value** `?`: the condition is tested on each final value of die from the previous part of the command.
* **All Of Them** `*`: All dice must fit the condition to trigger the true instruction. If all dice do not fit the condition the false instruction is run.
* **One Of Them** `.`: at least one die must fit the condition to trigger the true instruction. If no dices fit the condition the false instruction is run.
* **On Scalar** `:`: the condition is evaluated on the scalar result of the dice roll.

##### Examples:

###### Scalar comprehension failure
```
1L[7,8,9]c[>6]
```

This command will return 0 because, no die has been rolled, so the result of `1L[7,8,9]` is a final value.


###### Counting scalars

```
1L[7,8,9]c[?>6]
```

Output: 1

###### Counting exploding dice

**Incorrectly**
```
5d6e6sc[>=8]
```
Output:

> 0
details: [8 [6,2] 2 1 1 1]

**With a filter on the final sum**
```
5d6e6f[?>=16]
```

Output:
As the final sum is equal to 11. It's less than 16 so the filter is filtering everything.

> 0
details: [2 4 1 3 1]

The final sum is higher than 16 so the whole result is accepted by the filter operator.

> 23
details: [3 6 3 5 6]

**With a count on each roll**
```
5d6e6sc[:>=8]
```

Output:
> 1
details: [8 [6,2] 2 1 1 1]

**Getting the minimum of two rolls**
```
1d100;1d100;$1i:[>$2]{$2}{$1};"Final value: $3 details [$1 - $2]"
```

Output:
> Final value: 9 details [24 - 9]


### Operation Condition

This validator offers modulo as operation and a Boolean Condition to validate the value:

```
4d10c[%2=0]
```

Count how many even numbers have been rolled.


```
4d10c[%2=1]
```

Count how many odd numbers have been rolled.

[modulo](https://en.wikipedia.org/wiki/Modulo_operation)

### Composite Validator

Validator can be the result of several validator.

```
4d10c[>4&%2=0]
```

Count all dice greater than 4 and even [6,8,10].

Composite Validator supports 3 logical operations:

* AND : `&`
* OR : `|`
* Exclusive OR : `^`

Composite Validator accepts as many validator as you need:

```
!9d100c[=1|=3|=5|=7|=11|=13|=17|=19|=23|=29|=31|=37|=41|=43|=47|=53|=59|=61|=67|=71|=73|=79|=83|=89|=97]
```

## List operator


* [Number](#number-values)
* [Change the odd](#change-the-odd)

### Text values

The L operator (meaning list) provides a way to pick up value from list.

```
1L[sword,bow,knife,gun,shotgun]
```

With comment

```
1L[yes,no] # Am I evil ?
```

> Am I evil ?
yes


### Number values

If the value is a number, it is treated as well and you can do computation on it or use any operator.

```
1L[-1,0,1,2,3,4]+7
```

### Text and Number at the same time

It is not recommended to use text and number in the same list operator.
Currently, the behaviour changes given the result. If the chosen value is a number, you can do other computation, but otherwise, the result is displayed directly without any control on it.

The behaviour will change in future release to base the decision on the data set. If the data set only contains numbers, then computation is possible. Otherwise, it will be treated as string result.


### Change the odd

There are 2 main ways to control the odd on the pickup in the list.

#### The ant method

```
1L[2,2,3,3,3,3,4,4,4,5]
```

or

```
1L[arm,arm,leg,leg,chest,chest,chest,head]
```

#### The lazy method

By range:

```
1L[1,2,3,4,5,6[6..10]]
```


By weight:

```
1L[1[2],2[2],3[4]]
```

Several results:

```
1
3
1
2
2
1
2
3
1
2
3
2
2
3
```

## Examples

```
3D100
```

Roll 3 dice with 100 faces

```
10D10e[=10]s
```

Roll 10 dice with 10 faces, 10 explodes, and sort the result.

```
100291D66666666s
```

Roll 100291 dice with 66666666666 faces and sort result

```
15D10c[>7]
```

roll 15 dice with 10 faces and it counts number of dice which are above 7

```
1D8+2D6+7
```

roll 1 die with 8 faces and add the result to 2 dice with 6 faces and add 7.

```
D25
```

roll 1 die with 25 faces

```
88-1D20
```

88 minus the value of 1 die of 20 faces

```
8+8+8
```

compute: 24

```
1L[sword,bow,knife,gun,shotgun]
```

One of this word will be picked.

```
8D10c[Validator1]-@c[validator2]
```

Roll 8 dice with 10 faces then it counts how many dice respect the condition Validator1 and substract the number of dice which respect the validator2 and display the number (See Validator for more details about syntax)

```
8D10c[>=6]-@c[=1]
```

Old World in darkness system.

```
8D10c[>=7]+@c[=10]
```

Exalted 2nd edition system.


## Best Practices

As DiceParser provides more and more features, you may find several ways to do the same thing. We want here to explain the difference between those several approaches. Then you will be able to use the right one.


### Roll several kind of dice and sum them

**Bad**
```
2d8;2d10m
```

**Good**
```
2d8+2d10
```

The merge operator is useful when you want to use dice operator on all rolled dice.

**Bad**
```
1d20K1+1d10K1
```

**Good**
```
1d20+1d10
```

The k operator to keeps as many dice as you roll is pretty useless because it is the default behaviour.

## Platform specific documentation

### Roll dice on each platform

| platform | start character(s)  | more information |
|---|---|--- |
| Rolisteam |```!```| [Documentation](http://wiki.rolisteam.org/index.php/En:Dice) |
| Discord  |```!```| To install DiceParser on your server [http://www.rolisteam.org/discord.html](http://www.rolisteam.org/discord.html)  |
| Twitter  | ```#roll```  | Twit any message starting with #roll following by dice command (e.g: ```#roll 2d6```) |
| IrcBot   |```!```|  |
| webserver   | nothing |  No public published yet. Has dedicated text field |
| dice   | nothing | dice is a command line application to roll dice: ```dice "2d6"``` |


### Known Limitations

| platform | descriptions |
|---|---|
| Rolisteam | no support for comments yet. Rolisteam is a big software. You may not have all the most recent feature from diceparser. |
| Discord  | If the command takes too much time. It is canceled |
| Twitter  | Result should be short. No aliases |
| IrcBot   | No aliases |
| webserver| No aliases |
| dice     |  |

# Discord Bot

Documentation of the bot is now inline with the bot.

## Prefix documentation

> !help prefix

## Macro documentation

> !help macro

[Examples and More information](https://invent.kde.org/rolisteam/rolisteam-diceparser/-/blob/master/community_macro.md)

## Alias documentation

> !help alias

## Difference between Aliases and Macros

Aliases is the feature of the bot, macro is a feature of the dice system behind the bot.
Aliases is a way to run several commands at once.

Macro is a way to run huge command by typing few letters. Macros may have parameters (such as dice number, threshold for success or whatever)

# Bug report and new features

Please fulfill a ticket in our [Bug tracker](https://invent.kde.org/kde/rolisteam-diceparser/issues/new) system.
Or contact us on [discord](https://discordapp.com/invite/MrMrQwX) or any [other ways](http://www.rolisteam.org/contact.html)
