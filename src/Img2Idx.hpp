#include <string>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;
/**
 * TODO Use boost::bimap instead
 */
class Img2Idx {
    static map<string, int> index;
    static map<int, string> invIndex;
    static int nextIdx;
    public:
    static int getImgIdx(string &img) {
        if (! Img2Idx::index.count(img)) {
            Img2Idx::invIndex[Img2Idx::nextIdx] = img;
            Img2Idx::index[img] = Img2Idx::nextIdx;
            Img2Idx::nextIdx++;
        }
        return Img2Idx::index[img];
    }

    static string getImgName(int id) {
        if (Img2Idx::invIndex.count(id)) {
            return Img2Idx::invIndex[id];
        }
        return "";
    }

    /**
     * Writes the id => img_id mapping to a file
     * The img_id on line # i corresponds to id = i
     */
    static void dumpIdx(string fpath) {
        ofstream fout(fpath.c_str());
        for (auto iter = Img2Idx::invIndex.begin();
                iter != Img2Idx::invIndex.end(); ++iter) {
            fout << iter->second << endl;
        }
        fout.close();
    }
};
map<string, int> Img2Idx::index = map<string, int>();
map<int, string> Img2Idx::invIndex = map<int, string>();
int Img2Idx::nextIdx = 1;
