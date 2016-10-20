Every python codes need to be in the same directory.
These python codes are written in Python 3.4
The version of Numpy is 1.11.2
The version of Matplotlib is 1.3.1

1) EM-GMM
USAGE: python emgmm.py <input file> <output file> <# of clusters> <display on(1)/off(0)>
 - input file: the name of the input file name
 - output file: the name of the output file name
 - # of clusters: the number of clusters to be seperated
 - display on(1)/off(0): If 1 is assigned to this place, this program may show you the result in the image. ("matplotlib" is used)

Example:
 - python emgmm.py mouse.txt result.txt 3 1
 - python emgmm.py mouse.txt result.txt 2 0

Output file:
 - Each line is the number of cluster assigned to the input in the same line of the input file

2) K-Means
USAGE: python kmeans.py <input file> <output file> <# of clusters> <display on(1)/off(0)>
 - input file: the name of the input file name
 - output file: the name of the output file name
 - # of clusters: the number of clusters to be seperated
 - display on(1)/off(0): If 1 is assigned to this place, this program may show you the result in the image. ("matplotlib" is used)

Example:
 - python kmeans.py mouse.txt result.txt 3 1
 - python kmeans.py mouse.txt result.txt 2 0

Output file:
 - Each line is the number of cluster assigned to the input in the same line of the input file

