def vigenere(text,key):
    ciphertext=''
    for i in  range (0,len(text)):
        icode=0
        tcode=ord(text[i])-65
        kcode=ord(key[i%len(key)])-65
        if tcode+kcode>25:
            icode=tcode+kcode-26
        else:
            icode=tcode+kcode
        ciphertext+=chr(icode+65)
    print(ciphertext)
vigenere('EXPLANATION','LEG')