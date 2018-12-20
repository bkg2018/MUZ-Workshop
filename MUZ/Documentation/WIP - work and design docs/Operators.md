#  Operators

The table lists all possible operator combinations with arguments number and type.

When an operator can receive 1 or 2 operators, the 2 operators form is checked first.

When an operator can work on number or strings, numbers are tried first.

Unless notified, the result type is the same as the arguments type.


|Operator|Priority|nb arguments|Arg type|result type|Remark or example
|---|---|---|---|---|---|
|  !  |  0|1|number| | `!0xFFFF`
|  !  |  0|1|boolean||useable on expressions returning a boolean `!(a && b)`
|  *  |  1 | 2 | number	 | | `2 * 3` 
/ | 1 | 2 | number | | integer division only `48 / 3`
% | 1 | 2 | number | | module `255 % 8`
+ | 2 | 2 | number | | unary form not handled `2 + 5`
+ | 2 | 2 | string | | string concatenation `"R" + "0"`
- | 2 | 1 | number	 | | unary form `-2`
- | 2 | 2 | number	 | | `256 - 0x80`
<< | 3 | 2 | number	 | | left shift `1 << 4`
>> | 3 | 2 | number | | right shift `256 >> 3`
< | 4 | 2 | number | boolean | `VERSION < 2`
< | 4 | 2 | string | boolean | `"PLANE" < "QUESTION"`
> | 4 | 2 | number | boolean | `256 > 128`
> | 4 | 2 | string | boolean | `"v" > "V"`
<= | 4 | 2 | number | boolean | `127 <= 0x7F`
<= | 4 | 2 | string | boolean | `"" <= ""`
>= | 4 | 2 | number | boolean | ` >= `
>= | 4 | 2 | string | boolean | ` "" >= ""`
!=  <> | 5 | 2 | number | boolean | ` != `
!=  <> | 5 | 2 | string | boolean | ` "" != ""`
= == | 5 | 2 | number | boolean | ` = `
= == | 5 | 2 | string | boolean | `"" = ""`
& | 6 | 2 | number |  | ` & `
^ | 7 | 2 | number |  | ` ^ `
\| | 8 | 2 | number |  | ` | `
&& | 9 | 2 | boolean |  | ` && `
\|\| | 10 | 2 | boolean |  | ` || `


