from multiprocessing import Process
from sys import argv
from threading import Lock, Thread
from time import sleep

counter = 0
lock = Lock()
stop = False


def work(identifier):
    global counter, lock, stop

    # with lock:
    print(f"{identifier}: work {counter}...")
    for i in range(0, 65536):
        j = i * 65536
    counter += 1


def loop(identifier):
    global stop

    while not stop:
        try:
            work(identifier)
        except KeyboardInterrupt:
            break


def start_threads():
    for i in range(0, 8):
        Thread(target=loop, args=(i,), daemon=True).start()


def start_processes():
    for i in range(0, 8):
        Process(target=loop, args=(i,), daemon=True).start()


def main():
    global stop

    usage = f"usage: {argv[0]} [thread|process]\n"

    if len(argv) < 2:
        print(usage)
        print("error: first argument not given")
        exit(1)

    if argv[1] == "thread":
        start_threads()
    elif argv[1] == "process":
        start_processes()
    else:
        print(usage)
        print("error: first argument not one of [thread|process]")

    print("Press Ctrl + C to exit...")
    while not stop:
        try:
            sleep(1)
        except KeyboardInterrupt:
            break

    stop = True


if __name__ == "__main__":
    main()
