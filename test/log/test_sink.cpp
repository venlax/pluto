//
// Created by xixi on 2025-07-11.
//


#include "log/sink.h"

#include <fstream>
#include <iostream>

int main() {
    using namespace pluto;
    TLogSink<std::FILE*> sink1 {stdout};
    sink1.output("Hello World!");

    TLogSink<FILE*> sink2 {stderr};
    sink2.output("Hello World!");

    FILE* fp = fopen("log.txt", "w+");
    TLogSink<FILE*> sink3 {fp};
    sink3.output("Hello World!");

    auto sink4 = TLogSink(std::cout);
    sink4.output("Hello World!");


    std::ofstream fs("log2.txt", std::ios::out | std::ios::app);
    auto sink5 = TLogSink(fs);
    sink5.output("2Hello World!");

    auto sink6 = new TLogSink<FILE *>(stderr);
    sink6->output("Hello World!");
}