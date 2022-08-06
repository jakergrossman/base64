#include <iostream>
#include <cinttypes>
#include <map>
#include <fstream>
#include <array>

#include <getopt.h>

#include <cstring>

constexpr char encodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::map<uint8_t, char> decodeTable = {
    { 'A', 0 },
    { 'B', 1 },
    { 'C', 2 },
    { 'D', 3 },
    { 'E', 4 },
    { 'F', 5 },
    { 'G', 6 },
    { 'H', 7 },
    { 'I', 8 },
    { 'J', 9 },
    { 'K', 10 },
    { 'L', 11 },
    { 'M', 12 },
    { 'N', 13 },
    { 'O', 14 },
    { 'P', 15 },
    { 'Q', 16 },
    { 'R', 17 },
    { 'S', 18 },
    { 'T', 19 },
    { 'U', 20 },
    { 'V', 21 },
    { 'W', 22 },
    { 'X', 23 },
    { 'Y', 24 },
    { 'Z', 25 },
    { 'a', 26 },
    { 'b', 27 },
    { 'c', 28 },
    { 'd', 29 },
    { 'e', 30 },
    { 'f', 31 },
    { 'g', 32 },
    { 'h', 33 },
    { 'i', 34 },
    { 'j', 35 },
    { 'k', 36 },
    { 'l', 37 },
    { 'm', 38 },
    { 'n', 39 },
    { 'o', 40 },
    { 'p', 41 },
    { 'q', 42 },
    { 'r', 43 },
    { 's', 44 },
    { 't', 45 },
    { 'u', 46 },
    { 'v', 47 },
    { 'w', 48 },
    { 'x', 49 },
    { 'y', 50 },
    { 'z', 51 },
    { '0', 52 },
    { '1', 53 },
    { '2', 54 },
    { '3', 55 },
    { '4', 56 },
    { '5', 57 },
    { '6', 58 },
    { '7', 59 },
    { '8', 60 },
    { '9', 61 },
    { '+', 62 },
    { '/', 63 },
};

std::string encode(std::istream& input, bool pad)
{
    std::string result = "";
    std::array<char, 3> buf = { 0, 0, 0 };

    input.read(buf.data(), 3);
    auto numRead = input.gcount();
    while (!input.eof() || numRead > 0) {

        uint8_t a = (buf[0] & 0xFC) >> 2,
                b = ((buf[0] & 0x03) << 4) | ((buf[1] & 0xF0) >> 4),
                c = ((buf[1] & 0x0F) << 2) | ((buf[2] & 0xC0) >> 6),
                d = (buf[2] & 0x3F);

        switch (numRead) {
            case 3:
                result = result + encodeTable[a] + encodeTable[b] + encodeTable[c] + encodeTable[d];
                break;
            case 2:
                result = result + encodeTable[a] + encodeTable[b] + encodeTable[c];
                if (pad) {
                    result += "=";
                }
                break;
            case 1:
                result = result + encodeTable[a] + encodeTable[b];
                if (pad) {
                    result += "==";
                }
                break;
            default:
                std::cerr << "encode(): Invalid bytes read from input stream (" << numRead << ")";
                break;
        }

        std::fill(buf.begin(), buf.end(), 0);
        input.read(buf.data(), 3);
        numRead = input.gcount();
    }

    return result;
}

std::string decode(std::istream& input)
{
    std::string result = "";
    std::array<char, 4> readBuf = { 0 };
    std::array<char, 4> dataBuf = { 0 };

    do {
        std::fill(readBuf.begin(), readBuf.end(), 0);
        std::fill(dataBuf.begin(), dataBuf.end(), 0);
        input.read(readBuf.data(), readBuf.size());
        auto numRead = input.gcount();

        uint8_t a = decodeTable[readBuf[0]],
                b = decodeTable[readBuf[1]],
                c = readBuf[2] == '=' ? 0 : decodeTable[readBuf[2]],
                d = readBuf[3] == '=' ? 0 : decodeTable[readBuf[3]];


        switch (numRead) {
            case 4:
                dataBuf[2] = ((c & 0x03) << 6) | d;
                // fall through
            case 3:
                dataBuf[1] = ((b & 0x0F) << 4) | ((c & 0x3C) >> 2); 
                // fall through
            case 2:
                dataBuf[0] = (a << 2) | ((b & 0x30) >> 4);
                break;
        }

        result += dataBuf.data();
    } while (!input.eof());

    return result;
}

