
#include "qr_code_generator.hpp"

#include "qr_code_meta_data.hpp"

#include <bitset>
#include <unordered_map>

#include <iostream>

namespace {

std::string encodeAlphanumericData(std::string_view data) {
    const std::string alphanumTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

    std::string bitString;
    for (size_t i = 0; i < data.length(); i += 2) {
        if (i + 1 < data.length()) {
            // Encode pairs of characters
            int value = 45 * alphanumTable.find(data[i]) + alphanumTable.find(data[i + 1]);
            bitString += std::bitset<11>(value).to_string();
        } else {
            // Encode a single character
            int value = alphanumTable.find(data[i]);
            bitString += std::bitset<6>(value).to_string();
        }
    }
    return bitString;
}

std::string encodeByteData(std::string_view data) {
    std::string bitString;
    for (char c : data) {
        bitString += std::bitset<8>(static_cast<unsigned char>(c)).to_string();  // 8 bits for each byte
    }
    return bitString;
}

std::string encodeNumericData(std::string_view data) {
    std::string bitString;
    size_t length = data.length();

    for (size_t i = 0; i < length; i += 3) {
        // Handle the next group of up to 3 characters
        size_t groupLength = std::min(static_cast<size_t>(3), length - i);
        int value = 0;

        for (size_t j = 0; j < groupLength; ++j) {
            value = value * 10 + (data[i + j] - '0');
        }

        if (groupLength == 3) {
            bitString += std::bitset<10>(value).to_string();  // 10 bits for 3 digits
        } else if (groupLength == 2) {
            bitString += std::bitset<7>(value).to_string();   // 7 bits for 2 digits
        } else if (groupLength == 1) {
            bitString += std::bitset<4>(value).to_string();   // 4 bits for 1 digit
        }
    }

    return bitString;
}

std::string encodeData(std::string_view data, QREncodingMode mode) {
    if (mode == QREncodingMode::NUMERIC) {
        return encodeNumericData(data);
	} else if (mode == QREncodingMode::ALPHANUMERIC) {
        return encodeAlphanumericData(data);
    } else if (mode == QREncodingMode::BYTE) {
        return encodeByteData(data);
    } else {
        throw 1;
    }
    return "";
}

std::string addTerminatorAndPadding(std::string bitString, int requiredLength) {
    // Add terminator
    if (bitString.length() < requiredLength) {
        bitString += "0000";
        if (bitString.length() > requiredLength) {
            bitString = bitString.substr(0, requiredLength);
        }
    }

    // Pad to multiple of 8 bits
    while (bitString.length() % 8 != 0) {
        bitString += "0";
    }

    // Add padding bytes (0xEC and 0x11) alternately
    bool toggle = true;
    while (bitString.length() < requiredLength) {
        if (toggle) {
            bitString += "11101100"; // 0xEC
        } else {
            bitString += "00010001"; // 0x11
        }
        toggle = !toggle;
    }

    return bitString;
}

} // end of unnamed namespace

std::string
QRCodeGenerator:: 
generateBitString(const QRCodeMetaData& mData, const QRErrorCorrectionLevel ecLevel)
{
    std::string bitString = (mData.getModeIndicator() + 
                            mData.getCharacterCountIndicator() +
                            encodeData(mData.getDataToEncode(), mData.getEncodingMode()));

    #ifdef DEBUG
    std::cerr << "Mode Indicator: " << bitString << std::endl;
    #endif

    // Determine the required length based on the version and error correction level
    int requiredLength;
    switch (ecLevel) {
        case L: requiredLength = 80 * 8; break; // Version 4-L: 80 codewords
        case M: requiredLength = 64 * 8; break; // Version 4-M: 64 codewords
        case Q: requiredLength = 48 * 8; break; // Version 4-Q: 48 codewords
        case H: requiredLength = 36 * 8; break; // Version 4-H: 36 codewords
        default: requiredLength = 0; break;
    }

    return addTerminatorAndPadding(bitString, requiredLength);
}
