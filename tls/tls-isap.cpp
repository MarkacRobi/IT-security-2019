#include "tls-isap.h"
#include "isap128.h"
#include "endian.h"

#include <math.h>
#include "../utils/utils.h"

#define FTEEN 14
#define POWTWO 2
#define BUFFER 16
#define TLS 0x03
#define ADDER 0xff
#define SUBT 8

tls13_isap::tls13_isap(const key_storage& key, const std::vector<uint8_t>& nonce_data): nonce(nonce_data)
{
  // \todo Initialize with given key and nonce
    isap.set_key(key);
}

tls13_isap::~tls13_isap() {}

std::vector<uint8_t> tls13_isap::addAdditionalData(record record_for_ciph_txt, std::vector<uint8_t> plaintext_buffer_holder)
{
    size_t sizeOfBuffer = plaintext_buffer_holder.size();

    uint16_t sizeWithBuffer = sizeOfBuffer;
    sizeWithBuffer += BUFFER;

    uint8_t length[2] = { (uint8_t)(sizeWithBuffer & 0xff), (uint8_t)(sizeWithBuffer >> 8) };
    std::vector<uint8_t> data;
    data.push_back(record_for_ciph_txt.header.type);

    data.push_back(record_for_ciph_txt.header.version.major);
    data.push_back(record_for_ciph_txt.header.version.minor);
    data.push_back(length[1]);
    data.push_back(length[0]);

    return data;
}

tls13_isap::record tls13_isap::encrypt(content_type type, const std::vector<uint8_t>& plaintext)
{
  /// \todo Implement ciphertext record generation for given plaintext.
  //return record();
    size_t sizeOfPT = plaintext.size();
    std::vector<uint8_t> plaintext_buffer_holder;
    std::vector<uint8_t> encryption_vector;

    for (uint8_t byte: plaintext) {

        plaintext_buffer_holder.push_back(byte);
    }
    plaintext_buffer_holder.push_back((uint8_t) type);

    size_t powTo = std::pow(POWTWO, FTEEN);

    bool sizeOfL = sizeOfPT >= powTo;
    if (sizeOfL) {

        record tmp_record = record();
        return tmp_record;
    }
    bool plTextSize = sizeOfPT == 0;
    bool checkHandshake = plTextSize && (type == 22U || type == 21U);

    if (checkHandshake) {
        record tmp_record = record();
        return tmp_record;
    }


    record record_for_ciph_txt = record();

    record_for_ciph_txt.header.type = TLS_APPLICATION_DATA;


    record_for_ciph_txt.header.version.major = TLS;


    record_for_ciph_txt.header.version.minor = TLS;


    record_for_ciph_txt.header.version = TLSv1_2;

    std::vector<uint8_t> vectorWithAddData =
            addAdditionalData(record_for_ciph_txt,
                              plaintext_buffer_holder);

    isap.encrypt(encryption_vector,
                  plaintext_buffer_holder,
                  nonce.nonce(),
                  vectorWithAddData);
    record_for_ciph_txt.ciphertext = encryption_vector;
    size_t encSize = encryption_vector.size();
    record_for_ciph_txt.header.length = encSize;

    nonce.operator++();
    return record_for_ciph_txt;
}

bool tls13_isap::decrypt(const record& record, std::vector<uint8_t>& plaintext, content_type& type)
{
  /// \todo Implement decryption for the given record.
  //return false;
    std::vector<uint8_t> data;


    data.push_back(record.header.type);
    data.push_back(record.header.version.major);
    data.push_back(record.header.version.minor);

    data.push_back(record.header.length >> SUBT);
    data.push_back(record.header.length & ADDER);

    bool decrypt = isap.decrypt(plaintext,
                                 record.ciphertext,
                                 nonce.nonce(),
                                 data);

    if(decrypt == false)
    {
        return decrypt;
    }

    type = (content_type) plaintext.back();
    plaintext.pop_back();

    nonce.operator ++();
    return decrypt;
}
