//
// Created by akhtyamovpavel on 5/1/20.
//


#include "TreeTestCase.h"
#include "Tree.h"

namespace fs = boost::filesystem;

TreeTestCase::TreeTestCase() : base_path_("./TreeTestingDir") {
  fs::create_directories(base_path_);
}

TreeTestCase::~TreeTestCase() {
    fs::remove_all(base_path_);
}

TEST_F(TreeTestCase, NodeEquality) {
  FileNode lhs1{.name="default", .is_dir=true, .children={}};
  FileNode rhs1{.name="default", .is_dir=true, .children={}};

  FileNode lhs2{.name="default", .is_dir=true, .children={lhs1, rhs1}};
  FileNode rhs2{.name="default", .is_dir=true, .children={lhs1, rhs1}};

  EXPECT_TRUE(lhs1 == rhs1);
  EXPECT_TRUE(lhs2 == rhs2);
}

TEST_F(TreeTestCase, NodeInequality) {
  FileNode lhs1{.name="default", .is_dir=false, .children={}};
  FileNode rhs1{.name="default", .is_dir=true, .children={}};

  FileNode lhs2{.name="default", .is_dir=true, .children={lhs1, rhs1}};
  FileNode rhs2{.name="not_default", .is_dir=true, .children={lhs1, rhs1}};

  FileNode lhs3{.name="default", .is_dir=true, .children={lhs1}};
  FileNode rhs3{.name="default", .is_dir=true, .children={lhs1, rhs1}};

  EXPECT_FALSE(lhs1 == rhs1);
  EXPECT_FALSE(lhs2 == rhs2);
  EXPECT_FALSE(lhs3 == rhs3);
}

TEST_F(TreeTestCase, GetTestExceptions) {
  fs::path path1{""};
  fs::path path2{"NonexistentDir"};
  fs::path path3{base_path_ / "file.txt"};
  std::ofstream ofs(path3.string());
  ofs.close();

  EXPECT_THROW(GetTree(path1.string(), false), std::invalid_argument);
  EXPECT_THROW(GetTree(path2.string(), false), std::invalid_argument);
  EXPECT_THROW(GetTree(path3.string(), false), std::invalid_argument);
}

TreeTestCaseWithDirs::TreeTestCaseWithDirs() {
  /*
 * base_path_ -- dir1
 *            -- dir2 -- file1
 *                    -- file2
 *            -- file3
 */
  fs::path dir1 = base_path_ / "dir1";
  fs::path dir2 = base_path_ / "dir2";
  fs::create_directories(dir1);
  fs::create_directories(dir2);


  fs::path file1 = dir2 / "file1.txt";
  fs::path file2 = dir2 / "file2.txt";
  fs::path file3 = base_path_ / "file3.txt";

  std::ofstream file1_os(file1.string());
  file1_os.close();
  std::ofstream file2_os(file2.string());
  file2_os.close();
  std::ofstream file3_os(file3.string());
  file3_os.close();
}

namespace {
  FileNode MakeDefaultFileNode(const fs::path& base) {
    FileNode res = {base.filename().string(), true, {}};

    FileNode dir1 = {"dir1", true, {}};
    FileNode dir2 = {"dir2", true, {}};

    FileNode file1 = {"file1.txt", false, {}};
    FileNode file2 = {"file2.txt", false, {}};
    FileNode file3 = {"file3.txt", false, {}};

    dir2.children.push_back(file1);
    dir2.children.push_back(file2);

    res.children.push_back(dir1);
    res.children.push_back(dir2);
    res.children.push_back(file3);

    return res;
  }
  FileNode MakeDefaultFileNodeDirOnly(const fs::path& base) {
    FileNode res = {base.filename().string(), true, {}};

    FileNode dir1 = {"dir1", true, {}};
    FileNode dir2 = {"dir2", true, {}};

    res.children.push_back(dir1);
    res.children.push_back(dir2);

    return res;
  }
}

std::ostream& operator<<(std::ostream& os, const FileNode& node) {
  os << node.name << " - " << node.is_dir << " - { ";
  for (const auto& child : node.children) {
    os << child.name << " ";
  }
  os << "}\n";

  for (const auto& child : node.children) {
    os << child << "\n";
  }

  return os;
}

TEST_F(TreeTestCaseWithDirs, GetTestSimple) {
/*
 * base_path_ -- dir1
 *            -- dir2 -- file1
 *                    -- file2
 *            -- file3
 */
  auto root = MakeDefaultFileNode(base_path_);

  EXPECT_EQ(root, GetTree(base_path_.string(), false));
}

TEST_F(TreeTestCaseWithDirs, GetTestDirsOnly) {
 /*
 * base_path_ -- dir1
 *            -- dir2 -- file1.txt
 *                    -- file2.txt
 *            -- file3.txt
 */
  auto root = MakeDefaultFileNodeDirOnly(base_path_);

  EXPECT_EQ(root, GetTree(base_path_.string(), true));
}

TEST_F(TreeTestCaseWithDirs, FilterRemoveEmptyBase) {

  /*
   * base_path_ -- dir1
   *            -- dir2 -- file1.txt
   *                    -- file2.txt
   *            -- file3.txt
   *
   *  Dir1 is empty and should be removed entirely
   *
   */

  fs::path dir = base_path_ / "dir1";
  FilterEmptyNodes(GetTree(dir.string(), false), dir);

  FileNode expected = MakeDefaultFileNode(base_path_);
  expected.children.erase(expected.children.begin());

  // Expecting to have the directory removed
  // EXPECT_THROW(GetTree(dir.string(), false), std::invalid_argument);
  EXPECT_EQ(expected, GetTree(base_path_.string(), false));

}

TEST_F(TreeTestCaseWithDirs, FilterRemoveEmptySubdirs) {
  /*
  * base_path_ -- dir1
  *            -- dir2 -- file1.txt
  *                    -- file2.txt
  *            -- file3.txt
  *
  *
  *   We expect dir1 to disappear
  */
  FileNode expected = MakeDefaultFileNode(base_path_);
  expected.children.erase(expected.children.begin());

  FilterEmptyNodes(GetTree(base_path_.string(), false), base_path_);


  // Expecting to have the directory removed
  EXPECT_EQ(GetTree(base_path_.string(), false), expected);

}

TEST_F(TreeTestCaseWithDirs, FilterDontRemoveBaseFile) {
  /*
  * base_path_ -- dir1
  *            -- dir2 -- file1.txt
  *                    -- file2.txt
  *            -- file3.txt
  *
  *
  *   We expect file3 to stay
  */
  FileNode expected = MakeDefaultFileNode(base_path_);

  fs::path file3 = base_path_ / "file3.txt";
  FileNode file3_node = {file3.filename().string(), false, {}};
  FilterEmptyNodes(file3_node, file3);

  EXPECT_EQ(expected, GetTree(base_path_.string(), false));

}


