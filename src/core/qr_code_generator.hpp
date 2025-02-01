// qr_code_generator.hpp

#ifndef QR_CODE_GEN_SRC_CORE_QR_CODE_GENERATOR_HPP
#define QR_CODE_GEN_SRC_CORE_QR_CODE_GENERATOR_HPP

#include "constants.hpp"

#include <string>

class QRCodeMetaData; 

class QRCodeGenerator {
public:
    static std::string generateBitString(const QRCodeMetaData&, const QRErrorCorrectionLevel);
};

#endif // QR_CODE_GEN_SRC_CORE_QR_CODE_GENERATOR_HPP