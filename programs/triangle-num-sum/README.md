# Sum of triangle numbers up to a given number

Note: I use r01 as my first register because I forgot that r00 existed

```
/*
r01: Current triangle number index.
r02: Current triangle number
r03: Total

r08: The number 1
r09: Max triangle number
*/

// First import number that the triangle numbers to be added can't exceed
imp 100             .8064
mov res r09         .0F89
                    .
// init             .
imp 1               .8001
mov res r08         .0F88
mov res r01         .0F81
mov res r02         .0F82
mov res r03         .0F83
                    .
// Main loop ()        .
add r08 r01         .4101
mov res r01         .0F81
                    .
add r01 r02         .4022
mov res r02         .0F82
                    .
add r02 r03         .4043
mov res r03         .0F83
                    .
imp 7               .8007
mov res adr         .0F9E
cmp r02 r09         .2049
jlt                 .3000
                    .
// END              .
imp result memory   .8030
mov res adr         .0F9E
mov r03 res         .0C7C
sav                 .0800
hlt                 .0000
```