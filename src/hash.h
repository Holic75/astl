#ifndef ASTL_HASH_H
#define ASTL_HASH_H



namespace astl 
{
    
typedef size_t hash_type;

template <class T>
hash_type hash(const T& t)
{
    const uint8_t* ptr =  reinterpret_cast<const uint8_t* >(&t);
    
    hash_type hash = 0x811c9dc5u;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        hash = (hash ^ ptr[i]) * 0x01000193u;
    }
    
    return hash;
}

}
#endif