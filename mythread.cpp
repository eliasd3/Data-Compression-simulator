    #include "mythread.h"
#include <qobject.h>
#include <QtCore>
#include <qdebug.h>
#include "compressor.h"
#include "mytabpage.h"
#include "/home/eliasderuytter/Documents/sprintz/sprintz.h"
#include <zlib.h>
#include <chrono>
#include <zstd.h>
#include <bitset>

using namespace std;


#define CHUNK 16384



bool compressData(const vector<uint16_t>& input_data, std::vector<uint8_t>& output_data, int compression_level = Z_DEFAULT_COMPRESSION) {
    // based on: http://zlib.net/zlib_how.html
    z_stream strm;
    int ret;

    // Initialize zlib stream for compression
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, compression_level);
    if (ret != Z_OK) {
        return false;
    }

    // Compressed data buffer
    std::vector<uint8_t> compressed_buffer(CHUNK);

    // Initialize input and output pointers
    strm.avail_in = input_data.size() * sizeof(uint16_t);
    strm.next_in = reinterpret_cast<Bytef*>(const_cast<uint16_t*>(input_data.data()));
    strm.avail_out = CHUNK;
    strm.next_out = compressed_buffer.data();

    // Compress input data
    do {
        ret = deflate(&strm, Z_FINISH); //Z_finish instead of flush
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&strm);
            return false;
        }
        // Move compressed data to output vector
        size_t have = CHUNK - strm.avail_out;
        output_data.insert(output_data.end(), compressed_buffer.begin(), compressed_buffer.begin() + have);
        strm.avail_out = CHUNK;
        strm.next_out = compressed_buffer.data();
    } while (ret != Z_STREAM_END);

    // Clean up
    deflateEnd(&strm);
    return true;
}


bool decompressData(const std::vector<uint8_t>& input_data, std::vector<uint16_t>& output_data) {
    z_stream strm;
    int ret;

    // Initialize zlib stream for decompression
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return false;
    }

    // Decompressed data buffer
    std::vector<uint16_t> decompressed_buffer(CHUNK);

    // Initialize input and output pointers
    strm.avail_in = input_data.size();
    strm.next_in = const_cast<Bytef*>(input_data.data());
    strm.avail_out = CHUNK * sizeof(uint16_t);
    strm.next_out = reinterpret_cast<Bytef*>(decompressed_buffer.data());

    // Decompress input data
    do {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            inflateEnd(&strm);
            return false;
        }
        // Move decompressed data to output vector
        size_t have = CHUNK * sizeof(uint16_t) - strm.avail_out;
        output_data.insert(output_data.end(), decompressed_buffer.begin(), decompressed_buffer.begin() + have / sizeof(uint16_t));
        strm.avail_out = CHUNK * sizeof(uint16_t);
        strm.next_out = reinterpret_cast<Bytef*>(decompressed_buffer.data());
    } while (ret != Z_STREAM_END);

    // Clean up
    inflateEnd(&strm);
    return true;
}

bool compressDataZstd(const std::vector<uint16_t>& input_data, std::vector<uint8_t>& output_data, int compression_level = 3) { //22 = highest compression ratio, 3 standard
    size_t max_compressed_size = ZSTD_compressBound(input_data.size() * sizeof(uint16_t));
    output_data.resize(max_compressed_size);

    size_t compressed_size = ZSTD_compress(output_data.data(), max_compressed_size, input_data.data(), input_data.size() * sizeof(uint16_t), compression_level);
    if (ZSTD_isError(compressed_size)) {
        qDebug() << "Compression error:" << ZSTD_getErrorName(compressed_size);
        return false;
    }

    output_data.resize(compressed_size);
    return true;
}

