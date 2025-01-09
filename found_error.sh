1) minishell$ echo "Dilara"    && echo "Cagaray          Dilara"

Real shell behavior --->
Dilara
Cagaray          Dilara

My shell behavior --->   LOOKS CORRECT
AST_AND [depth=0]
  CMD: echo "Dilara" [depth=1]
  CMD: echo "Cagaray          Dilara" [depth=1]


2) minishell$ echo "Dilara

Real shell behavior ---> quote>
My shell behavior ---> Syntax error: Unclosed quote '"' at position 12... Parsing failed


3) minishell % cat <<EOF && echo done (test) || ls -l

Real shell behavior ---> cmdand cmdor heredoc>
My shell behavior ---> Parsing failed.

3) minishell$ ls -l && echo "dilara
My shell behavior ---> Parsing failed. Syntax error: Unclosed quote
Real shell behavior ---> cmdand dquote>
