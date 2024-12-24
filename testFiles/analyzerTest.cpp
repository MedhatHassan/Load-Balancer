#include"Analyzer.h"

int main(int argc, char const *argv[])
{
    vector<string> files = {"server1 log.txt"};
    analyzer(files, "taskGen log.txt");
    return 0;
}