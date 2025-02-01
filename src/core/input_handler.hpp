// input_handler.hpp
#ifndef QR_CODE_GEN_SRC_CORE_INPUT_HANDLER_HPP
#define QR_CODE_GEN_SRC_CORE_INPUT_HANDLER_HPP

#include "constants.hpp"

#include <string_view>

class QRCodeMetaData;

class InputHandler {
public:
    static bool validateAndParse(std::string_view input, QRCodeMetaData&, const QRErrorCorrectionLevel);

private:
    static bool validate(std::string_view input);

    static bool parse(std::string_view input, QRCodeMetaData& metaData, const QRErrorCorrectionLevel);
    static QREncodingMode identifyEncodingMode(std::string_view inputText);

    static std::string getCharacterCountIndicator(std::string_view data, QREncodingMode);

private:
    static bool isAlphanumeric(char ch);
    static bool isByte(char ch);
};


#endif // QR_CODE_GEN_SRC_CORE_INPUT_HANDLER_HPP