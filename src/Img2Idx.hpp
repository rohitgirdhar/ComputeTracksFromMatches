#include <string>
#include <map>

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
};
map<string, int> Img2Idx::index = map<string, int>();
map<int, string> Img2Idx::invIndex = map<int, string>();
int Img2Idx::nextIdx = 1;
