import zmq, random, time


def client(zcontext):
    csock = zcontext.socket(zmq.PUSH)  # socket for pushing to bitsource
    rsock = zcontext.Socket(zmq.PULL)  # socket for receiving from tally
    csock.connect('tcp://127.0.0.1:6900')
    rsock.connect('tcp://127.0.0.1:6840')
    print("write number of points to calculate:")
    N = input()
    csock.send_string(N)
    count = 0
    while count < N:
        toprint = rsock.recv_json()
        decision = toprint[0]
        p = toprint[1]
        q = toprint[2]
        print(decision, p, q, p / q)
        count += 1


def main(zcontext):
    time.sleep(10)
    client(zcontext)


if __name__ == '__main__':
    main(zmq.Context())
