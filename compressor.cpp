#include "compressor.h"
#include <cmath>
#include <bitset>

// ***
//  compressor class only used for compression methods I coded myself.
// ***


using namespace std;

int compressor::getNextIndex() const
{
    return nextIndex;
}

void compressor::setNextIndex(int newNextIndex)
{
    nextIndex = newNextIndex;
}

int compressor::getNextIndex2() const
{
    return nextIndex2;
}

void compressor::setNextIndex2(int newNextIndex2)
{
    nextIndex2 = newNextIndex2;
}

std::chrono::nanoseconds compressor::getDurationCompression() const
{
    return durationCompression;
}

compressor::compressor() {}

vector<double> compressor::decimate(const vector<double>& input, int amount) {
    //amount is how many number past the comma
    vector<double> output;


    for (double value : input) {
        // Round the number to the specified number of decimal places
        double roundedValue = round(value * pow(10, amount)) / pow(10, amount);
        output.push_back(roundedValue);
    }


    return output;
}

// Function to (fully) delta encode a vector of values

// vector<double> compressor::deltaEncode(const vector<double>& input) {
//     originalData = input;
//     vector<double> output;
//     if (input.empty())
//         return output;

//     output.push_back(input[0]); // First value remains unchanged
//     for (size_t i = 1; i < input.size(); ++i) {
//         output.push_back((input[i] - input[i - 1])); // Compute the difference
//     }

//     deltaEncoded = output;
//     return output;
// }

// Function to delta encode a vector of values, returning one value at a time
vector<double> compressor::deltaEncodeNext(const vector<double>& input) {
    //todo: decimating also 1 by 1 might be better.
    auto start_time = std::chrono::system_clock::now();

    if (nextIndex >= (int) input.size()) {
        // Return a special value to indicate end of encoding
        deltaEncoded.push_back(numeric_limits<double>::quiet_NaN());

        auto end_time = std::chrono::system_clock::now();
        durationCompression += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

        return deltaEncoded;// or any other appropriate value
    }

    double encodedValue;

    if (nextIndex == 0) {
        encodedValue = input[0]; // First value remains unchanged
    } else {
        encodedValue = (input[nextIndex] - input[nextIndex - 1]); // Compute the difference
    }

    // Increment the index for the next call
    nextIndex++;
    auto end_time = std::chrono::system_clock::now();

    deltaEncoded.push_back(encodedValue);

    durationCompression += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    return deltaEncoded;
}

vector<double> compressor::deltaDecode(const vector<double>& encodedValues) {
    vector<double> decodedValues;
    double prevValue = 0.0;

    for (double delta : encodedValues) {
        double originalValue = prevValue + delta;
        decodedValues.push_back(originalValue);
        prevValue = originalValue;
    }

    return decodedValues;
}

vector<string> compressor::HuffmanEncodeNext(const vector<double>& input, int digitsAfterComma) {
    //todo: decimating also 1 by 1 might be better.

    // if (nextIndex2 >= 513) { 513 zou moeten aangepast worden!
    //     // Return a special value to indicate end of encoding
    //     huffmanEncoded.push_back("END");
    //     return huffmanEncoded;// or any other appropriate value



    string encodedValue = "";


    int deltaValue = round(input[nextIndex2] * pow(10,digitsAfterComma));//preprocessing: times ten to make it integer

    auto start_time = std::chrono::system_clock::now();

    //according to table in thesis:

    // If delta is 0, send bit 1
    if(nextIndex2 == 0){ //first number is not delta encoded, this can be sent as int_16b
        encodedValue = bitset<16>((uint16_t)deltaValue).to_string();
    } else if (deltaValue == 0) {
        encodedValue = "1"; // Convert double to string
    } else {
        // Calculate the absolute value of delta
        int absDelta = abs(deltaValue);

        // Determine the sign of delta
        string sign = (deltaValue < 0) ? "00" : "01";
        // Determine the length of the code for absDelta
        int codeLength = absDelta - 1; // amount of 1's

        // Construct the Huffman code
        encodedValue = "0";
        for (int i = 0; i < codeLength; i++) {
            encodedValue += "1";
        }
        encodedValue = encodedValue + sign;
    }

    // Increment the index for the next call
    nextIndex2++;

    auto end_time = std::chrono::system_clock::now();

    huffmanEncoded.push_back(encodedValue);


    durationCompression += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);


    return huffmanEncoded;
}

vector<double> compressor::HuffmanDecode(const vector<string>& encodedValues,int digitsAfterComma) {
    vector<double> decodedValues;
    int index = 0;

    while (index < (int) encodedValues.size()) {
        string code = encodedValues[index];

        if(index == 0){ // first element was just integer representation
            int x = stoi(code, nullptr, 2);
            decodedValues.push_back(x/ pow(10,digitsAfterComma));
            index++;
        } else if (code[0] == '1') {
            decodedValues.push_back(0); // Delta value is 0
            index++;
        } else {
            // Count the number of 1's to determine the code length
            int codeLength = 1;
            // qDebug() << encodedValues[index].size() << "TESTTTTT" << encodedValues[index][0]<< encodedValues[index][1]<< encodedValues[index][2];
            while (codeLength < (int) code.size() && code[codeLength]=='1') {
                codeLength++;
            }

            // Construct the code for absDelta
            int absDelta = codeLength; // amount of 1's
            int deltaValue = (encodedValues[index].substr(encodedValues[index].length() - 2) == "01") ? absDelta : -absDelta;

            decodedValues.push_back(deltaValue / pow(10,digitsAfterComma)); // divide by 10 to get the original value
            index++;
        }
    }
    return decodedValues;

}



