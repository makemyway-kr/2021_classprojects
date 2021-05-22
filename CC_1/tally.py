import zmq,time
def tally(zcontext):
    #tally counts how many points are in circle
    print("Waiting....")
    time.sleep(30)  # wait until client is ready
    zsock = zcontext.socket(zmq.PULL)
    zsock.bind('tcp://127.0.0.1:6705') #put your url if needed
    osock=zcontext.socket(zmq.PUSH)
    osock.bind('tcp://127.0.0.1:6840') #put your url if needed
    p = q = 0
    while True:
        decision = zsock.recv_string()
        q += 1
        if decision == 'Y':
            p += 4
        osock.send_json([decision, p, q])

def main(zcontext):
    tally(zcontext)


if __name__ == '__main__':
    print("running")
    main(zmq.Context())
    time.sleep(15)
    print("program finished")
