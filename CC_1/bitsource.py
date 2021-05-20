import random, zmq, time

B = 32


def ones_and_zeros(digits):
    # expresses number in digits
    return bin(random.getrandbits(digits)).lstrip('0b').zfill(digits)


def bitsource(zcontext):
    # Produce random points in square
    insock = zcontext.socket(zmq.PULL)
    insock.bind('tcp://127.0.0.1:6900')  # url of client
    zsock = zcontext.socket(zmq.PUB)
    zsock.bind('tcp://127.0.0.1:6700')  # url of bitsource
    count = 0
    numrecv = insock.recv_string()
    seq = int(numrecv)
    while count < seq:
        zsock.send_string(ones_and_zeros((B * 2)))
        time.sleep(0.01)


def main(zcontext):
    time.sleep(30)  # wait
    bitsource(zcontext)


if __name__ == '__main__':
    main(zmq.Context())
