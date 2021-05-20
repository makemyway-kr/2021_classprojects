import zmq,time
def tally(zcontext):
    #tally counts how many points are in circle
    zsock = zcontext.socket(zmq.PULL)
    zsock.bind('tcp://127.0.0.1:6705')
    osock=zcontext.socket(zmq.PUSH)
    osock.bind('tcp://127.0.0.1:6840')
    p = q = 0
    while True:
        decision = zsock.recv_string()
        q += 1
        if decision == 'Y':
            p += 4
        osock.send_json([decision, p, q])

def main(zcontext):
    time.sleep(10)
    tally(zcontext)


if __name__ == '__main__':
    main(zmq.Context())
