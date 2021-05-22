import zmq,time
def pythagoras(zcontext):
    print("Waiting....")
    time.sleep(30)  # wait until client is ready
    """Return the sum-of-squares of number sequences."""
    zsock = zcontext.socket(zmq.REP)
    zsock.bind('tcp://127.0.0.1:6709')   #port for pythagoras #put your url if needed
    while True:
        numbers = zsock.recv_json()
        zsock.send_json(sum(n * n for n in numbers))

def main(zcontext):
    pythagoras(zcontext)


if __name__ == '__main__':
    print("running")
    main(zmq.Context())
    time.sleep(15)
    print("program finished")