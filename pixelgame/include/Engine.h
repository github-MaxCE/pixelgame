constexpr unsigned int str2int(const char* data, size_t const size) noexcept
{
    int hash = 5381;

    for (const char* c = data; c < data + size; ++c)
        hash = ((hash << 5) + hash) + (unsigned char)*c;

    return hash;
}