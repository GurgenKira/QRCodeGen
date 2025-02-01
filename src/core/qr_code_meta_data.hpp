// qr_code_meta_data.hpp

#ifndef QR_CODE_GEN_SRC_CORE_QR_CODE_META_DATA_HPP
#define QR_CODE_GEN_SRC_CORE_QR_CODE_META_DATA_HPP

#include "constants.hpp"

#include <string>

class QRCodeMetaData {
public:
    
    QRCodeMetaData() = default;

    QREncodingMode getEncodingMode() const noexcept
    {
        return m_encoding_mode;
    }

    std::string getCharacterCountIndicator() const noexcept
    {
	    return m_character_count_indicator;
    }

    std::string getModeIndicator() const noexcept
    {
        switch (m_encoding_mode) {
            case QREncodingMode::NUMERIC: 
                return "0001";
            case QREncodingMode::ALPHANUMERIC:
                return "0010";
            case QREncodingMode::BYTE:
                return "0100";
            case QREncodingMode::KANJI:
                return "1000";
            default:;
        }
        return "";
    }

    std::string getDataToEncode() const noexcept 
    {
        return m_data_to_encode;
    }

private:

    void setEncodingMode(QREncodingMode em)
    {
        m_encoding_mode = em;
    } 

    void setCharacterCountIndicator(std::string_view ci)
    {
        m_character_count_indicator = ci;
    } 

    void setDataToEncode(std::string_view dte)
    {
        m_data_to_encode = dte;
    }


private:
    friend class InputHandler;

    QREncodingMode m_encoding_mode;
    std::string    m_character_count_indicator;
    std::string    m_data_to_encode;
};

#endif // QR_CODE_GEN_SRC_CORE_QR_CODE_META_DATA_HPP