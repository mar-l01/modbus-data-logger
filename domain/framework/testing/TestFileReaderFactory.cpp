#include "domain/framework/includes/FileReaderFactory.hpp"

#include "gtest/gtest.h"


namespace {

using namespace Framework;

TEST(TestFileReaderFactory, createFileReaderReturnJsonFileAccessor)
{
    auto JsonFileAccessor = FileReaderFactory::createFileReader(FileReaderFramework::NLOHMANN_JSON);
    EXPECT_NE(JsonFileAccessor, nullptr);
}

TEST(TestFileReaderFactory, createFileReaderReturnNullptr)
{
    auto noFileReaderInstance = FileReaderFactory::createFileReader(FileReaderFramework::OTHER_READER_FRAMEWORK);
    EXPECT_EQ(noFileReaderInstance, nullptr);
}

}
