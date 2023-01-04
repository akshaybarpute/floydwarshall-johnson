#include<iostream>
#include <stdlib.h>     /* srand, rand */
#include<climits>
#include<vector>
#include <fstream>
#include<string>
#include <algorithm>
#include<cstring>
#include <dirent.h>
#include <regex>
#include<chrono>

#include "graphutil.h"
using namespace std;

int GraphUtil::minDist(int dist[], bool sptSet[], int V){
    // Initialize min value for dikstras
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void GraphUtil::dijkstra(vector<vector<int>> graph, vector<vector<int>> *mdGraph, int src){

    int vertexSize=graph.size();
    int dist[vertexSize]; //output array
    bool sptSet[vertexSize];

    for(int i=0;i<vertexSize;i++){
        dist[i]=INT_MAX;
        sptSet[i]=false;
    }

    // distance of source vertex from itself will be 0
    dist[src]=0;

    for(int i=0;i<vertexSize-1;i++){
        // getting minimum wt. vertices from list
        int u=minDist(dist,sptSet,vertexSize);
        sptSet[u]=true;

        for(int v=0;v<vertexSize;v++){

            if (!sptSet[v] && mdGraph->at(u).at(v) && dist[u] != INT_MAX
                && dist[u] + mdGraph->at(u).at(v) < dist[v]){
                dist[v] = dist[u] + mdGraph->at(u).at(v);
                }

            }
    }

    for(int i=0;i<vertexSize;i++){
        mdGraph->at(src).at(i)=dist[i]; //modifying mdGraph vector. it is passed as reference
    }
}

vector<int> GraphUtil::bellmanFord(vector<tuple<int,int,int>> edges, vector<vector<int>>graph){
    vector<int> dist;
    auto vertexSize=graph.size();

    for(int i=0;i<vertexSize+1;i++){
        dist.push_back(INT_MAX);
    }

    dist.at(vertexSize)=0;

    for(int i=0;i<vertexSize;i++){
        edges.push_back(make_tuple(vertexSize,i,0));
    }

    for(int i=0;i<vertexSize;i++){
        for(auto e: edges){
            if(dist.at(get<0>(e))!=INT_MAX && dist.at(get<0>(e))+ get<2>(e) < dist.at(get<1>(e))){
                dist.at(get<1>(e)) = dist.at(get<0>(e)) +get<2>(e);
            }
        }
    }

    vector<int> vec(dist.cbegin(), dist.cbegin()+vertexSize+1);
    return vec;
}

GraphUtil::GraphUtil(){
    srand(time(0));
    this->size=rand()%10+2;
    // this->size=500; // hardcoding sometime helps to get graph of strictly desired vertex size
    this->max=100;
    this->min=1;
}

GraphUtil::GraphUtil(int maxSize, int maxWeight, int minWeight){
    srand(time(0));
    this->size=rand()%maxSize+1;
    this->max=maxWeight;
    this->min=minWeight;
}

GraphUtil::GraphUtil(int maxSize, int maxWeight){
    srand(time(0));
    this->size=rand()%maxSize+1;
    this->max=maxWeight;
    this->min=maxWeight/2;
}

void GraphUtil::setSparse(bool val){ // sparse value==true, the sparse graph will be generated
    this->sparse=val;
}

vector<vector<int>> GraphUtil::generateRandom(){
    vector<vector<int>> res;

    for(int i=0;i<this->size;i++){
        vector<int> sub;
        for(int j=0;j<this->size;j++){
            if(i==j){
                sub.push_back(0); // distance from vertex from itself is always 0
            }
            else{
                int newRange= this->max - this->min;
                auto ele= rand()%newRange+1;
                ele=ele+this->min;
                sub.push_back(ele);
                }
        }
        res.push_back(sub);
    }

    int ran=0;

    if(this->sparse){
        ran=rand()%this->size+this->size*2; //adding more INT_MAX values .i.e removing edges for sparse graphs
    }else{
        ran=rand()%this->size;
    }

    for(int i=0;i<ran;i++){
        int p1= rand()%this->size;
        int p2=rand()%this->size;

        if(p1!=p2){
            res.at(p1).at(p2)=INT_MAX; // making sure that the diagonal values are still 0
        }
    }

    return res;
}

void GraphUtil::print(vector<vector<int>> res){
    int i=0,len=res.size();
    cout<<"INFINITY="<<this->max<<endl;
    cout<<"["<<endl;;
    for(auto e1: res){
        i=0;
        cout<<"[";
        for(auto e2: e1){
            if(i==len-1){
                cout<<e2;
            }
            else{
                cout<<e2<<",";
            }
            i++;
        }
        cout<<"],"<<endl; // if "]," replaced with "]" & made some changes in buildGraphFromText function major bug can be eliminated. But, out of time now
    }
    cout<<"]"<<endl;
}

void GraphUtil::print(vector<vector<int>> res, string filePath){
    cout<<"printing"<<endl;
    ofstream file;
    int i=0,len=res.size();
    // cout<<"INFINITY="<<this->max<<endl;
    file.open(filePath);
    file<<"["<<endl;;
    for(auto e1: res){
        i=0;
        file<<"[";
        for(auto e2: e1){
            if(i==len-1){
                file<<e2;
            }
            else{
                file<<e2<<",";
            }
            i++;
        }
        file<<"],"<<endl;
    }
    file<<"]"<<endl;
    file.close();
}

vector<vector<int>> GraphUtil::buildGraphFromText(string filePath){ // will generate vector<vector<int>> from input graph.txt files

    vector<vector<int>> output;
    string line;
    ifstream myfile (filePath);
    if (myfile.is_open()){
        int i=0;
        while (getline(myfile,line)){
            // cout<<++i<<" ## "<<line<<endl;
            string delimiter=",";
            if(i>1){
                vector<int> res;

                if(line.find('[') != std::string::npos)
                    line.erase(remove(line.begin(), line.end(), '['));
                        if(line.find(']') != std::string::npos)
                            line.erase(remove(line.begin(), line.end(), ']'));

                            size_t pos = 0;
                            string token;
                            while ((pos = line.find(delimiter)) != std::string::npos) {
                                token = line.substr(0, pos);
                                // std::cout << token << std::endl;
                                res.push_back(stoi(token));
                                line.erase(0, pos + delimiter.length());
                            }
                            if(res.size()>1){
                                output.push_back(res);
                            }
            }
            i++;
        }
        myfile.close();
    }
    return output;
}


pair<vector<vector<int>>,bool> GraphUtil::floydWarshall(vector<vector<int>> input){
    vector<vector<int>> output;

    //step1: copy input matrix into output matrix
    for(auto e1:input){
        vector<int> res;
        for(auto e2:e1){
            res.push_back(e2);
        }
        output.push_back(res);
    }


        //step2: main algorithm
    int v=input.size();

    for(int k=0;k<v;k++){
        for(int i=0;i<v;i++){
            for(int j=0;j<v;j++){
                if(output.at(i).at(j)>output.at(i).at(k)+output.at(k).at(j) && output.at(i).at(k)!=INT_MAX && output.at(k).at(j)!=INT_MAX){
                    output.at(i).at(j)=output.at(i).at(k)+output.at(k).at(j);
                }
            }
        }
    }


        //step3: min wt. cycle detection

    for(int i=0;i<v;i++){
        if(output.at(i).at(i)<0){
            // cout<<"-ve wt cycle present"<<endl;
            return make_pair(output,true);
        }
    }
    return make_pair(output,false);
}

vector<vector<int>> GraphUtil::johnsons(vector<vector<int>> graph){
    vector<tuple<int,int,int>> edges;
    int vertexSize=graph.size();

    //filling edges vector
    for(int i=0;i<vertexSize;i++){
        for(int j=0;j<vertexSize;j++){
            if(graph[i][j]!=0 && graph[i][j]!=INT_MAX){
                edges.push_back(make_tuple(i,j,graph[i][j]));
            }
        }
    }

    auto modifiedWts=bellmanFord(edges,graph);

    //copying current graph into new variable
    vector<vector<int>> modifiedGraph;

    for(auto e:graph){
        vector<int> ele(e);
        modifiedGraph.push_back(ele);
    }


    //changing the weights inside modified graph-- may need to change

    for(int i=0;i<vertexSize;i++){
        for(int j=0;j<vertexSize;j++){
            if(graph.at(i).at(j)!=0 && graph.at(i).at(j)!=0){
                modifiedGraph.at(i).at(j)=graph.at(i).at(j) + modifiedWts.at(i) - modifiedWts.at(j);
            }
        }
    }


    //calling Dijkstras here

    for(int i=0;i<vertexSize;i++){

        dijkstra(graph, &modifiedGraph, i);
    }

    return modifiedGraph;
}


void GraphUtil::runTests(string algo){
    string inputDir="";
    string outputDir="";
    if(algo=="floyd"){
        inputDir="./testing/floyd-warshall/inputs/";
        outputDir="./testing/floyd-warshall/outputs/";
    }
    else if(algo=="johnson"){
        inputDir="./testing/johnson/inputs/";
        outputDir="./testing/johnson/outputs/";
    }

    DIR *dr;
    struct dirent *en;
    dr = opendir(inputDir.c_str()); //open all or present directory
    time_t start,finish;

    if(algo=="floyd"){
        if (dr) {
            while ((en = readdir(dr)) != NULL) {
                // printf("%s\n", en->d_name); //print all directory name
                string filename=en->d_name; // returns filenames inside a directory
                    if(filename.find(".txt") != std::string::npos){
                        cout<<"TestCase: "<<filename<<endl;
                        string filePath=inputDir+filename;
                        ifstream myfile(filePath);
                        if(myfile.is_open()){
                            auto input=this->buildGraphFromText(filePath);
                            // timer here starts
                            auto begin = chrono::high_resolution_clock::now();    
                            auto res= this->floydWarshall(input);
                            myfile.close();
                            string newFileName = std::regex_replace(filename, std::regex("input"), "output");
                            string outputFilePath=outputDir+newFileName;

                            ifstream myfile2(outputFilePath);

                            if(myfile2.is_open()){
                                auto output=this->buildGraphFromText(outputFilePath);
                                if(output.size()==0&&res.second==true){
                                    cout<<"success"<<endl;
                                    auto end = chrono::high_resolution_clock::now();
                                    auto dur = end - begin;
                                    auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
                                    cout << "Time required = " << ms << " nano-seconds"<<endl;
                                }
                                else{

                                    for(int i=0;i<output.size();i++){
                                        for(int j=0;j<output.size();j++){
                                            if(output.at(i).at(j)!=res.first.at(i).at(j)){
                                                cout<<"failed"<<endl;
                                                return;
                                                break;
                                            }
                                        }
                                    }
                                    cout<<"success"<<endl;
                                    //timer here closes
                                    auto end = chrono::high_resolution_clock::now();
                                    auto dur = end - begin;
                                    auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
                                    cout <<"##"<< ms << endl;

                                    //print time
                                    cout << "Time required = " << ms << " nano-seconds"<<endl;
                                    myfile2.close();
                                }
                            }
                            else{
                                cout<<"error while opening output test file"<<endl;
                            }
                        }
                        else{
                            cout<<"Error while opening input test file"<<endl;
                        }
                    }
                }
                closedir(dr); //close all directory
            }

        }
        else{

            if (dr) {
                while ((en = readdir(dr)) != NULL) {
                    // printf("%s\n", en->d_name); //print all directory name
                    string filename=en->d_name;
                    if(filename.find(".txt") != std::string::npos){
                        cout<<"TestCase: "<<filename<<endl;
                        string filePath=inputDir+filename;
                        ifstream myfile(filePath);
                        if(myfile.is_open()){
                            auto input=this->buildGraphFromText(filePath);
                            // timer here starts
                            auto begin = chrono::high_resolution_clock::now();    
                            auto res= this->johnsons(input);
                            myfile.close();
                            string newFileName = std::regex_replace(filename, std::regex("input"), "output");
                            string outputFilePath=outputDir+newFileName;

                            ifstream myfile2(outputFilePath);

                            if(myfile2.is_open()){
                                auto output=this->buildGraphFromText(outputFilePath);

                                    for(int i=0;i<output.size();i++){
                                        for(int j=0;j<output.size();j++){
                                            if(output.at(i).at(j)!=res.at(i).at(j)){
                                                cout<<"failed"<<endl;
                                                return;
                                                break;
                                            }
                                        }
                                    }
                                    cout<<"success"<<endl;
                                    //timer here closes
                                    auto end = chrono::high_resolution_clock::now();
                                    auto dur = end - begin;
                                    auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
                                    cout <<"##"<< ms << endl;

                                    //print time
                                    cout << "Time required = " << ms << " nano-seconds"<<endl;
                                    myfile2.close();
                            }
                            else{
                                cout<<"error while opening output test file"<<endl;
                            }
                        }
                        else{
                            cout<<"Error while opening input test file"<<endl;
                        }
                    }
                }
                closedir(dr); //close all directory
            }
    }
}

int main(int argc, char *argv[]){

    if(argc<2){
        cout<<"incorrect no. of args. required 2"<<endl;
        return 0;
    }

    string input=argv[1];
        if(input.compare("--all-floyd")==0){
            GraphUtil g;
            g.runTests("floyd");
        }
        else if(input.compare("--all-johnson")==0){
            GraphUtil g;
            g.runTests("johnson");
        }
        else if(argc==3){

            /*
            ##### important ######
            floyd warshall treats INT_max as no edge
            But, Johnsons treat 0 as no edge in this implementation
            */
            string filePath=argv[1];
            string algo=argv[2];
            GraphUtil g;
            auto graph=g.buildGraphFromText(filePath);

            if(algo=="floyd"){
                cout<<"algorithm: floyd"<<endl;
                auto res= g.floydWarshall(graph);
                if(res.second==true){
                    cout<<"-ve wt. cycle detected"<<endl;
                }
                else{
                    g.print(res.first);
                }
            }
            else{
                cout<<"algorithm: johnsons"<<endl;
                auto res=g.johnsons(graph);
                g.print(res);
            }
            // g.print(graph);
        }
        else{
            cout<<"Invalid input"<<endl;
        }
  
    return 0;
}

