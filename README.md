
You can compile this code by running "make" in linux or unix terminal
It will generate executable with name algos

How to run?

You can run all the test cases related to floyd-warshall by running command

./algos --all-floyd


You can run all the test cases related to Johnson's algorithm by running command

./algos --all-johnson


You can create your own test cases.
You have to strictly use below format for that. It is important that you use strictly below format.
Floys warshall treats INT_MAX as no edge & Johnson treats 0 as no edge.

meta:{vertex=4,sparse=true},
[
[0,-5,2,3], 
[0,0,4,0], 
[0,0,0,1], 
[0,0,0,0],
] 

GraphUtil class has a function generateRandom to generate random big graphs.
Before generating graph you can set setSparse param either true or false.
It is used for gerating dense or sparse graphs.


To run your custom case fille as input.

Create a graph in a file as shown above.
Then, run following command

./algos path algo

For Ex. ./algos ./testing/floyd-warshall/inputs/input1.txt floyd

path=path of input file
& algo= algorithms. valid values are floyd & johnson.

Some test cases will take 2-3 minutes to complete.

You can generate very large random graphs by using generateRandom function provided by GraphUtil.
But, floyd-warshall treats INT_MAX as no edge & graph generation utility is made primary to test floyd-warshall algorithm.

Johnson's algorithm is added later. It considers 0 as no edge instead of INT_MAX.
So, you will need to replace INT_max values with 0 manually while generating testcases for Johnson's.

Maintainers:
Akshay Barpute
