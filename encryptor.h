#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include <iostream>

typedef unsigned char byte;

class Encryptor
{
private:
    std::string _key;
    std::string _iv;
    bool _initialized;
    EVP_CIPHER_CTX* _ctx;

    bool error();

public:
    Encryptor(const std::string& key, const std::string& iv);
    ~Encryptor();
    bool encrypt(const std::string& str, std::string& encryptedstr);
    bool decrypt(const std::string& str, std::string& decryptedstr);

};

#endif // ENCRYPTOR_H