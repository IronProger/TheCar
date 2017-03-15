//
// Created by user on 15.03.17.
//

#include <malloc.h>
#include <opencv2/core/types.hpp>
#include <assert.h>

#include "ImageVector.hpp"


uint *ImageVector::getData() const {
    return data;
}

ulong ImageVector::getSize() const {
    return size;
}

void ImageVector::add(ImageVector imageVector) {
    assert(imageVector.getSize() == size && imageVector.getSize() >= 0);
    auto src = imageVector.getData();
    for (uint i = 0; i < size; i++) {
        data[i] += src[i];
    }
}

void ImageVector::clear() {
    uint mem = size * sizeof(uint);
    data = (uint *) malloc(mem);
    for (ulong i = 0; i < size; i++) {
        data[i] = 0;
    }
}

ImageVector::ImageVector(ulong size) : size(size) {
    this->size = size;
    clear();
}

ImageVector::ImageVector() {
    size = DEFAULT_SIZE;
    clear();
}

ImageVector::ImageVector(cv::Mat mat) {
    valueOf(mat);
}

void ImageVector::valueOf(cv::Mat mat) {
    size = mat.cols * mat.rows;
    assert(size > 0);
    data = (uint *) malloc(size * sizeof(uint));

    for (uint i = 0; i < size; i++) {
        data[i] = mat.at<uint>(i) / 2 + 1;
    }
}

uint ImageVector::get(ulong n) {
    assert(n < size && n >= 0);
    return data[n];
}

uint ImageVector::set(ulong pos, uint v) {
    assert(pos < size && pos >= 0);
    data[pos] = v;
}

double ImageVector::vectorLen() {
    double result = 0;
    for (ulong i = 0; i < size; i++) {
        result += (data[i] * data[i]);
    }
    return sqrt(result);
}

ImageVector::ImageVector(std::vector<ImageVector> ivs) {
    clear();
    for (ImageVector &iv : ivs) {
        add(iv);
    }
}

ImageVector::ImageVector(std::vector<cv::Mat> mats) {
    if (!mats.empty()) {
        size = mats.at(0).cols * mats.at(0).rows;
    }
    clear();
    for (const ImageVector &mat : mats) {
        add(ImageVector(mat));
    }
}
