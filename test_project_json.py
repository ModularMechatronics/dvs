import json
import time
import psutil    
import subprocess

if __name__ == "__main__":

    start_time = time.time()
    q = "dvs" in (p.name() for p in psutil.process_iter())

    end_time = time.time()
    if q:
        print("dvs is running!!!")
    else:
        print("dvs is not running...")
    print(end_time - start_time)
    a = 1