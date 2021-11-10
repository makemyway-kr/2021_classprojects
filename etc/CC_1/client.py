import zmq, random, time


def client(zcontext):
    N = input()
    print("Waiting....")
    time.sleep(35)#wait
    csock = zcontext.socket(zmq.PUSH)  # socket for pushing to bitsource
    rsock = zcontext.Socket(zmq.PULL)  # socket for receiving from tally
    csock.connect('tcp://127.0.0.1:6900')  #put your url if needed  #url for client-bitsource connection
    rsock.connect('tcp://127.0.0.1:6840') #put your url if needed  #url for tally-client connection
    print("write number of points to calculate:")
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
    client(zcontext)


if __name__ == '__main__':
    print("running")
    main(zmq.Context())
    time.sleep(15)
    print("program finished")