// print usage to specified output stream
void usage(std::ostream& out)
{
    out << "Usage: base64 [OPTION]... [FILE]..." << std::endl;
}

void help(std::ostream& out)
{
    usage(out);
    out << "Encode or decode base64 from each FILE to standard output." << std::endl;
    out << std::endl;
    out << "With no FILE, or when FILE is -, read standard input." << std::endl;
    out << std::endl;
    out << "  -e, --encode\t\tencode data to base64" << std::endl;
    out << "              \t\t  This is the default if neither encode or decode are specified" << std::endl;
    out << "  -d, --decode\t\tdecode base64 to data" << std::endl;
    out << "  -p, --padding\t\tuse padding when encoding data to base64" << std::endl;
    out << "  -h, --help\t\tshow this help message" << std::endl;
}

// helper macro for picking whether to encode or decode, as well as print trailing newlines
#define ENCODE_DECODE(file) \
do { \
    if (encodeFlag) { \
        std::cout << encode(file, padFlag); \
    } else { \
        std::cout << decode(file); \
    } \
    if (newlineFlag) std::cout << std::endl; \
} while (0)

int main(int argc, char* argv[])
{
    int ch;
    static int padFlag, encodeFlag, decodeFlag, newlineFlag = 1;
    static struct option longOptions[] = {
        { "padding", no_argument, &padFlag, 1},
        { "encode",  no_argument, &encodeFlag, 1 },
        { "decode",  no_argument, &decodeFlag, 1 },
        { "newline",  no_argument, &newlineFlag, 0 },
        { "help", no_argument, NULL, 'h' },
        { 0, 0, 0, 0 }
    };

    while (1) {
        int optionIndex = 0;
        ch = getopt_long(argc, argv, "pednh",
                         longOptions, &optionIndex);

        // detect end of options
        if (ch == -1) {
            break;
        }

        switch (ch) {
            case 0: break; // flag already set
            case 'p':
                    padFlag = 1;
                    break;
            case 'e':
                    encodeFlag = 1;
                    break;
            case 'd':
                    decodeFlag = 1;
                    break;
            case 'n':
                    newlineFlag = 0;
                    break;
            case 'h':
                    help(std::cout);
                    exit(0);
            case '?':
                    // getopt_long printed an error, print more info
                    help(std::cerr);
                    exit(1);
            default: abort();
        }
    }

    if (!encodeFlag && !decodeFlag) {
        encodeFlag = 1; // default to encoding
    } else if (encodeFlag && decodeFlag) {
        std::cerr << argv[0] << ": Only one of '--encode' or '--decode' can be used" << std::endl;
        help(std::cerr);
        exit(1);
    }

    if (decodeFlag) {
        // initialize decode table from encode table
        for (int i = 0; i < 64; i++) {
            decodeTable[encodeTable[i]] = i;
        }
    } else if (encodeFlag) {
    }

    if (optind >= argc) {
        // no file, use STDIN
        ENCODE_DECODE(std::cin);
    } else {
        for (int i = optind; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {
                ENCODE_DECODE(std::cin);
            } else {
                std::ifstream file(argv[i]);
                if (!file) {
                    std::cerr << argv[0] << ": Could not open '" << argv[i] << "' for reading" << std::endl;
                    exit(1);
                }

                ENCODE_DECODE(file);
                file.close();
            }
        }
    }
    return 0;
}
