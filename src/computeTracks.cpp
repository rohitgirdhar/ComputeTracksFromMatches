#include <cstdio>
#include <iostream>
#include <map>
#include "Img2Idx.hpp"
#include "Match.hpp"
#include "Track.hpp"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

using namespace std;

void computeTracks(string inp_dir) {
    map<int, Track> tracks;
    fs::path p(inp_dir);
    if (!fs::is_directory(p)) {
        cerr << "Input is not a directory" << endl;
        return;
    }
    for (auto img1_iter = fs::directory_iterator(p); 
            img1_iter != fs::directory_iterator(); ++img1_iter) {
        string img1 = img1_iter->path().filename().string();
        int img1_id = Img2Idx::getImgIdx(img1);
        for (auto img2_iter = fs::directory_iterator(img1_iter->path());
                img2_iter != fs::directory_iterator(); ++img2_iter) {
            string img2 = img2_iter->path().filename().string();
            int img2_id = Img2Idx::getImgIdx(img2);
            vector<Match> matches = Match::readMatchesFile(img2_iter->path().string());
            for (auto match : matches) {
                int t1 = Track::getTrackID(img1_id, match.ftr_id1);
                int t2 = Track::getTrackID(img2_id, match.ftr_id2);
                if (t1 != -1) {
                    Track tr1 = tracks[t1];
                    tr1.addToTrack(img2_id, match.ftr_id2);
                } else if (t2 != -1) {
                    Track tr2 = tracks[t2];
                    tr2.addToTrack(img1_id, match.ftr_id1);
                } else {
                    Track tr3;
                    tr3.addToTrack(img1_id, match.ftr_id1);
                    tr3.addToTrack(img2_id, match.ftr_id2);
                    tracks[tr3.getID()] = tr3;
                }
            }
        }
    }
    cout << tracks.size() << endl;
}

void help() {
    cerr << "Usage: ./a.out <matches_dir>" << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
        return -1;
    }
    computeTracks(argv[1]);
    return 0;
}
