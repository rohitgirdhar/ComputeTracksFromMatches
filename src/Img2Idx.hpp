#include <string>
#include <map>

using namespace std;

class Img2Idx {
    static map<string, int> index;
public:
    static int getImgIdx(string &img) {
        if (Img2Idx::index.count(img)) return Img2Idx::index[img];
        return Img2Idx::index[img] = Img2Idx::index.size();
    }
};
map<string, int> Img2Idx::index = map<string, int>();
