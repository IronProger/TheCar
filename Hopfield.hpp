#ifndef DEF_Hopfield

#define DEF_Hopfield


#include <iostream>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/** warning: use it via Detect only */

using namespace std;

class Hopfield
{

public:

    Hopfield (int height, int width);

    void addPattern (const cv::Mat & pattern);

    void calculateWeightsHebbian ();

    void calculateWeightsStorkey ();

    void calculateWeightsOja ();

    bool compare (cv::Mat & src, cv::Mat & dsc, int maxCountOfChanges);

    void serialize (string name) const;

    void deserialize (string name);

    //~Hopfield ();

private:

    int width;
    int height;
    int size;
    cv::Mat weights;
    vector<int> order;
    vector<vector<signed char> > patterns;

    vector<signed char> matrixToVector (const cv::Mat & matrix) const;

};


#endif
