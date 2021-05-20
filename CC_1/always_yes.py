import zmq,time


def always_yes(zcontext):
    #Coordinates always inside the circle.
    isock = zcontext.socket(zmq.SUB)
    isock.connect('127.0.0.1:6700')  # bitsource
    isock.setsockopt(zmq.SUBSCRIBE, b'00')
    osock = zcontext.socket(zmq.PUSH)
    osock.connect('tcp://127.0.0.1:6705')  # tally url
    while True:
        isock.recv_string()
        osock.send_string('Y')


def main(zcontext):
    time.sleep(30)#wait
    always_yes(zcontext)


if __name__ == '__main__':
    main(zmq.Context())
