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
