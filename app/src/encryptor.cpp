#include "encryptor.h"

Encryptor::Encryptor(const std::string& key, const std::string& iv)
  : _key(key),
    _iv(iv),
    _initialized(false),
    _ctx(nullptr)
{
    _ctx = EVP_CIPHER_CTX_new();

    if(_ctx)
    {
        _error = "Failure during Encryptor initialization !";
        _initialized = true;
    }
}

Encryptor::~Encryptor()
{
    if(_ctx)
        EVP_CIPHER_CTX_free(_ctx);
}

std::string Encryptor::error()
{
    return _error;
}

bool Encryptor::isInitialized() const
{
    return _initialized;
}

bool Encryptor::encrypt(const std::string& str, std::string& encryptedstr)
{
    if(!isInitialized())
    {
        _error = "Encryptor not initialized !";
        return false;
    }

    int ret = 0;
    int len = 0;
    int finalLen = 0;

    encryptedstr.resize(str.size()+_iv.size());

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    ret = EVP_EncryptInit_ex(
        _ctx,
        EVP_aes_256_cbc(),
        NULL,
        (const byte*)_key.c_str(),
        (const byte*)_iv.c_str()
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    ret = EVP_EncryptUpdate(
        _ctx,
        (byte*)encryptedstr.data(),
        &len,
        (byte*)str.c_str(),
        str.size()
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    ret = EVP_EncryptFinal_ex(
        _ctx,
        ((byte*)encryptedstr.data()) + len,
        &finalLen
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    encryptedstr.resize(len + finalLen);

    return true;
}

bool Encryptor::decrypt(const std::string& str, std::string& decryptedstr)
{
    if(!isInitialized())
    {
        std::cerr << "Encryptor not initialized !" << std::endl;
        return false;
    }

    int ret = 0;
    int len = 0;
    int finalLen = 0;

    decryptedstr.resize(str.size()+_iv.size());

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    ret = EVP_DecryptInit_ex(
        _ctx,
        EVP_aes_256_cbc(),
        NULL,
        (const byte*)_key.c_str(),
        (const byte*)_iv.c_str()
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    ret = EVP_DecryptUpdate(
        _ctx,
        (byte*)decryptedstr.data(),
        &len,
        (byte*)str.c_str(),
        str.size()
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    ret = EVP_DecryptFinal_ex(
        _ctx,
        ((byte*)decryptedstr.data()) + len,
        &finalLen
        );

    if(!ret)
    {
        _error = ERR_error_string(ERR_get_error(), nullptr);
        return false;
    }

    decryptedstr.resize(len + finalLen);

    return true;
}