#include <iostream>
#include <plog/Log.h>

#include "Hopfield.hpp"

using namespace std;

const int THRESHOLD = 125;

Hopfield::Hopfield (int height, int width) : width(width), height(height)
{
    size = width * height;
    weights = cv::Mat::zeros(size, size, CV_64FC1);
    for (int i = 0; i < size;)
    {
        order.push_back(i++);
    }
}

void Hopfield::addPattern (const cv::Mat & pattern)
{
    std::vector<signed char> array = matrixToVector(pattern);

    if (array.size() == 0) return;

    patterns.push_back(array);
}


void Hopfield::calculateWeightsHebbian ()
{
    for (int n1 = 0; n1 < size; n1++)
    {
        for (int n2 = n1+1; n2 < size; n2++)
        {
            //for each pattern
            for (int i = 0; i < patterns.size(); i++)
            {
                double weight = patterns[i][n1] * patterns[i][n2];
                weights.at<double>(n1, n2) += weight;///patterns.size();
                weights.at<double>(n2, n1) += weight;
            }
        }
    }

    //~ cout << "weights : " << weights << endl;
}


void Hopfield::calculateWeightsStorkey ()
{
    //for each pattern
    for (int i = 0; i < patterns.size(); i++)
    {
        vector<int> h;
        for (int n1 = 0; n1 < size; n1++)
        {
            double h1 = 0;
            //h1 = SUM(weights(n1,j)*patterns[i][j]), j!=n1 && j!=n2)
            for (int n11 = 0; n11 < size; n11++)
            {
                h1 += weights.at<double>(n1, n11) * patterns[i][n11];
            }
            h.push_back(h1);

            for (int n2 = 0; n2 < n1; n2++)
            {
                double weight = patterns[i][n1] * patterns[i][n2];

                //j!=n2
                h1 -= weights.at<double>(n1, n2) * patterns[i][n2];

                double h2 = h[n2]-weights.at<double>(n1, n2) * patterns[i][n1];

                weights.at<double>(n1, n2) += (weight-h1 * patterns[i][n2]-h2 * patterns[i][n1]);
                weights.at<double>(n2, n1) += (weight-h1 * patterns[i][n2]-h2 * patterns[i][n1]);

            }
        }
    }

}


void Hopfield::calculateWeightsOja ()
{

}


bool Hopfield::compare (cv::Mat & src, cv::Mat & dsc, int maxCountOfChanges)
{
    vector<signed char> image = matrixToVector(src);

    bool stable = false;
    char z = 0;
    int change = 0;
    while (!stable)
    {
        if (change == maxCountOfChanges)
        {
            LOGD << "Out of max count of changes";
            return false;
        }

        stable = true;
        //random order for the neurons
        random_shuffle(order.begin(), order.end());

        //on met a jour chaque neurone
        for (int i = 0; i < size; i++)
        {
            int node = order[i];
            int sum = 0;

            for (int pnode = 0; pnode < size; pnode++)
            {
                sum += weights.at<double>(node, pnode) * image[pnode];
            }
            if (sum >= 0 && image[node] == -1)
            {
                image[node] = 1;
                stable = false;
                change++;
            } else if (sum < 0 && image[node] == 1)
            {
                image[node] = -1;
                stable = false;
                change++;
            }
        }

        if (z % 100 == 0)
        {
            cv::Mat img(height, width, CV_8UC1, cv::Scalar(0));

            for (int l = 0; l < height; l++)
            {
                for (int c = 0; c < width; c++)
                {
                    if (image[l * width+c] == 1)
                    {
                        img.at<unsigned char> (l, c) = 255;
                    }
                }
            }

            static int u;
            string teub = "etape/etape_"+to_string(u++)+".jpg";
            cv::imwrite(teub, img);
        }

        z++;

    }

    cv::Mat img(height, width, CV_8UC1, cv::Scalar(0));


    for (int l = 0; l < height; l++)
    {
        for (int c = 0; c < width; c++)
        {
            if (image[l * width+c] == 1)
            {
                img.at<unsigned char>(l, c) = 255;
            }
        }
    }

    dsc = img;
    LOGI << "Image detected with "+to_string(change)+" changes";
    return true;
}

//only serialize the weights matrix
void Hopfield::serialize (string name) const
{
    cv::imwrite(name, weights);
}

void Hopfield::deserialize (string name)
{
    weights = cv::imread(name, -1);
}


vector<signed char> Hopfield::matrixToVector (const cv::Mat & matrix) const
{
    vector<signed char> array;

    if (matrix.rows == height && matrix.cols == width)
    {
        for (int i = 0; i < matrix.rows; i++)
        {
            for (int j = 0; j < matrix.cols; j++)
            {
                signed char t;
                if (matrix.at<unsigned char>(i, j) < THRESHOLD)
                {
                    t = -1;
                } else {
                    t = 1;
                }
                array.push_back(t);
            }
        }
    } else
    {
        cerr << "The matrix has not the right dimension" << endl;
    }

    return array;
}
//
//Hopfield::~Hopfield ()
//{}
