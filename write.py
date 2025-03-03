import KVStoreModule
import random
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, help="Number of keys to write")
    parser.add_argument("--l", type=int, help="Number of values for each key", default=30)
    args = parser.parse_args()

    kv_store = KVStoreModule.KVStore("data.mmap")
    for key in range(args.n):
        if key % 100_000 == 0:
            print(f"Writting key {key}")
        items = [KVStoreModule.KVItem(i, random.random()) for i in range(args.l)]
        kv_store.add_item(key, items)

    kv_store.save()

    print(f"Successfully added {args.n} items!")
