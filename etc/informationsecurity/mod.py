from math import pow as mpow
def searchx(a,q,y):
    answer=0
    for i in range(0,q-1):
        if (mpow(a,i)%q)==y:
            answer=i
            break
    return answer
def getpublickey(x,a,q):
    Y=(mpow(a,x)%q)
    return Y
def encryption(y,a,q,k,M):
    C1=int(mpow(a,k)%q)
    K=int(mpow(y,k)%q)
    C2=int((M*K)%q)
    print("C1:",C1)
    print("K:",K),
    print("C2:",C2)
def decryption(x,C1,C2,q):
    K=(mpow(C1,x)%q)
    print(K)
    M=(C2*(pow(int(K),q-2,q)))%q
    print("M:",M)
    return M
#print(searchx(5,157,48))
#print(getpublickey(22,5,157))
#print(encryption(48,5,157,3,5))
#print(decryption(5,3,11,13))
def getrandnum(a,q,C1):
    k=[]
    for r in range(1,q):
        if (mpow(a,r)%q)==C1:
            k.append(r)
    return k
def main():
    while True:
        function=input('function key x:get private key / pk:get public key/e:encryption / d:decryption /gr:getrandnum :')
        if function=="x":
            a=int(input("generator: "))
            q=int(input("modular: "))
            y=int(input("public key: "))
            print(searchx(a,q,y))
        elif function=="pk":
            x=int(input("private key: "))
            a=int(input("generator: "))
            q=int(input("modular: "))
            print(getpublickey(x,a,q))
        elif function=="e":
            y=int(input("public key: "))
            a=int(input("generator: "))
            q=int(input("modular: "))
            k=int(input("random number: "))
            M=int(input("message: "))
            print(encryption(y,a,q,k,M))
        elif function=="d":
            x=int(input("private key: "))
            C1=int(input("C1: "))
            C2=int(input("C2: "))
            q=int(input("modular: "))
            print(decryption(x,C1,C2,q))
        else:
            a=int(input("generator: "))
            q=int(input("modulator: "))
            C1=int(input("C1: "))
            print(getrandnum(a,q,C1))
if __name__=="__main__":
    main()