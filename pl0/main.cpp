#include <iostream>
#include "Actuator.h"
#include "Compiler.h"
int main(int argc, char *argv[]){
    std::cout<<argv[0]<<std::endl;
    if(argc == 1){
        std::cout<<"Missing parameter."<<std::endl;
        exit(0);
    }
    int i = 1;
    std::string intput = "";
    std::string output = "assembly.pl0";
    bool save = false;
    while(i < argc){
        std::string a(argv[i]);
        if(a == "-i"){
            if(i + 1 < argc){
                intput = argv[i + 1];
            }
        }
        else if(a == "-o"){
                if(i + 1 < argc){
                    output = argv[i + 1];
            }
        }
        else if(a == "-s"){
            save = true;
        }
        i += 2;
    }

    // std::cout<<"Please input source file name:"<<std::endl;
    // std::string a;
    // std::cin>>a;
    // std::cout<<a<<std::endl;
    GrammarAnalysis A(intput);
    A.compile();
    Actuator B(A);
    B.interpret();
    if(save){
        std::cout<<"Write in "<<output<<std::endl;
        std::ofstream out;
        out.open(output, std::ios::out);
        for(auto i : A.code){
            out<<mnemonic[i.f]<<"   "<<i.l<<"   "<<i.a<<std::endl;
        }
        out.close();
    }
    return 0;
}