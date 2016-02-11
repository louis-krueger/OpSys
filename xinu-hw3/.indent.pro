/* indent profile following the XINU Coding Style Guidelines */

/* indentation */
--use-tabs
--indent-level 16             /* set large indent level... */
--tab-size 16                 /* because tab size will convert it to tabs... */
--continue-at-parentheses     /* .. and code will space in to parentheses. */
--preprocessor-indentation 16  


/* blank lines */
--blank-lines-after-declarations
--blank-lines-after-procedures
--swallow-optional-blank-lines

/* braces */
--braces-after-if-line
--brace-indent0

/* procedures */
--dont-break-procedure-type

/* spaces */
--space-after-if
--space-after-while
--space-after-for
--no-space-after-function-call-names
--no-space-after-parentheses
--no-space-after-casts
--space-special-semicolon

/* line length */
--line-length          75
--comment-line-length  75

/* line breaking */
--honour-newlines
--break-before-boolean-operator
--dont-cuddle-else
--cuddle-do-while