bool decompressDataZstd(const std::vector<uint8_t>& input_data, std::vector<uint16_t>& output_data) {
    unsigned long long decompressed_size = ZSTD_getFrameContentSize(input_data.data(), input_data.size());
    if (decompressed_size == ZSTD_CONTENTSIZE_ERROR) {
        qDebug() << "Decompression error: not a valid zstd frame";
        return false;
    } else if (decompressed_size == ZSTD_CONTENTSIZE_UNKNOWN) {
        qDebug() << "Decompression error: original size unknown";
        return false;
    }

    output_data.resize(decompressed_size / sizeof(uint16_t));
    size_t actual_decompressed_size = ZSTD_decompress(output_data.data(), decompressed_size, input_data.data(), input_data.size());
    if (ZSTD_isError(actual_decompressed_size)) {
        qDebug() << "Decompression error:" << ZSTD_getErrorName(actual_decompressed_size);
        return false;
    }

    return true;
}


//the thread will do all computation regarding compression

compressor *Thread::getC() const
{
    return c;
}

void Thread::setC(compressor *newC)
{
    c = newC;
}

void Thread::newValuePredicted(int err)
{
    msleep(tab->getTimeBetweenValues()); //sleep can only happen in this class, qt thread class
    while (tab->getPaused()) {
        // If paused, wait until resumed
        msleep(100);
    }

    compressedStream.push_back((double) err);

    emit deltaCompressedDataUpdate(tab->getOriginalData(), compressedStream, vector<double>() ,tab->getTabNumber());
}

void Thread::fireCompressedDataUpdate(int i)
{
    for (int64_t j = 0; j < i; ++j) {


        string dataAsString = std::to_string(fireCompressedData[j]);

        // Append the string to the compressedData vector
        compressedData.push_back(dataAsString);
    }

    // qDebug() << compressedData;

    emit fireDataUpdate(compressedData,tab->getTabNumber());
}

TabPage *Thread::getTab() const
{
    return tab;
}

void Thread::setHowManySleepsFire(int newHowManySleepsFire)
{
    delayedTime += newHowManySleepsFire;
}

Thread::Thread(TabPage *tab)
{
    this->tab = tab;
}

