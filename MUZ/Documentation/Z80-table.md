#  Essai de table pour encodage Z-80


|symboles|signification|fonction de recognizing|
|===|===|===|
r | registre 8 bits | reg8()
rp | registre 16 bits | reg16()
n | nombre 8 bits
nn | nombre 16 bits
b | nombre de 0 à 7
e | nombre 8 bits signé

spécification de registre 8 bits : r=<definitions>
<definitions> : <definition>[,<definitions>]
<definition>: <registre>:<codes>

spécification registre 16 bits: rp=<definitions>


ADC A,r=A:8F,B:88,C:89,D:8A,E:8B,H:8C,L:8D
ADC A,n=CE n
ADC A,(HL)=8E
ADC A,(IX+n) =DD 8E n
ADC A,(IY+n)=FD 8E n
ADC HL,rp=BC:ED 4A,DE:ED 5A,HL:ED 6A,SP:ED 7A
