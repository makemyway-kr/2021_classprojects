import zmq


def always_yes(zcontext):
    """Coordinates in the lower-left quadrant are inside the unit circle."""
    isock = zcontext.socket(zmq.SUB)
    isock.connect('127.0.0.1:6700')
    isock.setsockopt(zmq.SUBSCRIBE, b'00')
    osock = zcontext.socket(zmq.PUSH)
    osock.connect('127.0.0.1:6705')  # tally url
    while True:
        isock.recv_string()
        osock.send_string('Y')


def main(zcontext):
    always_yes(zcontext)


if __name__ == '__main__':
    main(zmq.Context())
