def findkey():
    plaintext=input('plaintext:')
    cipheredtext=input('ciphered:')
    keyarray=[]
    for i in range(len(plaintext)):
        key=0
        ccode=ord(cipheredtext[i])
        pcode=ord(plaintext[i])
        if ccode<pcode:
            key+=ccode-64
            key+=90-pcode
        else:
            key+=ccode-pcode
        keyarray.append(key)
    print(keyarray)
findkey()