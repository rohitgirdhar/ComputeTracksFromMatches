#include <cstdio>
#include <vector>
#include <sstream>

using namespace std;

class Track {
    int trackID;
    int trackStatus;
    map<int,int> points; // from imgID => ftrID
    static map< pair<int,int>, int > imgFtrToTrack; // (imgID, ftrID) => trackID
    static map<int, Track*> tracks;
    static int objCounter; // counter for Track objects
    // output values
    const static int CONSISTENT_TRACK = 1;
    const static int INCONSISTENT_TRACK = 2;

    void clearItself() {
        points.clear();
        imgFtrToTrack.clear();
    }

public:
    Track() {
        trackID = Track::objCounter++;
        trackStatus = Track::CONSISTENT_TRACK;
    }

    int getID() {
        return trackID;
    }

    int getStatus() {
        return trackStatus;
    }

    size_t size() {
        return points.size();
    }
    
    friend ostream & operator<<(ostream &os, Track& t);

    string pointsToString() {
        ostringstream oss;
        for (auto iter = points.begin(); 
                iter != points.end(); ++iter) {
            oss << Img2Idx::getImgName(iter->first) << "," << iter->second << " ";
        }
        return oss.str();
    }

    /**
     * Adds a new img feature to this Track, 
     * only if it keeps the track consistent
     */
    int addToTrack(int imgID, int ftrID) {
        if (points.count(imgID) && points[imgID] != ftrID) {
            trackStatus = Track::INCONSISTENT_TRACK;
        }
        points[imgID] = ftrID;
        Track::imgFtrToTrack[make_pair(imgID, ftrID)] = this->trackID;
        return trackStatus;
    }
   
    /**
     * Merge the 2 tracks into one. Assume tid1 is the smaller one
     */
    static void mergeTracksAndDeleteOne(int tid1, int tid2) {
        if (tracks[tid1]->size() > tracks[tid2]->size()) {
            Track::mergeTracksAndDeleteOne(tid2, tid1);
            return;
        }
        for (auto iter = tracks[tid1]->points.begin();
                iter != tracks[tid1]->points.end(); ++iter) {
            tracks[tid2]->addToTrack(iter->first, iter->second);
        }
        deleteTrack(tid1);
    }

    static Track* getNewTrack() {
        Track *tr = new Track();
        tracks[tr->getID()] = tr;
        return tr;
    }

    /**
     * @return track ID if the given image feature exists in some track.
     * Else returns -1
     */
    static int getTrackID(int imgID, int ftrID) {
        if (Track::imgFtrToTrack.count(make_pair(imgID, ftrID))) {
            return Track::imgFtrToTrack[make_pair(imgID, ftrID)];
        }
        return -1;
    }

    static Track* getTrackReference(int tid) {
        if (tracks.count(tid)) {
            return tracks[tid];
        }
        return NULL;
    }

    static void deleteTrack(int tid) {
        if (!tracks.count(tid)) {
            cerr << "Track::deleteTrack " << tid << " does not exist" << endl;
            return;
        }
        tracks[tid]->clearItself(); // just to be safe
        delete tracks[tid];
        tracks.erase(tid);
    }

    static void printGoodTracks() {
        // Good tracks are:
        // 1. >2 points
        // 2. CONSISTENT_TRACK
        for (auto iter = tracks.begin(); iter != tracks.end();
                ++iter) {
            if (iter->second->getStatus() == Track::CONSISTENT_TRACK &&
                iter->second->size() > 2) {
                cout << *(iter->second) << endl;
            }
        }
    }

};
map< pair<int,int>, int > Track::imgFtrToTrack = map< pair<int,int>, int >();
map<int, Track*> Track::tracks = map<int, Track*>();
int Track::objCounter = 0;
ostream & operator<<(ostream &os, Track& t) {
    return os << t.trackID << " : " << t.pointsToString();
}
