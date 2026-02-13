from sys import argv
from os import system

def edit_modarith_output(filename):
    f = open(filename,"r")
    lines = f.readlines()
    f.close()
    if(" 0x" in lines[2]):
        l = lines[2].split(" ")
        r = l[-1]
        lines[2] = " ".join(l[:-1])+"\n// "+r
    for i in range(len(lines)):
        if(len(lines[i].split(" "))>2):
            if(lines[i].split(" ")[2]=="inline"):
                l = lines[i].split(" ")
                l[1],l[2] = l[2],l[1]
                lines[i]=" ".join(l)
    w = open(filename,"w")
    w.write("".join(lines))
    w.close()


def make_fp(mp,Cp,ws,p,pn,es):
    cmd0 = "python3 "+mp+"monty.py " + str(ws)+" "+hex(p)+"\n"
    cmd1 = "mkdir "+ Cp +"src/gf/ref/"+pn+"\n"
    cmd2 = "cp field.c " + Cp+"src/gf/ref/"+pn+"/fp_"+pn+"_"+str(ws)+".c\n"
    cmd3 = "mkdir "+ Cp +"src/gf/ref/"+pn+"/test/\n"
    cmd4 = "python3 " + Cp + "fp_appendix.py " + str(es) + " " + Cp+"src/gf/ref/"+pn+"/fp_"+pn+"_"+str(ws)+".c\n"
    CMLp = "set(SOURCE_FILES_GF_SPECIFIC\n\tfp_"+pn+"_"+str(ws)+".c\n)\n\ninclude(../lvlx.cmake)\n"
    CMLt = "include(../../lvlx_test.cmake)\n"
    system(cmd0)
    system(cmd1)
    system(cmd2)
    system(cmd3)
    edit_modarith_output(Cp +"src/gf/ref/"+pn+"/fp_"+pn+"_"+str(ws)+".c")
    system(cmd4)
    f = open(Cp+"src/gf/ref/"+pn+"/CMakeLists.txt","w")
    f.write(CMLp)
    f.close()
    ft = open(Cp+"src/gf/ref/"+pn+"/test/CMakeLists.txt","w")
    ft.write(CMLt)
    ft.close()
    return

def write_CML(dir,CML):
    system("touch "+dir+"CMakeLists.txt")
    fcml = open(dir+"CMakeLists.txt","w")
    fcml.write(CML)
    fcml.close()
    return

def make_dirs(Cp,pn,p):
    dirs = ["src/ec/ref/","src/hd/ref/","src/nike/ref/","src/precomp/ref/"]
    for dir in dirs:
        system("mkdir "+Cp+dir+pn)
    system("mkdir "+Cp+"src/ec/ref/"+pn+"/test")
    system("mkdir "+Cp+"src/nike/ref/"+pn+"/test")
    system("mkdir "+Cp+"src/precomp/ref/"+pn+"/include")
    CMLec="include(../lvlx.cmake)\n"
    CMLect="include(../../lvlx_test.cmake)"
    CMLhd="include(../lvlx.cmake)"
    CMLnike="include(../lvlx.cmake)\n"
    CMLniket="include(../../lvlx_test.cmake)\n"
    CMLprecomp="include(../lvlx.cmake)\n"
    write_CML(Cp+dirs[0]+pn+"/",CMLec)
    write_CML(Cp+dirs[1]+pn+"/",CMLhd)
    write_CML(Cp+dirs[2]+pn+"/",CMLnike)
    write_CML(Cp+dirs[3]+pn+"/",CMLprecomp)
    write_CML(Cp+"src/ec/ref/"+pn+"/test/",CMLect)
    write_CML(Cp+"src/nike/ref/"+pn+"/test/",CMLniket)
    system("touch "+Cp+"src/precomp/ref/"+pn+"/sqisign_parameters.txt")
    fpmt = open(Cp+"src/precomp/ref/"+pn+"/sqisign_parameters.txt","w")
    fpmt.write("lvl = {pn}\np = {ph}\nnum_orders=1\nprimes_per_order={ppo}\ngood_primes = {gp}\nbad_primes = {bp}\n".format(pn=pn,ph=hex(p),ppo=101,gp=((p.bit_length())//200)*100,bp=((p.bit_length())//100)*100))
    fpmt.close()
    return

def precomp_setup(Cp,pn):
    system("touch "+Cp+"src/precomp/ref/"+pn+"/e0_basis.c")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/ec_params.c")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/endomorphism_action.c")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/hd_splitting_transforms.c")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/quaternion_data.c")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/torsion_constants.c")

    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/e0_basis.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/ec_params.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/encoded_sizes.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/endomorphism_action.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/fp_constants.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/hd_splitting_transforms.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/quaternion_constants.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/quaternion_data.h")
    system("touch "+Cp+"src/precomp/ref/"+pn+"/include/torsion_constants.h")
    return

def adjust_CMakeLists(Cp,pn):
    f=open(Cp+"CMakeLists.txt","r")
    rl = f.readlines() 
    f.close()
    for i in range(len(rl)):
        r = rl[i]
        if(r.split(" ")[0]=="SET(SVARIANT_S"):
            assert(r.split(" ")[1][-3]=="\"")
            if(pn not in r.split(" ")[1][:-3]):
                ntxt = r.split(" ")[1][:-3]+";"+pn+"\")\n"
                rl[i] = r.split(" ")[0] + " "+ntxt
    txt = "".join(rl)
    f = open(Cp+"CMakeLists.txt","w")
    f.write(txt)
    f.close()
    return

if __name__=="__main__":
#Take input: Encoding size, Wordsize,p, path to modarith, path to C-code, primename
    encsize = int(argv[1],10)
    wordsize = int(argv[2],10)
    p = int(argv[3],16)
    modarith_path = argv[4]
    C_path = argv[5]
    Pname = argv[6]
    make_fp(modarith_path,C_path,wordsize,p,Pname,encsize)
    make_dirs(C_path,Pname,p)
    precomp_setup(C_path,Pname)
    adjust_CMakeLists(C_path,Pname)
