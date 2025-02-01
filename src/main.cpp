#include <iostream>
#include <cassert>

#include <core/input_handler.hpp>
#include <core/qr_code_generator.hpp>
#include <core/qr_code_meta_data.hpp>

int main()
{
    std::string input;
    std::cout << "Enter the data to encode: ";
    std::getline(std::cin, input);

    QRCodeMetaData metaData;
    const QRErrorCorrectionLevel ecLevel = QRErrorCorrectionLevel::M; 

    if (!InputHandler::validateAndParse(input, metaData, ecLevel)) {
        std::cerr << "Invalid input!" << std::endl;
        return 1;
    }

    std::string bitString = QRCodeGenerator::generateBitString(metaData, ecLevel);

    return 0;
}
