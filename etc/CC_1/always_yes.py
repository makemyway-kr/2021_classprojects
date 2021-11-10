import zmq,time


def always_yes(zcontext):
    #search if Coordinates always inside the circle.
    print("Waiting....")
    time.sleep(30)  # wait until client is ready
    isock = zcontext.socket(zmq.SUB)
    isock.connect('127.0.0.1:6700')  # bitsource #put your url if needed
    isock.setsockopt(zmq.SUBSCRIBE, b'00')
    osock = zcontext.socket(zmq.PUSH)
    osock.connect('tcp://127.0.0.1:6705')  # tally url #put your url if needed
    while True:
        isock.recv_string()
        osock.send_string('Y')


def main(zcontext):
    always_yes(zcontext)


if __name__ == '__main__':
    print("running")
    main(zmq.Context())
    time.sleep(15)
    print("program finished")
