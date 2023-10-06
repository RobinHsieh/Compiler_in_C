# Compiler final project
### .bash_aliases
```bash
alias f1='bison -d -v -o mini_LISP.tab.c mini_LISP.y'
alias f2='gcc -c -g -I.. mini_LISP.tab.c'
alias f3='flex -o mini_LISP.yy.c mini_LISP.l'
alias f4='gcc -c -g -I.. mini_LISP.yy.c'
alias f5='gcc -c constructAST.c'
alias f6='gcc -o mini_LISP mini_LISP.tab.o mini_LISP.yy.o constructAST.o -ll'
alias f7='./mini_LISP < input.lsp'
```
### linux terminal command line
```commandline
source .bash_aliases
f1
f2
f3
f4
f5
f6
f7
```