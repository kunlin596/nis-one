#include "Feature.h"

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "Logger.h"

LOGGER("Core.Feature");

namespace MapCreator {

Feature::Feature() : type_(Type::Unknown) {}

Feature::Feature(const cv::Mat_<uchar> &image, Type type) : type_(type) {
  switch (type_) {
    case Type::ORB: {
      detector_ = cv::ORB::create();
      break;
    }
#ifdef ENABLE_OPENCV_CONTRIB
    case Type::FREAK: {
      detector_ = cv::xfeatures2d::FREAK::create();
      break;
    }
    case Type::SIFT: {
      detector_ = cv::xfeatures2d::SIFT::create();
      break;
    }
    case Type::SURF: {
      detector_ = cv::xfeatures2d::SURF::create();
      break;
    }
#endif
    default:
      break;
  }

  detector_->detectAndCompute(image, cv::Mat(), key_points_, descriptors_);
}

Feature::~Feature() {}

bool SaveFeature(const std::string &file_name, const Feature &feature) {
  std::ofstream out(file_name, std::ios::binary);

  if (out) {
    namespace bio = boost::iostreams;

    bio::filtering_ostream f;
    f.push(bio::gzip_compressor());
    f.push(out);

    boost::archive::binary_oarchive ar(out);
    ar << feature;
    return true;
  }
  return false;
}

bool LoadFeature(const std::string &file_name, Feature &feature) {
  std::ifstream in(file_name, std::ios::binary);

  if (in) {
    namespace bio = boost::iostreams;

    bio::filtering_istream f;
    f.push(bio::gzip_decompressor());
    f.push(in);

    boost::archive::binary_iarchive ar(in);
    ar >> feature;
    return true;
  }

  return false;
}

}  // namespace MapCreator
