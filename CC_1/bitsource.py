import random, zmq,time
B=32
def ones_and_zeros(digits):
    #expresses number in digits
    return bin(random.getrandbits(digits)).lstrip('0b').zfill(digits)
def bitsource(zcontext,N):
      #Produce random points in square
    zsock = zcontext.socket(zmq.PUB)
    zsock.bind('tcp://*:6700')
    count=0
    while count<N:
        zsock.send_string(ones_and_zeros(B * 2))
        time.sleep(0.01)
        count+=1
def main(N):
    bitsource(zmq.Context(),N)