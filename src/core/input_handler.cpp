// input_handler.cpp

#include "input_handler.hpp"

#include "qr_code_meta_data.hpp"

#include <cctype>
#include <iostream>
#include <bitset>

// I am sure there is a better solution for this
namespace {

struct Capacity {
    int numeric;
    int alphanumeric;
    int byteMode;
    int kanji;
};
    
Capacity getVersion4Capacity(QRErrorCorrectionLevel ecLevel) {
    switch (ecLevel) {
        case L: return {187, 114, 78, 48};
        case M: return {149, 90, 62, 38};
        case Q: return {111, 67, 46, 28};
        case H: return {82, 50, 34, 21};
        default: return {0, 0, 0, 0};
    }
}

bool fitsInVersion4(std::string_view data, QREncodingMode mode, QRErrorCorrectionLevel ecLevel) {
    Capacity capacity = getVersion4Capacity(ecLevel);
    int dataLength = data.length();

    switch (mode) {
        case QREncodingMode::NUMERIC:
            return dataLength <= capacity.numeric;
        case QREncodingMode::ALPHANUMERIC:
            return dataLength <= capacity.alphanumeric;
        case QREncodingMode::BYTE:
            return dataLength <= capacity.byteMode;
        default:
            return false;
    }
}

};

bool
InputHandler::validateAndParse(std::string_view input, QRCodeMetaData& metaData, const QRErrorCorrectionLevel ecLevel) {
    if (!validate(input)) {
        return false;
    }
    return parse(input, metaData, ecLevel);
}

bool
InputHandler::validate(std::string_view input) {
    // TODO check the length restrictions !!!
    return !input.empty() && input.length() <= 2953;
}

bool
InputHandler::parse(std::string_view input, QRCodeMetaData& metaData, const QRErrorCorrectionLevel ecLevel)  {
    auto em = identifyEncodingMode(input);
    if (!fitsInVersion4(input, em, ecLevel)) { 
        return false;
    }
    metaData.setDataToEncode(input);
    metaData.setEncodingMode(em);
    metaData.setCharacterCountIndicator(getCharacterCountIndicator(input, metaData.getEncodingMode()));
    return true;
}

// Numeric mode is for the digits 0 through 9. 
// Alphanumeric mode is for the decimal digits 0 through 9, as well as uppercase letters, and the symbols $, %, *, +, -, ., /,:, and space.
// Byte mode is for characters from the ISO-8859-1 character set.
// Kanji mode is for the double-byte characters from the Shift JIS character set.
QREncodingMode 
InputHandler::identifyEncodingMode(std::string_view inputText) {
    QREncodingMode mode = QREncodingMode::NUMERIC;

    for (char ch : inputText) {
        if (mode == QREncodingMode::NUMERIC) {
            if ((std::isdigit(static_cast<unsigned char>(ch)))) {
                continue;
            }
            mode = QREncodingMode::ALPHANUMERIC;
        }

        if (mode == QREncodingMode::ALPHANUMERIC) {
            if (isAlphanumeric(ch)) {
                continue;
            }
            mode = QREncodingMode::BYTE;
        }

        if (mode == QREncodingMode::BYTE) {
            if (isByte(ch)) {
                continue;
            }

            #ifdef DEBUG
            std::cerr << "Error: Unsupported character found: " << ch << std::endl;
            #endif

            return QREncodingMode::UNSUPPORTED;
        }
    }
    return mode;
}

std::string InputHandler::
getCharacterCountIndicator(std::string_view data, QREncodingMode mode) {
    int charCount = data.length();
    int numBits;

    switch (mode) {
        case QREncodingMode::NUMERIC:
            numBits = 10;
            break;
        case QREncodingMode::ALPHANUMERIC:
            numBits = 9;
            break;
        case QREncodingMode::BYTE:
        case QREncodingMode::KANJI:
            numBits = 8;
            break;
        default:
            numBits = 0;
    }

    std::bitset<16> binary(charCount);  // Use a large enough bitset to handle the conversion
    std::string binaryString = binary.to_string();

    // Get the rightmost numBits bits
    return binaryString.substr(16 - numBits, numBits);
}

bool InputHandler::isAlphanumeric(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || 
                                            (ch == ' ') ||
                                            (ch == '$') ||
                                            (ch == '%') ||
                                            (ch == '*') ||
                                            (ch == '+') ||
                                            (ch == '-') ||
                                            (ch == '.') ||
                                            (ch == '/') ||
                                            (ch == ':');
}

bool InputHandler::isByte(char ch) {
    return ch >= 0 && ch <= 255;
}
