#ifndef KVSTORE_H
#define KVSTORE_H

#include <vector>
#include <cstdint>

#define MAX_ITEMS 50000000
#define ITEM_SIZE 30

struct KVItem {
    uint32_t id;
    float score;
};

class KVStore {
public:
    KVStore(const std::string& filename, size_t num_keys); 
    ~KVStore();

    void add_item(uint32_t key, const std::vector<KVItem>& value);
    void save();
    void load();
    std::vector<KVItem> search(uint32_t key);

private:
    int fd;
    uint8_t* mmap_data;
    size_t mmap_size;
};


#endif
