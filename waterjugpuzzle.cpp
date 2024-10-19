/*******************************************************************************
 * Name        : waterjug.cpp
 * Author      : Ethan Shanahan and Kieran Corson
 * Date        : 10/11/2023
 * Description : Finds min number of fills to reach goal fill on three jugs using BFS
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

struct State {
    int a, b, c;
    string directions;
    State *parent;
    int amount;
    
    State(int _a, int _b, int _c, string _directions, int _amount) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr}, amount{_amount} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

bool stateExists(int curr1, int curr2, int jug2){
    //checks is state where curr1 pours into curr2 is possible
    if(curr1 <= 0){ return false; }
    if(curr2 >= jug2){ return false; }
    return true;
}

void pour(int &j1, int &j2, int lim2){
    //recursively pours j1 into j2 until a limit is reached
    if(j1 == 0){ return; }
    if(j2 == lim2){ return; }
    j1--;
    j2++;
    pour(j1, j2, lim2);
}

void print(State goal){
    //prints the proper format for a single line of the result
    if(goal.amount == 1){
            cout << "Pour 1 gallon from " << goal.directions << ". " << goal.to_string() << endl;
        }else{
            cout << "Pour " << goal.amount << " gallons from " << goal.directions << ". " << goal.to_string() << endl;
        }    
}

void printState(State goal){
    //checks if the state is the initial state, and then prints otherwise
    if(goal.directions.compare("Initial state") == 0){
        cout << goal.directions << ". " << goal.to_string() << endl;
    }else{
        print(goal);
    }
}

void backtrack(State goal){
    //recursively prints goal state and all of its parents
    if(goal.parent != nullptr){
        backtrack(*goal.parent);
    }
    printState(goal);
}

void bfs(int lim1, int lim2, int lim3, int gol1, int gol2, int gol3){

    //init the solvable state to false until proven true
    bool solvable = false;

    //init triple star graph (2d array of pointers)
    State*** graph = new State**[lim1+1];
    for(int i = 0; i < lim1+1; i++){
        graph[i] = new State*[lim2+1];
        for(int j = 0; j < lim2+1; j++){
            graph[i][j] = nullptr;
        }
    }

    //init queue
    queue<State*> q;

    //setup initial state of jugs and push into q
    State* initState = new State(0, 0, lim3, "Initial state", 0);
    q.push(initState);

    //begin bfs
    while(!(q.empty())){

        //set current to the front of the queue
        State current = *(q.front());

        //if the graph at this location alrdy exists, it must have been visited
        if(graph[current.a][current.b] != nullptr){
            //delete the state at the front and pop it
            delete q.front();
            q.pop();
            //next iteration
            continue;
        }

        //set location in graph to point towards state at front of queue
        graph[current.a][current.b] = q.front();
        q.pop();

        //if current state is the goal state, backtrack (print) and break
        if(current.a == gol1 && current.b == gol2){
            backtrack(current);
            solvable = true;
            break;
        }

        //check all 6 possible pours
        if(stateExists(current.c, current.a, lim1)){
            int C = current.c;
            int A = current.a;
            pour(C, A, lim1);

            //setup new state with poured values
            State* newState = new State(A, current.b, C, "C to A", current.c - C);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
        if(stateExists(current.b, current.a, lim1)){
            int B = current.b;
            int A = current.a;
            pour(B, A, lim1);

            //setup new state with poured values
            State* newState = new State(A, B, current.c, "B to A", current.b - B);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
        if(stateExists(current.c, current.b, lim2)){
            int C = current.c;
            int B = current.b;
            pour(C, B, lim2);

            //setup new state with poured values
            State* newState = new State(current.a, B, C, "C to B", current.c - C);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
        if(stateExists(current.a, current.b, lim2)){
            int A = current.a;
            int B = current.b;
            pour(A, B, lim2);

            //setup new state with poured values
            State* newState = new State(A, B, current.c, "A to B", current.a - A);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
        if(stateExists(current.b, current.c, lim3)){
            int B = current.b;
            int C = current.c;
            pour(B, C, lim3);

            //setup new state with poured values
            State* newState = new State(current.a, B, C, "B to C", current.b - B);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
        if(stateExists(current.a, current.c, lim3)){
            int A = current.a;
            int C = current.c;
            pour(A, C, lim3);

            //setup new state with poured values
            State* newState = new State(A, current.b, C, "A to C", current.a - A);
            newState->parent = graph[current.a][current.b];

            //push new state to queue
            q.push(newState);
        }
    }
    
    //if it wasn't solved after traversing all pours, must be no solution
    if(solvable == false){ cout << "No solution." << endl; }

    //delete all states left in queue
    while(!q.empty()){
        delete q.front();
        q.pop();
    }
    
    //delete all states left in graph and delete graph
    for(int i = 0; i < lim1+1; i++){
        for(int j = 0; j < lim2+1; j++){
            if(graph[i][j] != nullptr){
                delete graph[i][j];
            }
        }
        delete [] graph[i];
    }
    delete [] graph;
}

//function to pass arguments to the stringstream
bool passArg(char * arg, int &var, istringstream &iss){
    iss.clear();
    iss.str(arg);
    if(iss >> var){ return true; }
    return false;
}

//checks all bounds of arguments
int argCheck(int lim1, int lim2, int lim3, int gol1, int gol2, int gol3){
    if(lim1 <= 0){ return -1; } 
    if(lim2 <= 0){ return -2; } 
    if(lim3 <= 0){ return -3; }
    if(gol1 < 0){ return -4; }  
    if(gol2 < 0){ return -5; }  
    if(gol3 < 0){ return -6; }
    if(lim1 < gol1){ return 1; }
    if(lim2 < gol2){ return 2; }
    if(lim3 < gol3){ return 3; }
    if(gol1 + gol2 + gol3 != lim3){ return 4; }
    return 0;
}

int main(int argc, char * const argv[]){

    //checks if number of arguments matches expected
    if(argc < 7 || argc > 7){
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
        return 0;
    }

    //init array for limits and goals
    int lim[3], gol[3];

    //pass the arguments while checking that they are integer numbers
    istringstream iss;
    for(int i = 1; i < argc; i++){
        if(i < 4){
            if(!(passArg(argv[i], lim[i-1], iss))){
                cout << "Error: Invalid capacity '" << argv[i] << "' for jug " << ((i == 1)? "A." : (i == 2)? "B." : "C.");
                cout << endl;
                return 0;
            }
        }else{
            if(!(passArg(argv[i], gol[i-4], iss))){
                cout << "Error: Invalid goal '" << argv[i] << "' for jug " << ((i == 4)? "A." : (i == 5)? "B." : "C.");
                cout << endl;
                return 0;
            }
        }
    }

    //check if the arguments match the expected bounds
    int check = argCheck(lim[0], lim[1], lim[2], gol[0], gol[1], gol[2]);

    if(check == 0){
        //run bfs given the limits and goals
        bfs(lim[0], lim[1], lim[2], gol[0], gol[1], gol[2]);
        return 1;

    }else if(check == 4){
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return 0;
    }else if(check == -1 || check == -2 || check == -3){
        cout << "Error: Invalid capacity '" << ((check == -1)? lim[0] : (check == -2)? lim[1] : lim[2]) << "' for jug " << ((check == -1)? "A." : (check == -2)? "B." : "C.") << endl;
        return 0;
    }else if(check == -4 || check == -5 || check == -6){
        cout << "Error: Invalid goal '" << ((check == -4)? gol[0] : (check == -5)? gol[1] : gol[2]) << "' for jug " << ((check == -4)? "A." : (check == -5)? "B." : "C.") << endl;
        return 0;
    }

    cout << "Error: Goal cannot exceed capacity of jug " << ((check == 1)? "A." : (check == 2)? "B." : "C.") << endl;
    return 0;
}   