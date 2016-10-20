This version of Python is 2.7.6

To execute main.py, there needs two additional python code in the same directory.
They are
  cpt.py
  etc.py

There are two modes to execute.

Usage 1: python main.py
  This will display the following three queries.
  P(X4=1)
  P(X4=1|X3=1)
  P(X3=1|X1=1,X2=0)

Usage 2: python main.py <query variable> <evidence 1> <evidence 2> ...
  By this you can test any query. The followings are the example.
  P(X3=1|X1=0,X2=1): python main.py X3 X1=0 X2=1
  P(X4=1): python main.py X4
