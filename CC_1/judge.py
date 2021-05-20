import zmq,time

B = 32


def judge(zcontext):
    """Determine whether each input coordinate is inside the unit circle."""
    isock = zcontext.socket(zmq.SUB)
    isock.connect('127.0.0.1:6700')  # bitsource
    for prefix in b'01', b'10', b'11':
        isock.setsockopt(zmq.SUBSCRIBE, prefix)
    psock = zcontext.socket(zmq.REQ)
    psock.connect('tcp://127.0.0.1:6709')  # pythagoras
    osock = zcontext.socket(zmq.PUSH)
    osock.connect('tcp://127.0.0.1:6705') # tally
    unit = 2 ** (B * 2)
    while True:
        bits = isock.recv_string()
        n, m = int(bits[::2], 2), int(bits[1::2], 2)
        psock.send_json((n, m))
        sumsquares = psock.recv_json()
        osock.send_string('Y' if sumsquares < unit else 'N')


def main(zcontext):
    time.sleep(30)#wait
    judge(zcontext)


if __name__ == '__name__':
    main(zmq.Context())
