#include <iostream>
#include <fstream>
#include <cmath>
#include "SimpleGraph.h"
#include <chrono>
#include <thread>
#include <string>

using namespace std;
const double kPi = 3.14159265358979323;
const double kRepel= 0.001;
const double kAttract= 0.001;


void Welcome();
void openFile (ifstream &file);
vector<Edge> getEdgesFromFile(ifstream &file);
int getNumberNodes(ifstream &file);
vector<Node> getNodesInitPosition(int numNodes);
double calcForceRepel(Node &n0, Node &n1);
double calcForceAttract(Node &n0, Node &n1);
double calcTheta(Node &n0, Node &n1);
void getUserTime(double &maxTime);

// Main method
int main() {
        Welcome();
        cout<<"bey!"<<endl;
        return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
        cout<< "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout << "Welcome to CS106L GraphViz!" << endl;
        cout << "This program uses a force-directed graph layout algorithm" << endl;
        cout << "to render sleek, snazzy pictures of various graphs." << endl;
        cout<< "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

        cout << endl;
        // 1- First we open the file
        ifstream file;
        openFile(file);

        //2- Second we ask the user for maximum time (seconds)
        double maxTime;
        getUserTime(maxTime);
        //3- we process
        cout<<getNumberNodes(file)<<endl;
        vector<Edge> edges;
        edges= getEdgesFromFile(file);

        vector<Node> nodes;
        nodes= getNodesInitPosition(getNumberNodes(file));

        SimpleGraph simpleInitGraph;
        simpleInitGraph.nodes=nodes;
        simpleInitGraph.edges=edges;

        InitGraphVisualizer(simpleInitGraph);
        DrawGraph(simpleInitGraph);


        vector<Node> nodesCopy=nodes;
        int iter=0;
        int maxIter= 30000;
        while(iter<maxTime) {

                for(size_t i=0; i< nodes.size(); i++) {
                        Node n0 =nodes[i];

                        for(size_t j=0; j < nodes.size() && j!=i; j++) {
                                //if(n0 != n1) {
                                Node n1 =nodes[j];

                                double forceRepel = calcForceRepel(n0,n1);
                                double theta = calcTheta(n0,n1);
                                nodesCopy[i].x -= forceRepel*cos(theta);
                                nodesCopy[i].y -= forceRepel*sin(theta);
                                nodesCopy[j].x += forceRepel*cos(theta);
                                nodesCopy[j].y += forceRepel*sin(theta);

                                //  }
                        }
                        for (Edge edge: edges) {
                                if(edge.start==i) {
                                        Node n2 =nodes[edge.end];
                                        double forceAttract = calcForceAttract(n0,n2);
                                        double theta = calcTheta(n0,n2);
                                        nodesCopy[i].x += forceAttract*cos(theta);
                                        nodesCopy[i].y += forceAttract*sin(theta);
                                        nodesCopy[edge.end].x -= forceAttract*cos(theta);
                                        nodesCopy[edge.end].y -= forceAttract*sin(theta);

                                }

                        }



                }
                iter+=1;
                nodes=nodesCopy;
                if(iter%100==0) {
                        simpleInitGraph.nodes=nodes;
                        simpleInitGraph.edges=edges;

                        InitGraphVisualizer(simpleInitGraph);
                        DrawGraph(simpleInitGraph);


                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }

        }

        // simpleInitGraph.nodes=nodes;
        // simpleInitGraph.edges=edges;

        // DrawGraph(simpleInitGraph);
        // InitGraphVisualizer(simpleInitGraph);

        //  std::this_thread::sleep_for(std::chrono::milliseconds(5000));


        file.close();
        return;

}


void openFile (ifstream &file){
        while(true) {
                cout<<"Please enter the source file : ";
                string fileName;
                std::getline(cin,fileName);

                file.open(fileName);
                if(file.is_open()) {
                        return;
                }
                cout<<"Please enter a valid file name"<<endl;
        }
}

void getUserTime(double &maxTime){

        while(true) {
repeat:
                string maxTimeStr;
                cout<<"Please enter the maximum time for simulation (seconds): ";
                std::getline(cin, maxTimeStr);
                try{
                        maxTime= std::stod(maxTimeStr);
                }catch(...) {
                        goto repeat;
                }
                if(maxTime<=0) {
                        cout<< "Please enter a positive value"<<endl;
                }
                else{
                        return;
                }
        }

}
vector<Edge> getEdgesFromFile(ifstream &file){
        int _;
        _=  getNumberNodes(file);
        Edge edge;
        size_t start, end;
        vector<Edge> edges;
        while(!file.eof()) {
                file>>start>>ws;
                file>>end>>ws;
                edge.start= start;
                edge.end = end;
                edges.push_back(edge);
        }
        return edges;
}
int getNumberNodes(ifstream &file){
        file.clear(); // in the case we reach a flag (end of the file for example)
        file.seekg(0, ios::beg);
        int numNode;
        file>>numNode;
        return numNode;
}

vector<Node> getNodesInitPosition(int numNodes){
        Node node;
        vector<Node> nodes;
        for(int i =0; i<numNodes; i++) {
                node.x= cos(2*kPi*i/numNodes);
                node.y= sin(2*kPi*i/numNodes);
                nodes.push_back(node);
        }
        return nodes;
}

double calcForceRepel(Node &n0, Node &n1){
        return kRepel / sqrt(pow(n1.y - n0.y,2) + pow(n1.x - n0.x, 2));
}

double calcForceAttract(Node &n0, Node &n1){
        return kAttract *(pow(n1.y - n0.y,2) + pow(n1.x - n0.x, 2));

}

double calcTheta(Node &n0, Node &n1){
        return atan2((n1.y - n0.y),(n1.x - n0.x));
}

