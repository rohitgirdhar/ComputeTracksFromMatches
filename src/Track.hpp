#include <cstdio>
#include <vector>

using namespace std;

class Track {
    int trackID;
    int trackStatus;
    map<int,int> points; // from imgID => ftrID
    static map< pair<int,int>, int > imgFtrToTrack; // (imgID, ftrID) => trackID
    // output values
    const static int CONSISTENT_TRACK = 1;
    const static int INCONSISTENT_TRACK = 2;
public:
    Track() {
        static int count = 0;
        trackID = count++;
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

    /**
     * Adds a new img feature to this Track, 
     * only if it keeps the track consistent
     */
    int addToTrack(int imgID, int ftrID) {
        if (points.count(imgID)) {
            return trackStatus = Track::INCONSISTENT_TRACK;
        }
        points[imgID] = ftrID;
        Track::imgFtrToTrack[make_pair(imgID, ftrID)] = this->trackID;
        return trackStatus = Track::CONSISTENT_TRACK;
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
};
map< pair<int,int>, int > Track::imgFtrToTrack = map< pair<int,int>, int >();
