// Author: 2023080050 林曈 Pau Tong Lin Xu
// 计37，程序设计基础 1-2
// Date: 2023/12/10
// The program below generates a matrix and performs matrix operations.
// Configure the initial matrix generation in 'config.txt'
// Type a list of commands in 'cmd.txt'
// Output in terminal and 'log.txt' (if specified)

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// SIZE is determined at runtime
// so I use a 2d pointer to allocate dynamic memory to my matrix
int SIZE;
int** matrix; 

string LOG;
ofstream fout;

bool isLog = false;
bool BOX = false;

int getRandom(int max){ // returns an integer between 1 and max. 
    // The generation seed is created in the main() function.
    return (rand()*max/RAND_MAX)+1;
}

void output(string text, bool endline, int width){ // outputs in terminal (and log file)
    // the function will output in log file if global 'isLog' variable is true
    // for formatting purposes, the function also takes in the width and line skips as parameters

    if(width != 0){
        cout << setw(width);
        fout << setw(width);
    }
    if (isLog) {
        cout << text; 
        fout << text; 
        if (endline){
            cout << endl; 
            fout << endl;
        }
    }
    else{
        cout << text; 
        if (endline){
            cout << endl;
        }
    }
}

void normalOutput(){ // outputs the matrix in non-box format
    for (int i = 0; i<SIZE ; i++){
        output("----", false, 0);
    }
    output("", true, 0);
    for (int r =0 ; r<SIZE ; r++){
        for (int c=0; c<SIZE ; c++){
            if (matrix[r][c] == -1){
                output("*", false, 3);
            }
            else{
                output(to_string(matrix[r][c]), false, 3);
            }
            output(" ", false, 0);
        }
        output("", true, 0);
    }
    for (int i = 0; i<SIZE ; i++){
        output("----", false, 0);
    }
    output("", true, 0);
}

int getMax(){ // gets the maximum length of a values in the matrix
    int max = 0;
    for (int i = 0 ; i<SIZE ; i++){
        for (int j = 0 ; j<SIZE ; j++){
            if (to_string(matrix[i][j]).length() > max){
                max = to_string(matrix[i][j]).length();
            }
        }
    }
    return max;
}

void boxedOutput(){ // outputs the matrix in box format
    int width = getMax();

    for (int r =0 ; r<SIZE ; r++){
        for (int c=0; c<SIZE ; c++){
            output("+", false, 0);
            for (int i = 0 ; i<width ; i++){
                output("-", false, 0);
            }
        }
        output("+", true, 0);

        for (int c=0; c<SIZE ; c++){
            output("|", false, 0);
            if (matrix[r][c] == -1){
                output("*", false, width);
            }
            else{
                output(to_string(matrix[r][c]), false, width);
            }
        }
        output("|", true, 0);
    }
    
    for (int c=0; c<SIZE ; c++){
        output("+", false, 0);
        for (int i = 0 ; i<width ; i++){
            output("-", false, 0);
        }
    }
    output("+", true, 0);
}

void outputstep(){ // function chooses whether to use box format depending on global 'BOX' variable
    if(BOX){
        boxedOutput();
    }
    else{
        normalOutput();
    }
}

void compress(char direction){ // compresses the matrix
    // the direction of compression is passed in a the parameter
    // 'l' = left, 'r' = right, 'u' = up, 'd' = down 
    int a, b, r, c;
    for (a = 0; a<SIZE ; a++){
        int sum = 0;
        for (b = 0; b<SIZE ; b++){
            if (direction=='l' || direction=='r'){r = a; c = b;}
            else if (direction=='u' || direction=='d'){c = a; r = b;}
            sum += matrix[r][c];
            matrix[r][c] = 0;
        }
        if (direction=='l'){matrix[r][0] = sum;}
        else if (direction=='r'){matrix[r][SIZE-1] = sum;}
        else if (direction=='u'){matrix[0][c] = sum;}
        else if (direction=='d'){matrix[SIZE-1][c] = sum;}
    }
}

