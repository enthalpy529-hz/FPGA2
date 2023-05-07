#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Pic {
public:
    Pic(string filename) {
        // 打开图片文件
        ifstream file(filename, ios::binary);

        // 获取文件大小
        file.seekg(0, ios::end);
        size = file.tellg();
        file.seekg(0, ios::beg);

        // 读取文件内容
        data.resize(size);
        file.read(reinterpret_cast<char*>(&data[0]), size);

        // 解析图片信息
        parseHeader();
        parseData();
    }

    // 获取图片宽度
    int getWidth() const {return width;}

    // 获取图片高度
    int getHeight() const {return height;}

    // 获取指定像素的 RGB 值
    vector<unsigned char> getPixel(int x, int y) const {
        int pos = (y * width + x) * bpp;
        vector<unsigned char> pixel(bpp);
        for (int i = 0; i < bpp; i++) {
            pixel[i] = data[pos + i];
        }
        return pixel;
    }

private:
    int size;
    int width;
    int height;
    int bpp;
    vector<unsigned char> pixels;
    vector<unsigned char> data;

    void parseHeader() {
        // 判断图片类型
        if (data[0] == 'B' && data[1] == 'M') {  // BMP
            bpp = 3;  // BMP 格式每个像素占 3 个字节
            width = *(int*)&data[18];
            height = *(int*)&data[22];
        } else if (data[0] == 0xFF && data[1] == 0xD8) {  // JPEG
            bpp = 3;  // JPEG 格式每个像素占 3 个字节
            int pos = 0;
            while (pos < size - 1) {
                if (data[pos] == 0xFF && data[pos + 1] == 0xC0) {
                    height = data[pos + 5] * 256 + data[pos + 6];
                    width = data[pos + 7] * 256 + data[pos + 8];
                    break;
                }
                pos++;
            }
        } else if (data[0] == 0x89 && data[1] == 'P' && data[2] == 'N' && data[3] == 'G') {  // PNG
            bpp = 4;  // PNG 格式每个像素占 4 个字节
            width = *(int*)&data[16];
            height = *(int*)&data[20];
        }
    }

    void parseData() {
        // 如果是 PNG 格式，需要先去除文件头部分
        if (data[0] == 0x89 && data[1] == 'P' && data[2] == 'N' && data[3] == 'G') {
            data.erase(data.begin(), data.begin() + 33);
            size -= 33;
        }

        // 如果是 BMP 或 JPEG 格式，需要去除文件头部分
        if (data[0] == 'B' && data[1] == 'M') {
            data.erase(data.begin(), data.begin() + 14);
            size -= 14;
        } 
        else if (data[0] == 0xff && data[1] == 0xd8) {
            int i = 2;
            while (i < size) {
                if (data[i] == 0xff && data[i+1] == 0xc0) {
                data.erase(data.begin(), data.begin() + i);
            size -= i;
            break;
            }
            i++;
            }
            }
// 解析图像宽度和高度
    width = ((unsigned char)data[3] << 8) | (unsigned char)data[2];
    height = ((unsigned char)data[5] << 8) | (unsigned char)data[4];

    // 根据图像类型解析像素数据
    switch (data[1]) {
        case 0x02: // Truecolor
            for (int i = 0; i < width * height * 3; i += 3) {
                unsigned char r = data[headerSize + i + 2];
                unsigned char g = data[headerSize + i + 1];
                unsigned char b = data[headerSize + i];
                pixels.emplace_back(r, g, b);
            }
            break;
        case 0x0a: // Truecolor with alpha
            for (int i = 0; i < width * height * 4; i += 4) {
                unsigned char r = data[headerSize + i + 2];
                unsigned char g = data[headerSize + i + 1];
                unsigned char b = data[headerSize + i];
                unsigned char a = data[headerSize + i + 3];
                pixels.emplace_back(r, g, b, a);
            }
            break;
        case 0x0c: // Grayscale with alpha
            for (int i = 0; i < width * height * 2; i += 2) {
                unsigned char v = data[headerSize + i];
                unsigned char a = data[headerSize + i + 1];
                pixels.emplace_back(v, v, v, a);
            }
            break;
        default: // Unknown or unsupported type
            throw std::runtime_error("Unknown or unsupported image type.");
    }
}
}