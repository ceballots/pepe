import KVStoreModule
import random
import time
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, help="Number of keys to write")
    parser.add_argument("--file", type=str, help="File location", default="data.mmap")
    args = parser.parse_args()

    kv_store = KVStoreModule.KVStore(args.file, args.n)
    time_limit = 1
    start_time = time.time()
    reads = 0

    while time.time() - start_time < time_limit:
        i = random.randint(0, args.n)
        _ = kv_store.search(i)
        reads += 1

    print(f"Number of reads {reads}")