void Thread::run(){

    // delete c;
    c = new compressor();
    connect(this,&Thread::ratiosUpdates,tab->pointerToMainWindow,&MainWindow::handleRatiosUpdates);

    if(tab->getAlgorithmName() == QString("Delta")){
        //preprocessing:
        vector<double> decimatedStream = c->decimate(tab->getOriginalData(),tab->getDigitsAfterDecimal());
        vector<double> deltaDecodedValues = vector<double>();
        bool end = false;

        while(!end) {
            if (tab->getPaused()) {
                // If paused, wait until resumed
                msleep(100);
                continue;
            }

            //DELTA:
            vector<double> encodedValues = c->deltaEncodeNext(decimatedStream);


            if (isnan(encodedValues.back())) {
                // All values have been encoded, exit the loop
                tab->setRunning(false);

                auto start_time = std::chrono::system_clock::now();

                deltaDecodedValues = c->deltaDecode(encodedValues);

                auto end_time = std::chrono::system_clock::now();
                auto durationdeCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
                qDebug() << "deCompression time: " << durationdeCompression << " microseconds";
                tab->getWidget()->getDecompressTime()->setText(QString::number(durationdeCompression));

                tab->setDecompressedData(deltaDecodedValues);

                end = true;
            }

            msleep(tab->getTimeBetweenValues());
            // Process the encoded value, send it to the main window, etc.
            // For example:

            // tab->pointerToMainWindow->handleCompressedData(tab->getOriginalData(), encodedValues, deltaDecodedValues, tab->getTabNumber());

            emit deltaCompressedDataUpdate(tab->getOriginalData(), encodedValues, deltaDecodedValues, tab->getTabNumber());
        }
        qDebug() << "Compression time: " << (int) c->getDurationCompression().count()/1000 << " microseconds";
        tab->getWidget()->getCompressTime()->setText(QString::number((int) c->getDurationCompression().count()/1000));
    } else if (tab->getAlgorithmName() == QString("Delta + Huffman")){
        //preprocessing:
        vector<double> decimatedStream = c->decimate(tab->getOriginalData(),tab->getDigitsAfterDecimal());
        vector<string> HuffmanEncodedValues;
        vector<double> deltaDecodedValues = vector<double>();
        bool end = false;
        int totalBits = 0;

        while(!end) {
            if (tab->getPaused()) {
                // If paused, wait until resumed
                msleep(100);
                delayedTime+=100000;
                continue;
            }

            //DELTA
            vector<double> encodedValues = c->deltaEncodeNext(decimatedStream);

            if (isnan(encodedValues.back())) {
                tab->setRunning(false);

                auto start_time2 = std::chrono::system_clock::now();

                //last value has been encoded, time to decode at once:
                vector<double> huffmanDecodedValues = c->HuffmanDecode(HuffmanEncodedValues,tab->getDigitsAfterDecimal());
                // qDebug() << huffmanDecodedValues;
                deltaDecodedValues = c->deltaDecode(huffmanDecodedValues);
                // qDebug() << deltaDecodedValues;

                auto end_time2 = std::chrono::system_clock::now();
                auto durationDecompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
                qDebug() << "deCompression time: " << durationDecompression << " microseconds";
                tab->getWidget()->getDecompressTime()->setText(QString::number(durationDecompression));
                qDebug() << "Compression time: " << (int) c->getDurationCompression().count()/1000 << " microseconds";
                tab->getWidget()->getCompressTime()->setText(QString::number((int) c->getDurationCompression().count()/1000));

                tab->setDecompressedData(deltaDecodedValues);


                for (const string& str : HuffmanEncodedValues) {
                    totalBits += str.size(); // Add the size of each string
                }

                end=true;
            } else {
                //HUFFMAN
                HuffmanEncodedValues = c->HuffmanEncodeNext(encodedValues,tab->getDigitsAfterDecimal());
            }



            msleep(tab->getTimeBetweenValues());
            delayedTime+=tab->getTimeBetweenValues();
            // Process the encoded value, send it to the main window, etc.
            // For example:
            emit HuffmanDataUpdate(tab->getOriginalData(), HuffmanEncodedValues, encodedValues, deltaDecodedValues,tab->getTabNumber());
            // qDebug() << tab->getOriginalData().size() << "testttttttttttttttttttttttttttttttttttttttttttttttttttttttt";
            tab->setCompressionRatio(((double)totalBits)/((double)(tab->getOriginalData().size()*16))); //every element 16 bits

            emit ratiosUpdates(tab->getCompressionRatio(),tab->getTabNumber());

        }
    } else if (tab->getAlgorithmName() == QString("FIRE - Sprintz")){

        connect(this,&Thread::fireDataUpdate,tab->pointerToMainWindow,&MainWindow::handleFireUpdate);

        //PREPROCESSING:
        decimatedStream = c->decimate(tab->getOriginalData(),tab->getDigitsAfterDecimal());
        uint16_t input_data[decimatedStream.size()];
        size_t input_size = decimatedStream.size();//sizeof returns size in bytes, out variables consist of 2 bytes (16b)
        for (size_t i = 0; i < input_size; ++i) {
            // Multiply by 10^digitsAfterDecimal and convert to integers (16b)
            input_data[i] = static_cast<uint16_t>(decimatedStream[i] * pow(10, tab->getDigitsAfterDecimal()));
        }

        // int16_t compressed_data[input_size];
        delete[] fireCompressedData;
        fireCompressedData = new int16_t[input_size];
        memset(fireCompressedData, 0, sizeof(int16_t) * input_size);

        auto start_time = std::chrono::system_clock::now();

        // int64_t size = sprintz_compress_xff_16b(&input_data[0], input_size, &compressed_data[0], 1,true, this);
        int64_t size = sprintz_compress_xff_16b(&input_data[0], input_size, fireCompressedData, 1,true, this);

        auto end_time = std::chrono::system_clock::now();
        auto durationCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        durationCompression = durationCompression - delayedTime;
        qDebug() << "Compression time: " << durationCompression << " microseconds";
        tab->getWidget()->getCompressTime()->setText(QString::number(durationCompression));

        for (int64_t i = 0; i < size; ++i) {
            // qDebug() << "Compressed data at index" << i << ":" << fireCompressedData[i];
        }

        fireCompressedDataUpdate(size);

        uint16_t decompressed_data[input_size];
        // int64_t size2 = sprintz_decompress_xff_16b(&compressed_data[0], &decompressed_data[0]);

        auto start_time2 = std::chrono::system_clock::now();

        int64_t size2 = sprintz_decompress_xff_16b(fireCompressedData, &decompressed_data[0]);

        auto end_time2 = std::chrono::system_clock::now();
        auto durationdeCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        qDebug() << "deCompression time: " << durationdeCompression << " microseconds";
        tab->getWidget()->getDecompressTime()->setText(QString::number(durationdeCompression));

        qDebug() << "decompressed size: " << size2 << "decompress test sprintz: ";



        for (int64_t i = 0; i < size2; ++i) {
            // qDebug() << "Decompressed data at index" << i << ":" << decompressed_data[i];
        }

        vector<double> decompressed_data_vector;

        // Convert uint16_t array to vector<double>
        for (size_t i = 0; i < input_size; ++i) {
            decompressed_data_vector.push_back(static_cast<double>(decompressed_data[i]) / pow(10, tab->getDigitsAfterDecimal()));
        }

        tab->setDecompressedData(decompressed_data_vector);

        tab->setRunning(false);
        emit deltaCompressedDataUpdate(tab->getOriginalData(), compressedStream, decompressed_data_vector ,tab->getTabNumber());

        qDebug() << "original size:" << input_size << "   new size:"<< size << ";test sprintz:";

        tab->setCompressionRatio(((double)size)/((double)size2));

        emit ratiosUpdates(tab->getCompressionRatio(),tab->getTabNumber());

    } else if (tab->getAlgorithmName() == QString("Delta - Sprintz")){

        connect(this,&Thread::fireDataUpdate,tab->pointerToMainWindow,&MainWindow::handleFireUpdate);


        //PREPROCESSING:
        decimatedStream = c->decimate(tab->getOriginalData(),tab->getDigitsAfterDecimal());
        uint16_t input_data[decimatedStream.size()];
        size_t input_size = decimatedStream.size();//sizeof returns size in bytes, out variables consist of 2 bytes (16b)
        for (size_t i = 0; i < input_size; ++i) {
            // Multiply by 10^digitsAfterDecimal and convert to integers (16b)
            input_data[i] = static_cast<uint16_t>(decimatedStream[i] * pow(10, tab->getDigitsAfterDecimal()));
        }



        // int16_t compressed_data[input_size];
        delete[] fireCompressedData;
        fireCompressedData = new int16_t[input_size];
        memset(fireCompressedData, 0, sizeof(int16_t) * input_size);

        auto start_time = std::chrono::system_clock::now();
        // int64_t size = sprintz_compress_xff_16b(&input_data[0], input_size, &compressed_data[0], 1,true, this);
        int64_t size = sprintz_compress_delta_16b(&input_data[0], input_size, fireCompressedData, 1, true, this);

        auto end_time = std::chrono::system_clock::now();
        auto durationCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        durationCompression = durationCompression - delayedTime;
        qDebug() << "Compression time: " << durationCompression << " microseconds";
        tab->getWidget()->getCompressTime()->setText(QString::number(durationCompression));

        for (int64_t i = 0; i < size; ++i) {
            // qDebug() << "Compressed data at index" << i << ":" << fireCompressedData[i];
        }

        fireCompressedDataUpdate(size);

        uint16_t decompressed_data[input_size];

        auto start_time2 = std::chrono::system_clock::now();
        // int64_t size2 = sprintz_decompress_xff_16b(&compressed_data[0], &decompressed_data[0]);
        int64_t size2 = sprintz_decompress_delta_16b(fireCompressedData, &decompressed_data[0]);

        auto end_time2 = std::chrono::system_clock::now();
        auto durationdeCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        qDebug() << "deCompression time: " << durationdeCompression << " microseconds";
        tab->getWidget()->getDecompressTime()->setText(QString::number(durationdeCompression));

        qDebug() << "decompressed size: " << size2 << "decompress test sprintz: ";

        for (int64_t i = 0; i < size2; ++i) {
            // qDebug() << "Decompressed data at index" << i << ":" << decompressed_data[i];
        }

        vector<double> decompressed_data_vector;

        // Convert uint16_t array to vector<double>
        for (size_t i = 0; i < input_size; ++i) {
            decompressed_data_vector.push_back(static_cast<double>(decompressed_data[i]) / pow(10, tab->getDigitsAfterDecimal()));
        }
        tab->setDecompressedData(decompressed_data_vector);

        tab->setRunning(false);
        emit deltaCompressedDataUpdate(tab->getOriginalData(), compressedStream, decompressed_data_vector ,tab->getTabNumber());

        qDebug() << "original size:" << input_size << "   new size:"<< size << ";test sprintz:";

        tab->setCompressionRatio(((double)size)/((double)size2));

        emit ratiosUpdates(tab->getCompressionRatio(),tab->getTabNumber());
    } else if (tab->getAlgorithmName() == QString("ZLib")){

        // ********Compression********* //
        //TODO Zlib doen + mss ook compression speed is hier makkelijk
        decimatedStream = c->decimate(tab->getOriginalData(),tab->getDigitsAfterDecimal());

        std::vector<uint16_t> input_data;

        for (size_t i = 0; i < decimatedStream.size(); ++i) {
            // Multiply by 10^digitsAfterDecimal and convert to integers (16b)
            uint16_t scaled_value = static_cast<uint16_t>(decimatedStream[i] * pow(10, tab->getDigitsAfterDecimal()));
            input_data.push_back(scaled_value);
        }

        auto start_time = std::chrono::system_clock::now();
        // Compress input data
        std::vector<uint8_t> compressed_data;
        if (compressData(input_data, compressed_data)) {
            // Print compressed data size
            qDebug() << "Compressed data size: " << compressed_data.size()/2 << " uncompressed size: " << input_data.size();
        } else {
            qDebug() << "Compression failed.";
        }
        auto end_time = std::chrono::system_clock::now();
        auto durationCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        qDebug() << "Compression time: " << durationCompression << " microseconds";
        tab->getWidget()->getCompressTime()->setText(QString::number(durationCompression));

        // *******Decompression********* //
        std::vector<uint16_t> decompressed_data;
        std::vector<double> decompressedDouble_values;
        auto start_time2 = std::chrono::system_clock::now();

        if (decompressData(compressed_data, decompressed_data)) {

            for (size_t i = 0; i < decompressed_data.size(); ++i) {
                double original_value = (double) decompressed_data[i] / pow(10, tab->getDigitsAfterDecimal());
                decompressedDouble_values.push_back(original_value);
            }

            // Print decompressed data size
            qDebug() << "Decompressed data size: " << decompressed_data.size();
        } else {
            qDebug() << "Decompression failed.";
        }
        auto end_time2 = std::chrono::system_clock::now();
        auto durationDecompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        qDebug() << "deCompression time: " << durationDecompression << " microseconds";
        tab->getWidget()->getDecompressTime()->setText(QString::number(durationDecompression));

        tab->setDecompressedData(decompressedDouble_values);


        // Convert compressed data to binary string format
        std::vector<std::string> compressedDataStringFormat;
        for (uint8_t byte : compressed_data) {
            std::bitset<8> bits(byte);
            compressedDataStringFormat.push_back(bits.to_string());
        }

        emit HuffmanDataUpdate(tab->getOriginalData(), compressedDataStringFormat ,std::vector<double>(tab->getOriginalData().size(), 0.0) , decompressedDouble_values ,tab->getTabNumber());

        tab->setCompressionRatio(((double)compressed_data.size()/2)/((double)decompressed_data.size()));
        emit ratiosUpdates(tab->getCompressionRatio(),tab->getTabNumber());

        tab->setRunning(false);

    } else if (tab->getAlgorithmName() == QString("ZStandard")) {
        // Compression
        auto decimatedStream = c->decimate(tab->getOriginalData(), tab->getDigitsAfterDecimal());

        std::vector<uint16_t> input_data;
        for (size_t i = 0; i < decimatedStream.size(); ++i) {
            uint16_t scaled_value = static_cast<uint16_t>(decimatedStream[i] * pow(10, tab->getDigitsAfterDecimal()));
            input_data.push_back(scaled_value);
        }

        auto start_time = std::chrono::system_clock::now();
        std::vector<uint8_t> compressed_data;
        if (compressDataZstd(input_data, compressed_data)) {
            qDebug() << "Compressed data size: " << compressed_data.size() / 2 << " uncompressed size: " << input_data.size();
        } else {
            qDebug() << "Compression failed.";
        }
        auto end_time = std::chrono::system_clock::now();
        auto durationCompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        qDebug() << "Compression time: " << durationCompression << " microseconds";
        tab->getWidget()->getCompressTime()->setText(QString::number(durationCompression));

        // Decompression
        std::vector<uint16_t> decompressed_data;
        std::vector<double> decompressedDouble_values;
        auto start_time2 = std::chrono::system_clock::now();
        if (decompressDataZstd(compressed_data, decompressed_data)) {
            for (size_t i = 0; i < decompressed_data.size(); ++i) {
                double original_value = static_cast<double>(decompressed_data[i]) / pow(10, tab->getDigitsAfterDecimal());
                decompressedDouble_values.push_back(original_value);
            }
            qDebug() << "Decompressed data size: " << decompressed_data.size();
        } else {
            qDebug() << "Decompression failed.";
        }
        auto end_time2 = std::chrono::system_clock::now();
        auto durationDecompression = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        qDebug() << "Decompression time: " << durationDecompression << " microseconds";
        tab->getWidget()->getDecompressTime()->setText(QString::number(durationDecompression));

        tab->setDecompressedData(decompressedDouble_values);

        // Convert compressed data to binary string format
        std::vector<std::string> compressedDataStringFormat;
        for (uint8_t byte : compressed_data) {
            std::bitset<8> bits(byte);
            compressedDataStringFormat.push_back(bits.to_string());
        }

        emit HuffmanDataUpdate(tab->getOriginalData(), compressedDataStringFormat, vector<double>(tab->getOriginalData().size(), 0.0), decompressedDouble_values, tab->getTabNumber());
        tab->setCompressionRatio(static_cast<double>(compressed_data.size() / 2) / static_cast<double>(decompressed_data.size()));
        emit ratiosUpdates(tab->getCompressionRatio(), tab->getTabNumber());

        tab->setRunning(false);
    }
    qDebug() << "end of run inside thread object!";
    // // Print original data
    // cout << "Original Data (TEMPS901):\n";
    // for (const auto& value : tab->getOriginalData()) {
    //     cout << value << "  ";
    // }

    // cout << '\n';
    // //Print delta compressed data
    // cout << "\nCompressed Data (Delta Encoded):\n";
    // for (const auto& valueDelta : tab->getDeltaEncodedData()) {
    //     cout << valueDelta << "  ";
    // }
    // cout << '\n';

    delete c;
}
