//
// Created by user on 15.03.17.
//

#ifndef DETECTIMAGESBYVECTORANGLE_IMAGEVECTOR_HPP
#define DETECTIMAGESBYVECTORANGLE_IMAGEVECTOR_HPP

#include <opencv2/core/mat.hpp>
#include <sys/types.h>

const ulong DEFAULT_SIZE = 50 * 50;

class ImageVector {
private:
    uint *data;
    ulong size;
public:
    uint *getData() const;

    ulong getSize() const;

    uint get(ulong n);

    uint set(ulong pos, uint v);

    double vectorLen();

    void add(ImageVector imageVector);

    void clear();

    void valueOf(cv::Mat mat);

    ImageVector(ulong size);

    ImageVector(cv::Mat mat);

    ImageVector(std::vector<ImageVector> ivs);

    ImageVector(std::vector<cv::Mat> mats);

    ImageVector();
};


#endif //DETECTIMAGESBYVECTORANGLE_IMAGEVECTOR_HPP
