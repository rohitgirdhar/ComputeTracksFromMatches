#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Match {
public:
    int ftr_id1;
    int ftr_id2;
    Match(int id1, int id2): ftr_id1(id1), ftr_id2(id2) {}
    static vector<Match> readMatchesFile(string fname) {
        ifstream fin(fname.c_str());
        vector<Match> res;
        string line;
        while (getline(fin, line)) {
            int pos = line.find(',');
            Match m(stoi(line.substr(0, pos)), stoi(line.substr(pos+1)));
            res.push_back(m);
        }
        fin.close();
        return res;
    }
};
