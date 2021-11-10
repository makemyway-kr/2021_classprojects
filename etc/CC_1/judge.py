import zmq,time

B = 32


def judge(zcontext):
    print("Waiting....")
    time.sleep(30)  # wait until client is ready
    """Determine whether each input coordinate is inside the unit circle."""
    isock = zcontext.socket(zmq.SUB)
    isock.connect('127.0.0.1:6700')  # bitsource #put your url if needed
    for prefix in b'01', b'10', b'11':
        isock.setsockopt(zmq.SUBSCRIBE, prefix)
    psock = zcontext.socket(zmq.REQ)
    psock.connect('tcp://127.0.0.1:6709')  # pythagoras #put your url if needed
    osock = zcontext.socket(zmq.PUSH)
    osock.connect('tcp://127.0.0.1:6705') # tally #put your url if needed
    unit = 2 ** (B * 2)
    while True:
        bits = isock.recv_string()
        n, m = int(bits[::2], 2), int(bits[1::2], 2)
        psock.send_json((n, m))
        sumsquares = psock.recv_json()
        osock.send_string('Y' if sumsquares < unit else 'N')


def main(zcontext):
    judge(zcontext)


if __name__ == '__main__':
    print("running")
    main(zmq.Context())
    time.sleep(15)
    print("program finished")