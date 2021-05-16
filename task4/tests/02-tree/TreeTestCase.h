//
// Created by akhtyamovpavel on 5/1/20.
//


#pragma once

#include <gtest/gtest.h>
#include <boost/filesystem/path.hpp>

class TreeTestCase : public ::testing::Test {
 protected:
  TreeTestCase();

  ~TreeTestCase();

  boost::filesystem::path base_path_;
};

class TreeTestCaseWithDirs : public TreeTestCase {
 protected:
  TreeTestCaseWithDirs();

};
