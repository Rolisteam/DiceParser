[![Logo](https://raw.githubusercontent.com/Rolisteam/rolisteam/master/resources/logo/1000-rolisteam.png)](http://www.rolisteam.org)

**Table of Contents**  

* [DiceParser](#diceparser--what-is-it-) 
* [Platforms](#roll-dice-on-each-platform) 
* [Limitations](#known-limitations) 
* [Roll a die](#how-to-roll-a-die) 
* [List of operator](#list-of-operator) 
* [Arithmetic](#arithmetic) 
* [Arithmetic and Dice](#arithmetic-and-dice) 
* [Validator](#validator) 
* [Value from set](#select-value-from-list) 
* [Miscellaneous examples](#examples) 
* [Best Practices](#best-practices)

# Documentation:

## DiceParser : What is it ?

DiceParser is a software component dedicated to roll dice through simple commands. This software component is available on different platform.  
Such as: Irc bot, discord bot, included in Rolisteam, web server, on twitter etc.

Each platform has its own limitations.
Let's take a look about it:

### Roll dice on each platform


Please, remember it is important to prefix all you command by `!`. This will allow the system to identify your command. To clarify the documentation, the `!` is not repeated before all commands.

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
| Twitter  | Result such be short. No aliases |
| IrcBot   | No aliases |
| webserver| No aliases |
| dice     | No aliases |

We can't set some aliases for any platform but It has to be labelled with game's name.

## Documentation about the Discord Bot

### To change the prefix
> !prefix set newprefix

/!\ Please, don't set "newprefix" as your new prefix. 

### Set the perfix by default

> newprefixprefix set !

### Example

> !prefix set roll

> roll 2d6

> rollprefix set ! 

## Bug report and new features

Please fulfill a ticket in our [Bug tracker](https://github.com/Rolisteam/DiceParser/issues) system.
Or contact us on [discord](https://discordapp.com/invite/MrMrQwX) or any [other ways](http://www.rolisteam.org/contact.html)


## How to roll a die 

It is real simple. you have to call:
> 1d6

The first number is the count of dice you want to roll. The second number should be die's faces count.

### Examples
> 1d6

Roll one six sided die.

> 1d10

Roll one ten sided die.

> 5d10

Roll five ten sided die.

> 777d6

Roll 777 six sided die.

Thanks of several operations and options, you can tune a bit your rolling command: see [List of operator](#list-of-operator).


## Instruction: Roll two (or more) kinds of dice at once 

To make it, you have to separate all dice commands by `;`


> 1d10;1d6 # 2 instructions

or

> 5d6;1d10;4d100;3d20  # 4 instructions


### Merge

It is possible to merge every instruction inside a huge one. 
The operator merge is dedicated to that. 
It is useful when you need to manage all diceresult as the same result.

For example, if you need to keep the high dice between a d6 and d8.

> d6;d8mk1

More details about k operator in [List of operator](#list-of-operator) .

### Computation between instructions

Thanks to variable system, it is possible to reference the result of a specific instruction.
- To reference the first instruction: `$1`
- To reference the second instruction: `$2`
- To reference the third instruction: `$3`
etc…
the number of instruction is not limited.

> 8d10;$1c[>6];$1c1;$2-$3

* The first instruction rolls 8 (10 sided) dice 
* The second instruction counts how many dice are higher than 6.
* The third instruction counts how many dice are equal to 1.
* The fourth instruction substracts the result of the third instruction to the result of seconde one.


## List of operator
* k : Keep
* K : Keep And Explode
* s : Sort
* c : Count
* r : Reroll
* R : Reroll until
* e : Explode
* a : Reroll and add
* @ : Backward Jump
* p : Paint dice
* m : Merge
* i : if
* ; : Next instruction
* g : Group
* \# : Comment

### Keep

> kX

The option sorts the resulting die list and select the X higher dice.

### Explode and Keep

> KX

Dice explode if their value are at the die **maximum**, the option sorts the resulting die list, the it selects the X higher dice.

**Examples**:
> 6d10K4
Roll 6 10-sided dices, each 10 explodes. So the value of exploded dices are greater than 10.
Result: 40 details: 23 [10,10,3],9,5,3,1,1

Another way to write this command is:
> 6d10e10k4
This way allows you to change the explode threshold.

### Keep Lower dice

> klX

The option sorts the resulting die list, the it selects the X lowest dice.

### Sorting

> 3D10s

The dice list is sorted in descending order.

> 10d6sl

Roll 6 dice at 6 faces and then sort them ascendingly

### Counter

> 3D10c[Validator]

Count how many dice respect the condition and display the number (See Validator for more details about syntax)

### Reroll

> 3D10r[Validator]

Reroll the die if the previous value fits the validator (See Validator for more details about syntax).

### Reroll until

> 3D10R[Validator]

Works like "Reroll", but continue to roll the dice until the condition is false.

### Explode

> 3D10e[Validator]

Explode while the value fits the Validator (See Validator for more details about syntax).

**Examples**:
> 3D10e10
While dice value is equal to 10, the dice is rolled again and its result is added to the previous dice value.
Result: 49 details: 8, 12 [10,2], 29 [10,10,9]

> 3D10e[>8]
While the dice value is greater than 8, the dice is rolled again and its result is added to the previous dice value.
Result: 35 details: 3, 27 [9,10,8], 5

### Add

> 3D10a[Validator]

Reroll the die if its value fits the Validator and add the new value to the previous one. It does that only once.

### Roll dice in Range

> 4d[-1-1]

Rolling 4 dice with value between -1 to 1. (Fudge/Fate system)

> 3d[0-9]

Rolling 3 dice with 10 faces starting at 0.

> 3d[-20--9]

Rolling 3 dice, values ars between -20 and -9.

### Backward Jump

This operator is dedicated to apply its next operator to the second to last result.
For example: 

> 8D10c[>=7]+@c[=10]

c[=10] in this command is counting the number of 10 in the result of 8D10, if you remove the @, it will try to count the number of 10 in the result of c[>=7]. The result of c[>=7] is a scalar number (1 or 2 ... (max value 8)); it is not dice list.

### Painter

> 8D10p[1:blue]

Paint the first die in the list in blue

> 8d10p[2:blue] 

Paint the two first dice in the list in blue.

https://gist.github.com/obiwankennedy/62101383d411e55d205e44df78aa5299


The amount of color is depending of client application of DiceParser.
- With Rolisteam, you may set any Qt color's name or set the Hexcode of your color: #ff28AC.
- The cli application supports few colors: black, white, blue, red, black, green, yellow.

### Merge

Merge operator is used for gathering several dice rolls from different die type into one dice result and then you may apply any kind of operator.

> 1d6;1d8mk1

This command merges together the result from the d6 and the d8. Then, it applied the k operator on both result to keep the best.

### if

If operator means to allow you to do one thing if some conditions are true.
The if operator has 2 mandatory parameters:
* The condition (see validator)
* the instruction to do when it is true.

There is also 2 optional parameters
* the compare method
* the instruction to do when it is false.

> i*[]{}{}

* \* : the compare method
* [] : the validator
* {} : the true instruction
* {} : the false instruction

## Compare method

There are 3 different methods.
* **On Each** : the condition is tested on each die from the previous part of the command. \[Default method\]
* **All Of Them** : All dice must fit the condition to trigger the true instruction. If all dice do not fit the condition the false instruction is run.
* **One Of Them** : at least one die must fit the condition to trigger the true instruction. If no dices fit the condition the false instruction is run.
* **On Scalar** : the condition is evaluated on the scalar result of the dice roll.

To switch the operator to act in **All Of Them** method you must add **```*```** character as compare method position.  
To switch the operator to act in **One Of Them** method you must add **```.```** character as compare method position.  
To switch the operator to act in **On Scalar** method you must add **```:```** character as compare method position.  

If you plan to use if operator to display text message. You must surround text with ```"```. Example available below.

## If examples:

>  1d6i[<4]{3}

If the value of the die is less than 4, the die value is 3. (So 1, 2 , 3 become 3).

>  4d6e6i[=4]{-4}

If die has 4 as value, it remove it. \[Kuro System\]

> 4d6i.[=6]{+1d6}

if at least one die is equal to 6, then roll another d6 and add it to the result.

> 4d6i*[=6]{+1d6}

if all dice are equal to 6, then roll another d6 and add it to the result.

> 2d10i:[>15]{"Success"}{"Fail"}

if the sum of two dice is greater than 15, It displays "Success", otherwise it displays "Fail".

> 2d10i:[>15]{"Success %1 "}{"Fail %1"}

Same as above, but the final result is displayed beside Success or Fail.

> 2d10i:[>15]{"Success %1 [%2]"}{"Fail %1 [%2]"}

Same as above, but the result of each die is displayed inside square brackets.




## String Result

To improve readability, it is possible to set the text that should be displayed after the roll.

Several data can be displayed:

* %1 : last scalar result from each instruction.
* %2 : all dice results
* %3 : last scalar result from the last instruction.

The default output is `%1 details[%2]`.
So, it shows the last scalar result of each instruction and dice result.

%1 and %3 are equivalent when there is only one instruction (no \;).


### Specific value from instrustion

It is also possible to set reference to the scalar result of specific instruction.
- To reference the first instruction: `$1`
- To reference the second instruction: `$2`
- To reference the third instruction: `$3`
etc…
the number of instruction is not limited.


### If examples:

> 8d10;$1c[>6];$1c1;$2-$3

The default output displays: `45,4,0,4 details[4,3,10,7,2,2,7,10]`

> 8d10;$1c[>6];$1c1;$2-$3i:[>0]{"%3 Success[%2]"}{i:[<0]{"Critical fail %3 [%2]"}{"Fail %3 [%2]"}}"

The output is now: `4 Success[4,3,10,7,2,2,7,10]`
or
`Fail 0 [10,3,1,1,2,2,7,5]` (2 success - 2 fails = 0)
or
`Critical fail -2 [1,3,1,1,2,2,7,5]` (1 success - 3 fails = -2)

In this example, the critical fail happens when there are more fails than success.

  
  
In the next example, the critical fail happens when there was no success and a least one fail.

> 8d10;$1c[>6];$1c1;$2-$3;$4i:[=0]{"Fail $4 [%2]"}{$4i:[>0]{"$2 Success[%2]"}{$2i:[=0]{"Critical Fail $4 [%2]"}{"Fail $4 [%2]"}}}


### Group

> 5d10g10 

Roll 5 dice and then try to group them to make group of 10 [7th sea system].

### Comment (\#)

> 2D6 # Sword attack 

Display "Sword attack" and the result of the two dice.
DiceParser ignore everything after the \#. The whole part is treated as one comment.
So DiceParser can answer question:

> 1L[yes,no] # Am I evil ?

```
Am I evil ?
yes
```

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

> (2+2)^2
Result: 16 

> 1d10^2



## Validator

There are three kind of Validator:

* Scalar
* Range
* Boolean expression

Any operator which requires validator (such as `a,r,e,c`) can use those three kind.

### Scalar 

The scalar value sets the validator on eguality between the dice value and the validator

> 4d10e10

This command means : roll 4 dice and they explode on 10.

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

## Select value from List

The L operator (meaning list) can offer you the opportunity to pick up value from list.

> 1L[sword,bow,knife,gun,shotgun]

With comment

> 1L[yes,no] # Am I evil ?

```
Am I evil ?
yes
```

## Examples

> 3D100

Roll 3 dice with 100 faces

> 10D10e[=10]s

Roll 10 dice with 10 faces, 10 explodes, and sort the result.

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


# Best Practices

As DiceParser provides more and more features, you may find several ways to do the same thing. We want here to explain the difference between those several approaches. Then you will be able to use the right one.


## Roll several kind of dice and sum them

```Bad```
> 2d8;2d10m

```Good```
> 2d8+2d10

The merge operator is useful when you want to use dice operator on all rolled dice.