void rotate(){ // rotates matrix 90 degrees clockwise with respect to bottom right element
    int** temp;
    temp = new int*[SIZE];
    for (int i = 0; i<SIZE ; i++){
        temp[i] = new int[SIZE];
    }

    for (int r = 0; r < SIZE; r++){
        for (int c = 0; c < SIZE; c++){
            temp[r][c] = matrix[SIZE-1-c][r];
        }
    }
    for (int r = 0; r < SIZE; r++){
        for (int c = 0; c < SIZE; c++){
            matrix[r][c] = temp[r][c];
        }
    }

    for (int i = 0; i<SIZE ; i++){
        delete[] temp[i];
    }
    delete[] temp;
}

void adjust_column(char c){ // moves zeros to the top of column c
    // the order of the remaining elements are maintained
    vector<int> nums(0);
    for (int r=0; r<SIZE; r++){
        if (matrix[r][c]!=0){
            nums.push_back(matrix[r][c]);
        }
    }
    for (int i=0; i<(SIZE-nums.size()); i++){
        matrix[i][c] = 0;
    }
    int a = 0;
    for (int i=(SIZE-nums.size()); i<SIZE; i++){
        matrix[i][c] = nums[a];
        a++;
    }

}

void ablation(){
    // 1. orders each column using adjust_column()
    // 2. adds pairs
    // 3. orders each column again using adjust_column()
    // 4. adds a new 2 or 4 to a zero element
    for (int c = 0; c<SIZE ;c++){
        adjust_column(c);
        for (int i=0; i<SIZE-1; i++){
            if (matrix[i][c]==matrix[i+1][c]){
                matrix[i][c] = 0;
                matrix[i+1][c]*=2;
                i++;
            }
        }
        adjust_column(c);
    }
    bool exists_zero = false;
    for (int r =0 ; r<SIZE ; r++){
        for (int c=0; c<SIZE ; c++){
            if (matrix[r][c] == 0){
                exists_zero = true;
                break;
            }
        }
    }
    if (exists_zero){
        while(true){
            int r = getRandom(4)-1;
            int c = getRandom(4)-1;
            if (matrix[r][c] == 0){
                matrix[r][c] = pow(2, getRandom(2));
                break;
            }
        }
    }
}

void asterix_count(){ // sets each entry as the number of '*' surrounding it
    int count;
    for (int r=0 ; r<SIZE ; r++){
        for (int c=0; c<SIZE ; c++){
            count = 0;
            if (matrix[r][c] == 0){
                for(int a = -1; a < 2; a++){
                    for (int b = -1 ; b < 2 ; b++){
                        if (r+a>=0 && c+b>=0 && r+a<SIZE && c+b<SIZE){
                            if (matrix[r+a][c+b]==-1){
                                count += 1;
                            }
                        }
                    }
                }
                matrix[r][c] = count;
            }
        }
    }
}

string remove_space(string line){ // removes spaces from string
    string result = "";
    for (int i = 0; i<line.length(); i++){
        if (line[i] != ' ') {result += line[i];}
    }
    return result;
}

int str_to_num(string str){ // changes a number from string to int format
    int num = 0;
    int multiple = 1;
    for (int i = str.length()-1; i>=0 ; i--){
        num += (int(str[i])-48)*multiple;
        multiple *= 10;
    }
    return num;
}

bool is_Empty(ifstream& pFile) // checks if a file is empty
{
    return pFile.peek() == ifstream::traits_type::eof();
}

bool is_Valid(string num){ // checks if string can be converted to int
    if (!(num[0]=='-' || (num[0]>='0' && num[0]<='9'))){
        return false;
    }
    for (int i=1; i<num.length() ; i++){
        if (!(num[i]>='0' && num[i]<='9')){
            return false;
        }
    }
    return true;
}

using namespace std;

