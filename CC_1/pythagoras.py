import zmq,time
def pythagoras(zcontext, url):
    """Return the sum-of-squares of number sequences."""
    zsock = zcontext.socket(zmq.REP)
    zsock.bind('tcp://127.0.0.1:6709')   #port for pythagoras
    while True:
        numbers = zsock.recv_json()
        zsock.send_json(sum(n * n for n in numbers))

def main(zcontext):
    time.sleep(10)
    pythagoras(zcontext)


if __name__ == '__name__':
    main(zmq.Context())

