Here you may find macro made by the community:


# Legend of the 5 rings, 4th edition

* Nine-again and reroll 1: `!macro add KNS d10r1e[>8]k False`
* Nine-again: `!macro add KN d10e[>8]k False`
* Reroll 1: `!macro add KS d10r1e10k False`
* Skilled roll: `!macro add K d10e10k False`
* unskilled roll: `!macro add k d10k False`

Author: Obi

# Legend of the 5 rings, 4th edition [For French-speaking Users]

* Nine-again and reroll 1: `!macro add GNS d10r1e[>8]k False`
* Nine-again: `!macro add GN d10e[>8]k False`
* Reroll 1: `!macro add GS d10r1e10k False`
* Skilled roll: `!macro add G d10e10k False`
* unskilled roll: `!macro add G d10k False`

Author: Obi


# Spire/Heart by Rowan, Rook and Deckard

```
!macro add s([1-5])d([0-2]) "\1-\2;$1i:[<=0]{1}{$1};$1i:[<=0]{\2+1-\1}{0};$2d10s;$4k1;$4c10;$5i:[=10]{5-$3}{$5i:[>=8]{4-$3}{$5i:[>=6]{3-$3}{$5i:[>=2]{2-$3}{$5i:[=1]{1}}}}};$7i:[=5]{\"\n# Critical Success ($6 extra stress)\n[%2]\"}{$7i:[=4]{\"\n# Success\n[%2]\"}{$7i:[=3]{\"\n# Success at a cost\n[%2]\"}{$7i:[=2]{\"\n# Failure\n[%2]\"}{$7i:[<=1]{\"\n# Critical Failure\n[%2]\"}}}}}" True
```

Author: GreaterGerardon


# Earthdawn, 3th and 4th editions

* Niveau 1: `!macro add n01 ((d4e4)-2) False`
* Niveau 2: `!macro add n02 ((d4e4)-1) False`
* Niveau 3: `!macro add n03 (d4e4) False`
* Niveau 4: `!macro add n04 (d6e6) False`
* Niveau 5: `!macro add n05 (d8e8) False`
* Niveau 6: `!macro add n06 (d10e10) False`
* Niveau 7: `!macro add n07 (d12e12) False`
* Niveau 8: `!macro add n08 (2d6e6) False`
* Niveau 9: `!macro add n09 (d8e8)+(d6e6) False`
* Niveau 10: `!macro add n10 (d10e10)+(d6e6) False`
* Niveau 11: `!macro add n11 (d10e10)+(d8e8) False`
* Niveau 12: `!macro add n12 (2d10e10) False`
* Niveau 13: `!macro add n13 (d12e12)+(d10e10) False`
* Niveau 14: `!macro add n14 (d20e20)+(d4e4) False`
* Niveau 15: `!macro add n15 (d20e20)+(d6e6) False`
* Niveau 16: `!macro add n16 (d20e20)+(d8e8) False`
* Niveau 17: `!macro add n17 (d20e20)+(d10e10) False`
* Niveau 18: `!macro add n18 (d20e20)+(d12e12) False`
* Niveau 19: `!macro add n19 (d20e20)+(2d6e6) False`
* Niveau 20: `!macro add n20 (d20e20)+(d8e8)+(d6e6) False`
* Niveau 21: `!macro add n21 (d20e20)+(d10e10)+(d6e6) False`
* Niveau 22: `!macro add n22 (d20e20)+(d10e10)+(d8e8) False`
* Niveau 23: `!macro add n23 (d20e20)+(2d10e10) False`
* Niveau 24: `!macro add n24 (d20e20)+(d12e12)+(d10e10) False`
* Niveau 25: `!macro add n25 (d20e20)+(d10e10)+(d8e8)+(d4e4) False`
* Niveau 26: `!macro add n26 (d20e20)+(d10e10)+(d8e8)+(d6e6) False`
* Niveau 27: `!macro add n27 (d20e20)+(d10e10)+(2d8e8) False`
* Niveau 28: `!macro add n28 (d20e20)+(2d10e10)+(d8e8) False`
* Niveau 29: `!macro add n29 (d20e20)+(d12e12)+(d10e10)+(d8e8) False`
* Niveau 30: `!macro add n30 (d20e20)+(d10e10)+(d8e8)+(2d6e6) False`
* Niveau 31: `!macro add n31 (d20e20)+(d10e10)+(2d8e8)+(d6e6) False`
* Niveau 32: `!macro add n32 (d20e20)+(2d10e10)+(d8e8)+(d6e6) False`
* Niveau 33: `!macro add n33 (d20e20)+(2d10e10)+(2d8e8) False`
* Niveau 34: `!macro add n34 (d20e20)+(3d10e10)+(d8e8) False`
* Niveau 35: `!macro add n35 (d20e20)+(d12e12)+(2d10e10)+(d8e8) False`
* Niveau 36: `!macro add n36 (2d20e20)+(d10e10)+(d8e8)+(d4e4) False`
* Niveau 37: `!macro add n37 (2d20e20)+(d10e10)+(d8e8)+(d6e6) False`
* Niveau 38: `!macro add n38 (2d20e20)+(d10e10)+(2d8e8) False`
* Niveau 39: `!macro add n39 (2d20e20)+(2d10e10)+(d8e8) False`
* Niveau 40: `!macro add n40 (2d20e20)+(d12e12)+(d10e10)+(d8e8) False`

Author: DorDeDor

# Pathfinder 1e / D&D3.5e
```
!macro add ([0-9]+)th([0-9]+)d([0-9]+)\+([0-9])crit([0-9]+) d20;$1+\1;$1i:[>=\5]{+1d20-$1};$3+\1;\2d\3+\4;\2d\3+\4;$1i:[>=\5]{+$6+$5-$1};$1i:[>=\5]{"Critical Hit: $1+\1=$2\nConfirm Hit: $3+\1=$4 \nIf confirms Damage: [@5]+\4+[@6]+\4=$7 \nIf doesn't confirm Damage: [@5]+\4=$5"}{"To Hit: [$1]+\1=$2 \nDamage: [@5]+\4=$5"} True
```

Usage: [To hit modifier]th[# of damage dice]d[size of damage dice]+[size of static modifier]crit[lowest value for weapon's crit range]

Author:tyjo99

# Prowlers and Paragons
Once you set up this macro just input !Xpp where X is the number of dice you are rolling. These will count one success on a 2 and 4, and count two successes on a 6. If roll in just one die it will only count a success on a roll of a six. 

```
!macro add ([0-9]+)pp \1D6;$1c[=2|=4];$1c6;\1;$4i:[>1]{$2+$3*2}{$3*2};"$5 successes [@1]" True
```


Author:darthgaul
