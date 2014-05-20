I/O
---
Compute tracks from the matches in the following format:

Directory structure
```bash
matches
  |
  +--img_id1
      |
      +-- matches_<img_id2>.txt
```
Each file is a CSV file with each row as
<SIFT Kpt index in image 1, SIFT kpt index in image 2>
The above index is the index of the keypoints returned by `vl_sift` from the corresponding images
