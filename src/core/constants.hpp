// constants.hpp
#ifndef QR_CODE_GEN_SRC_CORE_QR_CONSTANTS_HPP
#define QR_CODE_GEN_SRC_CORE_QR_CONSTANTS_HPP

enum class QREncodingMode {
    NUMERIC = 0,
    ALPHANUMERIC,
    BYTE,
    KANJI,
    UNSUPPORTED
};

enum QRErrorCorrectionLevel {
    L,  // Recovers 7% of data
    M,  // Recovers 15% of data
    Q,  // Recovers 25% of data
    H,  // Recovers 30% of data
    UNKNOWN
};

#endif // QR_CODE_GEN_SRC_CORE_QR_CONSTANTS_HPP