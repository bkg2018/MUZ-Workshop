# TODO pour MUZ-Workshop

Le source de Small Computer Monitor possibles pour l'assemblage : 

* 0.1.9 Original : ` /Users/bkg2018/Desktop/RETROCOMP/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source `
* 0.2.0 Original : ` /Users/bkg2018/Desktop/RETROCOMP/SCW020_SCM100_20190124 `
* 0.2.0 Modifié pour basic en ram switchable: ` /Users/bkg2018/Desktop/RETROCOMP/SCW020_SCM100_20190124.mod/SCMonitor/Source `


Modif de Small Computer Monitor:

- utilise config RC2014 Custom R0, fichiers dans Source / hardware / RC2014
- dans config.asm, `kROMTop` est en `0x3FFF` au lieu de `0x1FFF` car la ROM est sur 16K au lieu de 8K (on laisse de la place pour extension BASIC)
- dans ROM_Info.asm, la config R0 utilise un nouveau fichier `ROM_Info_R0_Bank1.asm` au lieu de celui indiqué d'origine qui est celui de la R1
- le fichier `ROM_Info_R0_Bank1.asm` est un R4 modifié dans lequel il y a un loader qui:
	- BasicRAMLoader: copie la ROM (16 KB) vers la RAM à partir de 8000h (transfert 1)
	- saute vers la suite de son propre code (ContinueLoader:) 8000h plus haut
	- ContinueLoader: page-out les 32K ROM du bas (000-7FFF) ce qui remplace par la RAM
	- recopie les 16KB vers le bas de mémoire (transfert 2)
	- Saute au démarrage normal du BASIC en bas de mémoire
	
Il reste à refaire un page-in de la ROM dans l'instruction SYSTEM du BASIC, pour cela il faut modifier les sources du BASIC, et générer un autre 
HEX qui remplacera `.\Apps\MSBASIC_adapted_by_GSearle\SCMon_BASIC_code2000_data4000.hex` dans `ROM_Info_R0_Bank1.asm`
	
	
Modifier la fin du source (ligne `MONITR:     JP      kMonWarm    ; Warm start Monitor   <SCC> original $0`) pour tester si on a de la ram à un endroit sans danger dans les premiers 16K:

- préserver la valeur à l'adresse choisie (dans un registre ou en pile)
- faire un XOR de cette valeur avec $FF
- la stocker à l'adresse
- la comparer à ce qui est effectivement stocké
   - si identique : (c'est de la RAM) il faut faire le pagein
- puis saut normal vers `kMonWarm`