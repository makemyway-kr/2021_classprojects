def vigenere():
    text=input()
    key=[]
    ciphertext=''
    for i in  range (0,len(text)):
        key.append(int(input('input key')))
        icode=0
        tcode=ord(text[i])-65
        kcode=key[i]
        if tcode+kcode>25:
            icode=tcode+kcode-26
        else:
            icode=tcode+kcode
        ciphertext+=chr(icode+65)
    print(ciphertext)
vigenere()