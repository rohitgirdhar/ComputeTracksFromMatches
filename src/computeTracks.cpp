#include <cstdio>
#include <iostream>
#include <map>
#include <sys/time.h>
#include "Img2Idx.hpp"
#include "Match.hpp"
#include "Track.hpp"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

using namespace std;

void computeTracks(string inp_dir, int tau, string output_file) {
    fs::path p(inp_dir);
    if (!fs::is_directory(p)) {
        cerr << "Input is not a directory" << endl;
        return;
    }
    timeval begin, end;
    for (auto img1_iter = fs::directory_iterator(p); 
            img1_iter != fs::directory_iterator(); ++img1_iter) {
        gettimeofday(&begin, NULL);
        string img1 = img1_iter->path().filename().string();
        int img1_id = Img2Idx::getImgIdx(img1);
        int match_file_count = 0;
        for (auto img2_iter = fs::directory_iterator(img1_iter->path());
                img2_iter != fs::directory_iterator(); ++img2_iter, match_file_count++) {
            string img2 = img2_iter->path().filename().string();
            img2 = img2.substr(strlen("matches_"));
            img2 = img2.substr(0, img2.length() - strlen(".txt"));
            int img2_id = Img2Idx::getImgIdx(img2);
            if (img1_id == img2_id) continue;
            vector<Match> matches = Match::readMatchesFile(img2_iter->path().string());
            if (matches.size() < tau) continue;
            for (int i = 0; i < matches.size(); i++) {
                Match match = matches[i];
                int t1 = Track::getTrackID(img1_id, match.ftr_id1);
                int t2 = Track::getTrackID(img2_id, match.ftr_id2);
                if (t1 != -1 && t2 != -1) {
                    Track::mergeTracksAndDeleteOne(t1, t2);
                } else if (t1 != -1) {
                    Track *tr = Track::getTrackReference(t1);
                    tr->addToTrack(img2_id, match.ftr_id2);
                } else if (t2 != -1) {
                    Track *tr = Track::getTrackReference(t2);
                    tr->addToTrack(img1_id, match.ftr_id1);
                } else {
                    Track* tr3 = Track::getNewTrack();
                    tr3->addToTrack(img1_id, match.ftr_id1);
                    tr3->addToTrack(img2_id, match.ftr_id2);
                }
            }
        }
        gettimeofday(&end, NULL);
        cerr << "Done for " << img1 << endl;
        cerr << "> Had " << match_file_count << " matching files" << endl;
        cerr << "> Time elapsed: " << end.tv_sec - begin.tv_sec << " sec" << endl;
    }
    Track::printGoodTracksNVM(output_file);
    Img2Idx::dumpIdx("Img2Idx.txt");
}

void help() {
    cerr << "Usage: ./a.out <matches_dir> <tau> <op NVM file>" << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        help();
        return -1;
    }
    computeTracks(argv[1], stoi(argv[2]), argv[3]);
    return 0;
}
