#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "lib//MetaDecoder.h"

class TempFileFixture {
public:
    std::string filename;

    TempFileFixture(const std::string& content) : filename("test_temp.mp3") {
        std::ofstream file(filename, std::ios::binary);
        file.write(content.data(), content.size());
    }

    ~TempFileFixture() {
        std::remove(filename.c_str());
    }
};

TEST(FrameSizeCalculation, ValidInput) {
    std::array<uint8_t, 4> bytes = {0x00, 0x00, 0x00, 0x7F};
    EXPECT_EQ(calculateFrameSize(bytes), 127u);
}

TEST(FrameSizeCalculation, MaxValue) {
    std::array<uint8_t, 4> bytes = {0x7F, 0x7F, 0x7F, 0x7F};
    EXPECT_EQ(calculateFrameSize(bytes), 0x0FFFFFFFu);
}

TEST(MetadataBlock, BaseClassFunctionality) {
    std::vector<uint8_t> data = {0x01, 0x02};
    MetadataBlock block("TEST", 2, 0, data);

    testing::internal::CaptureStdout();
    block.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "TEST [2 bytes]");
}

TEST(TextMetadata, SimpleText) {
    std::vector<uint8_t> data = {0x00, 'H', 'e', 'l', 'l', 'o'};
    TextMetadata frame("TIT2", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[*] TIT2 (Track name): Hello");
}

TEST(WebLink, SimpleUrl) {
    std::vector<uint8_t> data = {'h','t','t','p',':','/','/','e','x','a','m','p','l','e'};
    WebLink frame("WXXX", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[*] WXXX (URL): http://example");
}

TEST(UserComment, ValidComment) {
    std::vector<uint8_t> data = {0x00, 'e','n','g', 0x00, 'T','e','s','t',' ','c','o','m','m','e','n','t'};
    UserComment frame("COMM", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[*] COMM [eng]: Test comment");
}

TEST(UserComment, InvalidComment) {
    std::vector<uint8_t> data = {0x00}; // Недостаточно данных
    UserComment frame("COMM", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.substr(0, 6), "[?] In");
}

TEST(BlockFactory, CreateTextBlock) {
    std::vector<uint8_t> data = {0x00};
    auto block = createMetadataBlock("TALB", 1, 0, data);
    EXPECT_NE(dynamic_cast<TextMetadata*>(block.get()), nullptr);
}

TEST(BlockFactory, CreateUrlBlock) {
    std::vector<uint8_t> data = {'x'};
    auto block = createMetadataBlock("WXYZ", 1, 0, data);
    EXPECT_NE(dynamic_cast<WebLink*>(block.get()), nullptr);
}

TEST(BlockFactory, CreateCommentBlock) {
    std::vector<uint8_t> data = {0x00, 'e','n','g', 0x00, 'T','e','s','t'};
    auto block = createMetadataBlock("COMM", data.size(), 0, data);
    EXPECT_NE(dynamic_cast<UserComment*>(block.get()), nullptr);
}

TEST(BlockFactory, CreateGenericBlock) {
    std::vector<uint8_t> data = {0x01, 0x02};
    auto block = createMetadataBlock("XXXX", 2, 0, data);
    EXPECT_NE(dynamic_cast<MetadataBlock*>(block.get()), nullptr);
    EXPECT_EQ(dynamic_cast<TextMetadata*>(block.get()), nullptr);
}

TEST(BlockDescription, KnownDescriptions) {
    EXPECT_EQ(getBlockDescription("TIT2"), "Track name");
    EXPECT_EQ(getBlockDescription("TALB"), "Collection");
    EXPECT_EQ(getBlockDescription("COMM"), "Note");
}

TEST(BlockDescription, UnknownDescription) {
    EXPECT_TRUE(getBlockDescription("XXXX").empty());
}

TEST(CorruptedData, InvalidFrameSize) {
    std::vector<uint8_t> data(1000, 0xFF); // Большой размер
    MetadataBlock block("TEST", 1000, 0, data);

    testing::internal::CaptureStdout();
    block.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "TEST [1000 bytes]");
}

TEST(FlagsHandling, WithFlagsSet) {
    std::vector<uint8_t> data = {0x00, 'T','e','x','t'};
    TextMetadata frame("TIT2", data.size(), 0x8000, data);

    testing::internal::CaptureStdout();
    frame.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Text"), std::string::npos);
}