int main(){

    SIZE = 0;
    int RAND = 0;
    vector<string> commands(0);

    // HANDLING config.txt

    ifstream fin("config.txt");
    if (fin){
        while (!fin.eof()){
            string line;
            getline(fin, line);
            line = remove_space(line);
            if (line.substr(0,4) == "SIZE"){
                if (is_Valid(line.substr(5, line.length()))){
                    SIZE = str_to_num(line.substr(5, line.length()));
                }
            }
            if (line.substr(0,4) == "RAND"){
                if (is_Valid(line.substr(5, line.length()))){
                    RAND = str_to_num(line.substr(5, line.length()));
                }
            }
            if (line.substr(0,3) == "LOG"){
                LOG = line.substr(4, line.length());
                isLog = true;
            }
            if (line.substr(0,3) == "BOX"){
                if (line.substr(4, line.length()) == "TRUE"){
                    BOX = true;
                }
            }
            // any line starting with '#' is disregarded
        }
        if (!(SIZE > 0)){
            cout << "Incorrect 'config.txt' format" << endl;
            SIZE = 4;
        }
        if (!(RAND > 0)){
            RAND = 0;
        }
        fin.close();
    }
    else{
        SIZE = 4;
        RAND = 0;
        cout << "Incorrect 'config.txt' format: file not found" << endl;
    }
    
    // setting the log file to "LOG"

    if (!LOG.empty()){
        fout.open(LOG);;
    }

    // output chosen configuration

    output("SIZE=" + to_string(SIZE) + ", RAND=" + to_string(RAND), true, 0);

    // HANDLING cmd.txt

    bool cmd_exists = true;
    fin.open("cmd.txt");
    bool isEmpty_cmd = is_Empty(fin);

    if (fin){
        while (!fin.eof()){
            string line;
            getline(fin, line);
            commands.push_back(remove_space(line));
        }
        fin.close();
    }
    else{
        cout << "'cmd.txt' not found" << endl;
        cmd_exists = false; 
    }

    // GENERATING MATRIX

    srand((unsigned int)time(NULL)); // generation seed

    matrix = new int*[SIZE];
    for (int i = 0; i<SIZE; i++){
        matrix[i] = new int[SIZE];
    }

    for (int r =0 ; r<SIZE ; r++){ // filling the initial matrix
        for (int c=0; c<SIZE ; c++){
            matrix[r][c] = 0;
        }
    }

    int num;
    if (!(RAND > 0)){
        num = 1;
    }
    else {
        num = SIZE*SIZE*RAND/100;
    }

    int count = 0;
    while (count < num){ // randomly selecting elements to be filled
        int max_power = 4;
        if (num == 1){
            max_power = 2;
        }
        int r = getRandom(SIZE)-1;
        int c = getRandom(SIZE)-1;
        if (matrix[r][c] == 0){
            matrix[r][c] = pow(2, getRandom(max_power));
            count++;
        }
    }

    // MATRIX OPERATIONS

    //ofstream fout("")

    if (!cmd_exists){
        for (int r =0 ; r<SIZE ; r++){
            for (int c=0; c<SIZE ; c++){
                if (matrix[r][c] != 0){
                    matrix[r][c] = -1;
                }
            }
        }
        asterix_count();
        output("FINAL MATRIX:", true, 0);
        outputstep();
    }
    else if (isEmpty_cmd && cmd_exists){
        for (int r =0 ; r<SIZE ; r++){
            for (int c=0; c<SIZE ; c++){
                if (matrix[r][c] != 0){
                    matrix[r][c] = getRandom(20);
                }
            }
        }
        output("FINAL MATRIX:", true, 0);
        outputstep();
    }
    else {
        int counter = 1;
        
        output("INITIAL MATRIX:", true, 0);
        outputstep();

        for (int i = 0 ; i<commands.size() ; i++){
            if (commands[i][0] == 'r'){
                rotate();
                output(to_string(counter) + ". ROTATION:", true, 0);
                outputstep();
                counter++;
            }
            else if (commands[i][0] == 'a'){
                ablation();
                output(to_string(counter) + ". ABLATION:", true, 0);
                outputstep();
                counter++;
            }
            else if (commands[i][0] == 'c'){
                compress(commands[i][1]);
                output(to_string(counter) + ". COMPRESSION with direction " + commands[i][1] + ":", true, 0);
                outputstep();
                counter++;
            }
            else if (commands[i][0] == 'Q'){
                output(to_string(counter) + ". QUIT:", true, 0);
                break;
            }
        }
    }

    // END 

    fout.close();

    for (int i = 0; i < SIZE; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}