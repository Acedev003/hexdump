####Lessons Learnt

1. fget() did not parse file completely and stops prematurely with a return type of EOF.
    Cause of error exactly described in the first link below
    - http://c-faq.com/stdio/getcharc.html
    - https://stackoverflow.com/questions/13830338/377-character-in-c#13830379 

2. This error was that of distorted hexadecimal outputs in random bytes
    Cause and solution explained here:
     https://stackoverflow.com/a/15823764/14036609